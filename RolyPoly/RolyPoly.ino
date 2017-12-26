#include <Wire.h>
#include <SparkFun_MMA8452Q.h>
MMA8452Q accel;

const int row1 = 2;
const int row2 = 3;
const int row3 = 4;
const int row4 = 5;
const int row5 = 6;
const int row6 = 7;
const int row7 = 8;
const int col1 = 9;
const int col2 = 10;
const int col3 = 11;
const int col4 = 12;
const int col5 = 13;

unsigned long currentTime = 0;
unsigned long startingTime = 0;
unsigned long endingTime = 0;
unsigned long nextDraw1 = 0;
unsigned long nextDraw2 = 0;
unsigned long nextScreen = 0;
unsigned long nextOff = 0;
unsigned long nextDelay = 0;

const unsigned long gameTime = 15000;
const unsigned long endTime = 4000;
const unsigned long fourHz = 250;
const unsigned long showTime = 100;
const unsigned long delayTime = 1;

int preyWin = 0;
int predatorWin = 0;
int i = 0;

boolean gameJustStarted = false;
boolean readyToStart = true;
boolean preyJustWon = false;
boolean predatorJustWon = false;
boolean winCountReady = false;
boolean gameNotOver = true;

int preyRow = row1;
int preyCol = col3;
int predatorRow = row7;
int predatorCol = col2;
boolean positions[7][5];

void setup() {
  Serial.begin(9600);
  
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  pinMode(row3, OUTPUT);
  pinMode(row4, OUTPUT);
  pinMode(row5, OUTPUT);
  pinMode(row6, OUTPUT);
  pinMode(row7, OUTPUT);
  pinMode(col1, OUTPUT);
  pinMode(col2, OUTPUT);
  pinMode(col3, OUTPUT);
  pinMode(col4, OUTPUT);
  pinMode(col5, OUTPUT);
  
  accel.init();
  
  reset();
}

void loop() {
  currentTime = millis();
  if(readyToStart) { 
    startGame();
    readyToStart = false; 
  }
  if(currentTime >= nextDelay) {
    reset();
    drawCol();
    nextDelay += delayTime;
  }
  if (gameNotOver)
    playGame();
  if (!gameNotOver) {
    if (currentTime >= nextScreen) {
      endGame();
      nextScreen += delayTime*2;
    }
   if (currentTime >= nextScreen-delayTime)
      displayScore();
  }
}

void on() {
  digitalWrite(row1, HIGH);
  digitalWrite(row2, HIGH);
  digitalWrite(row3, HIGH);
  digitalWrite(row4, HIGH);
  digitalWrite(row5, HIGH);
  digitalWrite(row6, HIGH);
  digitalWrite(row7, HIGH);
  digitalWrite(col1, LOW);
  digitalWrite(col2, LOW);
  digitalWrite(col3, LOW);
  digitalWrite(col4, LOW);
  digitalWrite(col5, LOW);
}

void reset() {
  digitalWrite(row1, LOW);
  digitalWrite(row2, LOW);
  digitalWrite(row3, LOW);
  digitalWrite(row4, LOW);
  digitalWrite(row5, LOW);
  digitalWrite(row6, LOW);
  digitalWrite(row7, LOW);
  digitalWrite(col1, HIGH);
  digitalWrite(col2, HIGH);
  digitalWrite(col3, HIGH);
  digitalWrite(col4, HIGH);
  digitalWrite(col5, HIGH);
}

void startGame(){
  preyRow = random(row1, row7+1);
  preyCol = random(col1, col5+1);
  predatorRow = random(row2, row7+1);
  predatorCol = random(col1, col5);
  while (overlaps()) {
    predatorRow = random(row2, row7+1);
    predatorCol = random(col1, col5);
  }
  gameJustStarted = true;
  gameNotOver = true;
  winCountReady = true;
}

void playGame() {
  if (gameJustStarted) {
    startingTime = millis();
    gameJustStarted = false;
  } 
  if (currentTime >= nextDraw1) {
    preyMove();
    predatorMove();
    nextDraw1 += showTime;
  }
  if (gameNotOver) {
    preyJustWon = preyWon();
    predatorJustWon = predatorWon();
  }
  if (preyJustWon || predatorJustWon) {
    endingTime = millis();
    gameNotOver = false;
  }   
}

void endGame() {
  if (preyJustWon) {
    if (winCountReady) {
      preyWin++; 
      winCountReady = false; 
    }
    preyRow = row4;
    preyCol = col3;
    predatorRow = 0;//
    predatorCol = 0;//
  }
  if (predatorJustWon) {
    if (winCountReady) {
      predatorWin++;
      winCountReady = false;
    }
    predatorWin++;
    preyRow = 0;//
    preyCol = 0;//
    predatorRow = row4;
    predatorCol = col2;
  }
  if (millis()-endingTime > endTime) {
    readyToStart = true;
    preyJustWon = false;
    predatorJustWon = false;
  }
}

void displayScore() {
  digitalWrite(row7, HIGH);
  digitalWrite(col3, LOW);
  if (preyJustWon) {
    if (preyWin > predatorWin) {
      if (preyWin-predatorWin >= 1) 
        digitalWrite(col4, LOW);
      if (preyWin-predatorWin > 2)
        digitalWrite(col5, LOW);   
    }
    if (preyWin < predatorWin) {
      if(predatorWin-preyWin >= 1)
        digitalWrite(col2, LOW);
      if(predatorWin-preyWin > 2)
        digitalWrite(col1, LOW);
    }
  }
  if (predatorJustWon) {
    if (preyWin > predatorWin) {
       if (preyWin-predatorWin >= 1) 
         digitalWrite(col2, LOW);
       if (preyWin-predatorWin > 2)
         digitalWrite(col1, LOW);
    }
    if (preyWin < predatorWin) {
      if(predatorWin-preyWin >= 1)
        digitalWrite(col4, LOW);
      if(predatorWin-preyWin > 2)
        digitalWrite(col5, LOW);
    }
  }
}

void drawCol() {
  digitalWrite(col1 + (i%5), LOW);
  if(preyCol == (col1+i%5))
    digitalWrite(preyRow, HIGH);
  if((predatorCol == (col1+i%5)) || (predatorCol+1 == (col1+i%5))) {
    digitalWrite(predatorRow, HIGH);
    digitalWrite(predatorRow-1, HIGH);
  }
  if (currentTime >= nextOff) {
    if((predatorCol == (col1+i%5)) || (predatorCol+1 == (col1+i%5))) {
      digitalWrite(predatorRow, LOW);
      digitalWrite(predatorRow-1, LOW);
    }
    nextOff += fourHz*3;
  }
  ++i;
}

void setDisplay() {
  positions[preyRow-row1][preyCol-col1] = true;
  positions[predatorRow-row1][predatorCol-col1] = true;
  positions[predatorRow-1-row1][predatorCol-col1] = true;
  positions[predatorRow-row1][predatorCol+1-col1] = true;
  positions[predatorRow-1-row1][predatorCol+1-col1] = true;
}

void drawScreen() {
  for (int row = 0; row < 7; ++row) {
    for (int col = 0; col < 5; ++col) {
      positions[row][col] = false;
    }
  }
}

void preyMove() {
  if (accel.available()) {
    accel.read();
    byte prey = accel.readPL();
    switch(prey) {
      case PORTRAIT_U:
        if (preyRow != row7)
          preyRow++;
        break;
      case PORTRAIT_D:
        if (preyRow != row1)
          preyRow--;
        break;
      case LANDSCAPE_R:
        if (preyCol != col5)
          preyCol++;
        break;
      case LANDSCAPE_L:
        if (preyCol != col1)
          preyCol--;
        break;
    }
  }
}

void predatorMove() {
  if (Serial.available() > 0) {
    byte predator = Serial.read();
    switch(predator) {
      case 'w':
        if (predatorRow != row2)
          predatorRow--;
        break;
      case 'a':
        if (predatorCol != col1)
          predatorCol--;
        break;
      case 's':
        if (predatorRow != row7)
          predatorRow++;
        break;
      case 'd':
        if (predatorCol != col4)
          predatorCol++;
        break;
    }
  }
}

boolean preyWon() {
  return (millis()-startingTime > gameTime); 
}

boolean predatorWon() {
  return overlaps();
}

boolean overlaps() {
  return (((preyRow == predatorRow) && (preyCol == predatorCol)) 
    || ((preyRow == predatorRow) && (preyCol == predatorCol+1)) 
    || ((preyRow == predatorRow-1) && (preyCol == predatorCol)) 
    || ((preyRow == predatorRow-1) && (preyCol == predatorCol+1)));
}


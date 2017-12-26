import java.awt.event.KeyEvent;
import java.io.IOException;

import jssc.SerialPortException;

public class PCPlayer {
	public static void main(String[] args) throws SerialPortException, IOException {
		String portName = "/dev/cu.usbserial-DN02BJDS";
		SerialComm port = new SerialComm(portName);
		int delta = 200;
		long nextTime = 0;
		while(true) {
			long time = System.currentTimeMillis();
			if(time > nextTime) {
				if(StdDraw.isKeyPressed(KeyEvent.VK_UP)) {
					port.writeByte((byte)'w');
					System.out.println("up");
					nextTime = time;
					nextTime += delta;
				}
				else if(StdDraw.isKeyPressed(KeyEvent.VK_DOWN)) {
					port.writeByte((byte)'s');
					System.out.println("down");
					nextTime = time;
					nextTime += delta;
				}
				else if(StdDraw.isKeyPressed(KeyEvent.VK_LEFT)) {
					port.writeByte((byte)'a');
					System.out.println("left");
					nextTime = time;
					nextTime += delta;
				}
				else if(StdDraw.isKeyPressed(KeyEvent.VK_RIGHT)) {
					port.writeByte((byte)'d');
					System.out.println("right");
					nextTime = time;
					nextTime += delta;
				}
			}
		}
	}
}



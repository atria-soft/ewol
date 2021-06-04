/*******************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Contributors:
 *     Edouard DUPIN - initial API and implementation
 ******************************************************************************/
package test.atriasoft.esignal;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

import io.scenarium.logger.Logger;

import org.atriasoft.esignal.Connection;
import org.atriasoft.esignal.Signal;
import org.junit.Test;
import org.junit.jupiter.api.MethodOrderer.OrderAnnotation;
import org.junit.jupiter.api.Order;
//import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;

@TestMethodOrder(OrderAnnotation.class)
public class TestSignal {

	class EmiterSimple {
		public Signal<String> signalEvent = new Signal<String>();
		public void sendEvent(String value) {
			signalEvent.emit(value);
		}
	}
	class ReceiverSimple {
		private String dataReceive = null;
		ReceiverSimple() {
		}
		public void connect1(EmiterSimple other) {
			WeakReference<ReceiverSimple> tmpp = new WeakReference<ReceiverSimple>(this);
			other.signalEvent.connect(data -> {
					tmpp.get().onData(data);
				});
		}
		public void connect2(EmiterSimple other) {
			// the solo lambda will not depend on the object => the remove must be done manually... 
			other.signalEvent.connect(data -> {
					Log.error("lambda receive: " + data);
				});
		}
		public void connect3(EmiterSimple other) {
			// we reference the local object, then the lambda is alive while the object is alive...
			other.signalEvent.connect(data -> {
					Log.error("lambda receive: " + data);
					this.dataReceive = data;
				});
		}
		public void connect4(EmiterSimple other) {
			other.signalEvent.connect(data -> {
					onData(data);
				});
		}
		public void connect5(EmiterSimple other) {
			other.signalEvent.connect(this::onData);
		}
// Does not work at all:
//		public void disconnect5(EmiterSimple other) {
//			other.signalEvent.disconnect(this::onData);
//		}

		public void connect6(EmiterSimple other) {
			// the solo lambda will not depend on the object => the remove must be done manually... 
			other.signalEvent.connectAutoRemoveObject(this, data -> {
					Log.error("lambda receive: " + data);
				});
		}
		private Connection tmpConnect = null;
		
		public void connect7(EmiterSimple other) {
			tmpConnect = other.signalEvent.connectDynamic(this::onData);
		}

		public void disconnect7(EmiterSimple other) {
			other.signalEvent.disconnect(tmpConnect);
		}

		public void disconnect72() {
			tmpConnect.disconnect();
		}
		public boolean isConnected() {
			return tmpConnect.isConnected();
		}
		
		public void onData(String data) {
			Log.error("Retrive data : " + data);
			dataReceive = data; 
		}
		public String getDataAndClean() {
			String tmp = dataReceive;
			dataReceive = null;
			return tmp;
		}
		
	}

	@Test
	@Order(1)
	public void testConnectAndTransmit1() {
		Log.warning("Test 1 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect1(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		assertEquals(testData1, receiver.getDataAndClean());
		receiver = null;
		assertEquals(1, sender.signalEvent.size());
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 1 [ END ]");
		
	}
	@Test
	@Order(2)
	public void testConnectAndTransmit2() {
		Log.warning("Test 2 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect2(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		// No data stored ... assertEquals(testData1, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "Solo Lambda MUST receive this data...";
		sender.sendEvent(testData2);
		assertEquals(1, sender.signalEvent.size());
		Log.warning("Test 2 [ END ]");
	}

	@Test
	@Order(3)
	public void testConnectAndTransmit3() {
		Log.warning("Test 3 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect3(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		assertEquals(testData1, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 3 [ END ]");
		
	}

	@Test
	@Order(4)
	public void testConnectAndTransmit4() {
		Log.warning("Test 4 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect4(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		assertEquals(testData1, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 4 [ END ]");
		
	}

	@Test
	@Order(5)
	public void testConnectAndTransmit5() {
		Log.warning("Test 5 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect5(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		assertEquals(testData1, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size());
		// remove connection
//		receiver.disconnect5(sender);
//		assertEquals(0, sender.signalEvent.size()); 
//		System.gc();
//		String testData2 = "MUST NOT receive this data... 222";
//		sender.sendEvent(testData2);
//		assertEquals(null, receiver.getDataAndClean());
//		// reconnect (step 2
//		receiver.connect5(sender);
//		assertEquals(1, sender.signalEvent.size()); 
//		String testData3 = "MUST receive this data... 333";
//		sender.sendEvent(testData3);
//		assertEquals(testData3, receiver.getDataAndClean());
//		assertEquals(1, sender.signalEvent.size());
		// check auto remove...
		receiver = null;
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 5 [ END ]");
		
	}
	@Test
	@Order(6)
	public void testConnectAndTransmit6() {
		Log.warning("Test 6 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		receiver.connect6(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data...";
		sender.sendEvent(testData1);
		//assertEquals(testData1, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size()); 
		receiver = null;
		System.gc();
		String testData2 = "MUST NOT receive this data...";
		sender.sendEvent(testData2);
		assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 6 [ END ]");
		
	}

	@Test
	@Order(7)
	public void testConnectAndTransmit7() {
		Log.warning("Test 7 [BEGIN]");
		EmiterSimple sender = new EmiterSimple();
		ReceiverSimple receiver = new ReceiverSimple();
		//connect step 1
		receiver.connect7(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData1 = "MUST receive this data... 111";
		sender.sendEvent(testData1);
		assertEquals(testData1, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size());
		assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnect7(sender);
		assertEquals(false, receiver.isConnected());
		System.gc();
		String testData2 = "MUST NOT receive this data... 222";
		sender.sendEvent(testData2);
		assertEquals(0, sender.signalEvent.size()); 
		assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect7(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData3 = "MUST receive this data... 333";
		sender.sendEvent(testData3);
		assertEquals(testData3, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size());
		assertEquals(true, receiver.isConnected());
		// remove connection
		receiver.disconnect72();
		assertEquals(false, receiver.isConnected());
		assertEquals(0, sender.signalEvent.size()); 
		System.gc();
		String testData4 = "MUST NOT receive this data... 444";
		sender.sendEvent(testData4);
		assertEquals(null, receiver.getDataAndClean());
		// reconnect (step 2
		receiver.connect7(sender);
		assertEquals(1, sender.signalEvent.size()); 
		String testData5 = "MUST receive this data... 555";
		sender.sendEvent(testData5);
		assertEquals(testData5, receiver.getDataAndClean());
		assertEquals(1, sender.signalEvent.size());
		// check auto remove...
		receiver = null;
		System.gc();
		String testData6 = "MUST NOT receive this data... 666";
		sender.sendEvent(testData6);
		assertEquals(0, sender.signalEvent.size());
		Log.warning("Test 7 [ END ]");
		
	}

}

/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.obex.*;
import java.io.*;
import java.util.Enumeration;
import java.util.Vector;
import javax.bluetooth.*;

/**
 *
 * Minimal Services Search example.
 */
class ServiceSearch {

	static final UUID OBEX_OBJECT_PUSH = new UUID("1105", true);
	static int port = -1;

	public static int search(String address) throws Exception {

		final Object serviceSearchCompletedEvent = new Object();

		DiscoveryListener listener = new DiscoveryListener() {
			public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
			}
			public void inquiryCompleted(int discType) {
			}

			public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
				for (int i = 0; i < servRecord.length; i++) {
					String url = servRecord[i].getConnectionURL(ServiceRecord.NOAUTHENTICATE_NOENCRYPT, false);
					if (url == null) {
						continue;
					}
					port = Integer.parseInt(url.substring(22, url.indexOf(';', 22)));
					System.out.println("port found: " + port);
				}
			}

			public void serviceSearchCompleted(int transID, int respCode) {
				System.out.println("service search completed.");
				synchronized (serviceSearchCompletedEvent) {
					serviceSearchCompletedEvent.notifyAll();
				}
			}
		};

		UUID[] searchUuidSet = new UUID[] { OBEX_OBJECT_PUSH };
		//int[] attrIDs = new int[] {};

		class MyRemoteDevice extends RemoteDevice {
			public MyRemoteDevice(String address) {
				super(address);
			}
		};
		RemoteDevice btDevice = new MyRemoteDevice(address);

		synchronized (serviceSearchCompletedEvent) {
			System.out.println("search services on " + btDevice.getBluetoothAddress() + " " +
				btDevice.getFriendlyName(false));
			LocalDevice.getLocalDevice().getDiscoveryAgent().searchServices(null, searchUuidSet,
				btDevice, listener);
			serviceSearchCompletedEvent.wait();
			if(port == -1)
				throw new IOException("No service port found");
			return port;
		}
	}
}

public class Bcobex {


	public static void main(String[] args) throws Exception {
		if(args == null || args.length != 2) {
			System.out.println("Usage: bcobex <address> <file>\n" + 
				"\n" + 
				"Sends file to bluetooth address.\n" + 
				"Address format: %012x\n" +
				"\n" + 
				"Example: bcobex 012345689ABCD someApp.jar\n" +
				"\n");
			return;
		}
		String address = args[0];
		String filename = args[1];
		int port = ServiceSearch.search(address);
		sendObexFile(address, port, filename);
	}

	public static void sendObexFile(String address, int port, String filename) throws IOException {
		String url = "btgoep://" + address + ":" + port;

		FileInputStream file = new FileInputStream(filename);
		byte[] data = new byte[(int)file.getChannel().size()];
		new DataInputStream(file).readFully(data);
		file.close();

		System.out.println("Read " + data.length + " bytes.");
		System.out.println("Connecting to " + url + "...");

		ClientSession clientSession = (ClientSession)Connector.open(url);
		HeaderSet hsConnectReply = clientSession.connect(null);
		if (hsConnectReply.getResponseCode() != ResponseCodes.OBEX_HTTP_OK) {
			throw new IOException("OBEX failed to connect");
		}
		System.out.println("Connected.");

		HeaderSet head = clientSession.createHeaderSet();
		head.setHeader(HeaderSet.NAME, filename);
		head.setHeader(HeaderSet.LENGTH, new Long(data.length));

		//Create PUT Operation
		Operation putOperation = clientSession.put(head);

		// Send data to server
		OutputStream os = putOperation.openOutputStream();
		os.write(data);
		System.out.println("Data written.");
		os.close();

		putOperation.close();

		clientSession.disconnect(null);

		clientSession.close();
		System.out.println("System closed, operation finished.");
	}
}

/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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

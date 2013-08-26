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

/** \file BluetoothConnection.h
* \brief An easy way to connect to a Bluetooth service.
* \author Fredrik Eldh
*/

#ifndef _SE_MSAB_MAUTIL_BLUETOOTHCONNECTION_H_
#define _SE_MSAB_MAUTIL_BLUETOOTHCONNECTION_H_

#include "BluetoothDiscovery.h"
#include "Connection.h"

namespace MAUtil {

	/**
	* \brief Can connect to a Bluetooth service by UUID.
	* Works mostly like a Connection, except the connect() method is different.
	*
	* This class wraps the special behaviour of the Android platform, which
	* does not currently support Bluetooth service discovery.
	*/
	class BluetoothConnection :
		public Connection, BluetoothServiceDiscoveryListener {
	public:
		/**
		* \param listener Will receive events from this Connection.
		*/
		BluetoothConnection(ConnectionListener* listener);

		/**
		* Closes the connection, if open.
		* \see close()
		*/
		virtual ~BluetoothConnection();

		/**
		* Asynchronously opens a connection using a Bluetooth address and UUID.
		*
		* Service discovery is performed. If a service is found, a connection
		* is opened.
		*
		* This function returns immediately. If the connect operation could be
		* started, the return value will be \> 0, and connectFinished() of the
		* ConnectionListener associated with this Connection will be called
		* when the operation is finished.
		*
		* If the connect operation could NOT be started, the return value will
		* be a \link #CONNERR_GENERIC CONNERR \endlink code \< 0, and
		* connectFinished() will not be called.
		*
		* connectFinished() may use #CONNERR_NOTFOUND to indicate that the
		* requested service could not be found.
		*
		* \param address Address to the remote device.
		* \param uuid The UUID of the service you wish to connect to.
		*
		* \return \> 0 on success, \< 0 on error.
		*/
		int connect(const MABtAddr& address, const MAUUID& uuid);

		/**
		* Asynchronously opens a connection using a URL that specifies
		* a Bluetooth address and a service UUID.
		*
		* Service discovery is performed. If a service is found, a connection
		* is opened.
		*
		* This function returns immediately. If the connect operation could
		* be started, the return value will be \> 0, and connectFinished()
		* of the ConnectionListener associated with this Connection will be
		* called when the operation is finished.
		*
		* If the connect operation could NOT be started, the return value will
		* be a \link #CONNERR_GENERIC CONNERR \endlink code \< 0, and
		* connectFinished() will not be called.
		*
		* connectFinished() may use #CONNERR_NOTFOUND to indicate that the
		* requested service could not be found.
		*
		* \param url Has the format:
		* "btspp://<6 byte hex Bluetooth address>:<16 byte hex UUID>"
		* Example: "btspp://38E7D8C7605E:DCBDB690C17111DF851A0800200C9A66"
		*
		* \return \> 0 on success, \< 0 on error.
		*
		* \note This function supports only its own form of URL.
		* If you want to use another form with an object of this class,
		* you can call the superclass function with this syntax:
		* btConn.Connection::connect(url)
		*/
		int connect(const char* url);

		/** \copydoc Connection::close() */
		void close();

	private:
		virtual void btNewService(const BtService& serv);
		virtual void btServiceDiscoveryFinished(int state);

	private:
		BluetoothDiscoverer mBluetoothDiscoverer;
		
		MABtAddr mAddress;
		
		/**
		 * <0 when closed, ==0 when discovering, >0 when connect-ing/-ed.
		 */
		int mPort;
	};

}	//namespace MAUtil

#endif	//_SE_MSAB_MAUTIL_BLUETOOTHCONNECTION_H_


#ifndef _SE_MSAB_MAUTIL_BLUETOOTHCONNECTION_H_
#define _SE_MSAB_MAUTIL_BLUETOOTHCONNECTION_H_

#include "BluetoothDiscovery.h"
#include "Connection.h"

namespace MAUtil {

	/** \brief Can connect to a Bluetooth service by UUID.
	* Works mostly like a Connection, except the connect() method is different.
	*
	* This class wraps the special behaviour of the Android platform, which does not
	* currently support Bluetooth service discovery.
	*/
	class BluetoothConnection : public Connection, BluetoothServiceDiscoveryListener {
	private:
		BluetoothDiscoverer mBD;
		MABtAddr mAddr;
		int mPort;
	public:
		/**
		* \param listener Will recieve events from this Connection.
		*/
		BluetoothConnection(ConnectionListener* listener);

		/**
		* Closes the connection, if open.
		* \see close()
		*/
		virtual ~BluetoothConnection();

		/*
		* Asynchronously opens a connection using a Bluetooth address and UUID.
		*
		* Service discovery is performed. If a service is found, a connection is opened.
		*
		* This function returns immediately. If the connect operation could be started,
		* the return value will be \> 0, and connectFinished() of the
		* ConnectionListener associated with this Connection will be called
		* when the operation is finished.
		*
		* If the connect operation could NOT be started, the return value will be a
		* \link #CONNERR_GENERIC CONNERR \endlink code \< 0, and connectFinished() will
		* not be called.
		*
		* connectFinished() may use #CONNERR_NOTFOUND to indicate that the requested
		* service could not be found.
		*
		* \param address Address to the remote device.
		* \param uuid The UUID of the service you wish to connect to.
		*/
		int connect(const MABtAddr& address, const MAUUID& uuid);

		/** \copydoc Connection::close() */
		void close();

	private:
		virtual void btNewService(const BtService& serv);
		virtual void btServiceDiscoveryFinished(int state);
	};

}	//namespace MAUtil

#endif	//_SE_MSAB_MAUTIL_BLUETOOTHCONNECTION_H_

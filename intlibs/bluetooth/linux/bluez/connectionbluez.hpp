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

/* 
 * File:   connectionbluez.hpp
 * Author: Ali Mosavian
 *
 * Created on July 27, 2009
 */
#ifndef __CONNECTIONBLUEZ_HPP__
#define	__CONNECTIONBLUEZ_HPP__

#include <sys/socket.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "../../connection.h"



namespace Bluetooth
{
    namespace BlueZ
    {
        /**
         * Blocking RFCOMM socket
         *
         */
        class BtSppConnectionBluez : public BtSppConnection
        {
        private:
            sockaddr_rc mAddr;
            int         mSocket;

        public:
            /**
             * Constructor, creates the socket
             *
             * @param a Pointer to bluetooth adress
             * @param p Port to connect to
             */
            BtSppConnectionBluez ( const MABtAddr* a, uint p );

            /**
             * Destructor, will close any connection and free
             * resources
             *
             */
             virtual ~BtSppConnectionBluez ( void );

            // From connection
            /**
             * Attempts to connect to the remote host
             *
             * @return > 0      On success
             *         CONNERR  On failure
             */
            virtual int connect ( void );

            /**
             * Reads one to max bytes from the the socket without blocking.
             *
             * @param dst   Pointer to the buffer to write to.
             * @param max   Maximum number of bytes to read.
             *
             * @return Number of read bytes or CONNERR if failed.
             */
            virtual int read ( void* dst, int max );

            /**
             * Sends the supplied number of bytes to the destination.
             *
             * @param src   Pointer to the buffer that holds the data.
             * @param len   Number of bytes to send.
             *
             * @return > 0      The data has been sent.
             *         CONNERR, If there was an error.
             */
            virtual int write ( const void* src, int len );

            // From Closable
            /**
             * Closes any open connections
             *
             */
            virtual void close ( void );

            /**
             * Returns the adress of the remote host.
             *
             * @param a     Structure to fill
             */
            virtual int getAddr ( MAConnAddr& a );
	    
	    virtual bool isConnected();
        };
    }
}

#endif	/* __CONNECTIONBLUEZ_HPP__ */


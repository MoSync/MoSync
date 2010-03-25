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

/* 
 * File:   bluetoothbluez.hpp
 * Author: Ali Mosavian
 *
 * Created on July 21, 2009
 */

#ifndef __BLUETOOTHBLUEZ_HPP__
#define	__BLUETOOTHBLUEZ_HPP__

#include <list>
#include "cbtdevice.hpp"
#include "cbtservice.hpp"
#include "../../discovery.h"
#include "../lock.hpp"
#include "../mutex.hpp"
#include "../thread.hpp"

namespace Bluetooth
{
    namespace BlueZ
    {
        class BluetoothBluez
        {
        private:
            typedef Base::Thread::Functor Functor;

            int                     mDevID;
            int                     mState;
            bool                    mWorking;
            Thread*                 mDiscThread;
            bool                    mThreadQuit;
            Functor*                mMainFunctor;

            std::list<Functor *>    mTaskList;
            Mutex*                  mTaskListMutex;

            std::list<CBtDevice *>  mDeviceList;
            Mutex*                  mDeviceListMutex;

            std::list<CBtService *> mServiceList;
            Mutex*                  mServiceListMutex;



            /**
             * Thread stub, runs tasks until it has been signaled to quit.
             *
             */
            void threadStub ( void );

            /**
             * This method is used for discovering new devices
             *
             * @param cb    The callback to invoke onces that the discovery
             *              has finished.
             * @param n 	Attempt to discover device names
             */
            void discoverDevices ( MABtCallback cb, bool n );

            /**
             * This method is used for performing a service discovery.
             *
             * @param cb    The callback to invoke onces that the discovery
             *              has finished.
             * @param a     Address of the device to do discovery on.
             * @param u     The UUID to search for.
             */
            void discoverServices ( MABtCallback cb, MABtAddr a, MAUUID u );

        public:
            /**
             * Constructor, creates thread and mutex
             *
             */
            BluetoothBluez ( void );

            /**
             * Destructor, deletes allocated resources
             *
             */
            ~BluetoothBluez ( void );

            /**
             * Starts a device discovery
             *
             * @param cb    The callback to invoke onces that the discovery
             *              has finished.
             * @param n 	Attempt to discover device names
			 *
			 * @return 0 on success, \< 0 on failure.
             */
            int startDiscovery ( MABtCallback cb, bool n );

            /**
             * Returns and removes the next discovered device.
             *
             * @param d Pointer to the structure to fill.
             *
             * @return 1 if there was a device, 0 if not
             */
            int getNextDevice ( MABtDevice *d );

            /**
             * Starts a service discovery on a device
             *
             * @param cb    The callback to invoke onces that the discovery
             *              has finished.
             * @param a     Address of the device to do discovery on.
             * @param u     The UUID to search for.
			 *
			 * @return 0 on success, \< 0 on failure.	
             */
            int startServiceDiscovery ( MABtCallback cb, const MABtAddr* a, const MAUUID* u );


            /**
             * Returns and removes the next newly found service.
             *
             * @param d     Pointer to the structure to fill
             *
             * @return 1 if there was a service, 0 if not
             */
            int getNextService ( MABtService* d );

            /**
             * Returns the number of UUIDs the next service
             * has.
             *
             * @param d     Pointer to the structure to fill.
             *
             * @return 1 If there was a service. 0 if not.
             */
            int getNextServiceSize ( MABtServiceSize* d );


            /**
             * Returns the current state
             *
             */
            int getState ( void );

            /**
             * Returns the local bluetooth interface adress
             *
             * @return < 0    Operation failed.
             */
            int getLocalAddress ( MABtAddr& a );
        };
    }
}

#endif	/* __BLUETOOTHBLUEZ_HPP__ */


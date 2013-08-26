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

			enum WORK_STATE
			{
				IDLE,
				DEVICE_DISC,
				SERVICE_DISC,
				DEVICE_DISC_CANCELED
			};

            int                     mDevID;
            int                     mState;
            WORK_STATE              mWorkState;

            Thread*                 mDiscThread;
            bool                    mThreadQuit;
            Functor*                mMainFunctor;

            std::list<Functor *>    mTaskList;
            Mutex*                  mTaskListMutex;

            std::list<CBtDevice *>  mDeviceList;
            Mutex*                  mDeviceListMutex;
			Mutex*                  mDeviceDiscoveryCritical;

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

            /**
             * This method is to clean up after a canceled discovery. After
			 * it has run, it will change the work state to IDLE.
             *
             */
            void doCleanupDiscoverDevices ( void );

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
            int getNextDevice ( MABtDeviceNative* d );

			/**
			 * Cancels an on going device discovery, 
			 * Note: If an operation was canceled, its last BT event will have 
			 *       the status CONNERR_CANCELED. This is an asynchronous operation. 
			 *       It is not safe to start another discovery before you've recieved 
			 *       the CONNERR_CANCELED event.
			 *
			 * @return 0 if there was no active operation
			 *         1 if there was.
			 */
            int cancelDeviceDiscovery ( void );

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
            int getNextService ( MABtServiceNative* d );

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


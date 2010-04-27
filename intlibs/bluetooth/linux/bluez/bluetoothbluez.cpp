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
 * File:   bluetoothbluez.cpp
 * Author: Ali Mosavian
 *
 * Created on July 21, 2009
 */
#include <cstdlib>
#include <algorithm>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>

#include "../../config_bluetooth.h"
#include "bluezcommon.hpp"
#include "bluetoothbluez.hpp"
#include "helpers/helpers.h"
#include "helpers/cpp_defs.h"
#include "thread/thread.hpp"
#include "../../bt_errors.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT( S ) fprintf( stderr, "%s:%d:%s\n", __FILE__, __LINE__, S )
#else
#define DEBUG_PRINT( S )
#endif

using namespace Base::Thread;
using namespace Bluetooth::BlueZ;


/**
 * Simple class that closes file descriptor,
 * for forgetful people.
 *
 */
class DescriptorCloser
{
private:
    int m_fd;

public:
    DescriptorCloser ( int fd ) : m_fd( fd ) { }
    ~DescriptorCloser ( void ) { close( m_fd ); }
};



/**
 * Constructor, creates thread and mutex
 *
 */
BluetoothBluez::BluetoothBluez ( void )
{
    mState      = 1;
    mThreadQuit = false;
    mWorkState  = IDLE;

    mTaskListMutex   = new Mutex( );
    mDeviceListMutex = new Mutex( );
    mServiceListMutex= new Mutex( );
    mDiscThread      = new Thread( );
	mDeviceDiscoveryCritical = new Mutex( );

    mTaskListMutex->init( );
    mDeviceListMutex->init( );
    mServiceListMutex->init( );
	mDeviceDiscoveryCritical->init( );
    
    // Start discovery thread
    mMainFunctor = Base::Thread::bind( &BluetoothBluez::threadStub, this );
    mDiscThread->start( mMainFunctor );

    // Get first bluetooth device
    mDevID = hci_get_route( NULL );
    if ( mDevID < 0 )
    {
        LOGBT( "Failed to retreive bluetooth device" );
    }
}


/**
 * Destructor, deletes allocated resources
 *
 */
BluetoothBluez::~BluetoothBluez ( void )
{
    // Signal thread to quit and wait for it
    mThreadQuit = true;
    mDiscThread->join( );

    // Delete all the objects in the list, if any
    {
        Lock lck( mServiceListMutex );
        while ( mServiceList.empty( ) == false )
        {
            delete mServiceList.front( );
            mServiceList.pop_front( );
        }
    }

    {
        Lock lck( mDeviceListMutex );
        while ( mDeviceList.empty( ) == false )
        {
            delete mDeviceList.front( );
            mDeviceList.pop_front( );
        }
    }

    // Delete any remaining task
    {
        Lock lck( mTaskListMutex );
        while ( mTaskList.empty( ) == false )
        {
            delete mTaskList.front( );
            mTaskList.pop_front( );
        }
    }

    // Delete mutexes and thread
    delete mDiscThread;
    delete mTaskListMutex;
    delete mDeviceListMutex;
    delete mServiceListMutex;
	delete mDeviceDiscoveryCritical;

    // Delete the main functor
    delete mMainFunctor;
}


/**
 * Starts a device discovery
 *
 * @param cb    The callback to invoke onces that the discovery
 *              has finished.
 * @param n 	Attempt to discover device names
 *
 * @return 0 on success, \< 0 on failure.
 */
int BluetoothBluez::startDiscovery ( MABtCallback cb,
		 							 bool n )
{
	MYASSERT( mWorkState == IDLE, MoSyncError::BTERR_DISCOVERY_IN_PROGRESS );

    mState     = 0;
    mWorkState = DEVICE_DISC;

    // Check if there's a bluetooth device
    if ( mDevID < 0 )
    {
        LOGBT( "Invalid interface ID - No BT device?" );
        mWorkState = IDLE;
        return -1;
    }

    // Clear old devices in the list
    {
        Lock lock( mDeviceListMutex );

        while ( mDeviceList.empty( ) == false )
        {
            delete mDeviceList.front( );
            mDeviceList.pop_front( );
        }
    }

    // Start task
    {
        Lock lock( mTaskListMutex );
        mTaskList.push_back( bind( &BluetoothBluez::discoverDevices,
                                   this,
                                   (MABtCallback)cb,
									n ) );
    }

	return 0;
}


/**
 * Returns the next discovered device.
 *
 * @param d Pointer to the structure to fill.
 *
 * @return 1 if there was a device, 0 if not
 */
int BluetoothBluez::getNextDevice ( MABtDevice *d )
{
    CBtDevice *pDevice;
    Lock lock( mDeviceListMutex );

    // Check if list is empty
    if ( mDeviceList.empty( ) == true )
        return 0;

    pDevice = mDeviceList.front( );
    mDeviceList.pop_front( );
    pDevice->getInfo( d );
    delete pDevice;

    return 1;
}

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
int BluetoothBluez::startServiceDiscovery ( MABtCallback cb,
                                            const MABtAddr* a,
                                            const MAUUID* u )
{
	MYASSERT( mWorkState == IDLE, MoSyncError::BTERR_DISCOVERY_IN_PROGRESS );

    mState     = 0;
    mWorkState = SERVICE_DISC;

    // Check if there's a bluetooth device
    if ( mDevID < 0 )
    {
        LOGBT( "Invalid interface ID" );
        mWorkState = IDLE;
        return -1;
    }

    // Clear old service in list
    {
        Lock lck( mServiceListMutex );
        while ( mServiceList.empty( ) == false )
        {
            delete mServiceList.front( );
            mServiceList.pop_front( );
        }
    }

    // Start task
    {
        Lock lock( mTaskListMutex );
        mTaskList.push_back( bind( &BluetoothBluez::discoverServices,
                                    this,
                                    (MABtCallback)cb,
                                    *a,
                                    *u ) );
    }
	
	return 0;
}

/**
 * Returns and removes the next newly found service.
 *
 * @param d     Pointer to the structure to fill
 *
 * @return 1 if there was a service, 0 if not
 */
int  BluetoothBluez::getNextService ( MABtService* d )
{
    CBtService *s;
    Lock lck( mServiceListMutex );

    if ( mServiceList.empty( ) == true )
        return 0;

    s = mServiceList.front( );
    mServiceList.pop_front( );
    s->getInfo( d );
    delete s;

    return 1;
}

/**
 * Returns the number of UUIDs the next service
 * has.
 *
 * @param d     Pointer to the structure to fill.
 *
 * @return 1 If there was a service. 0 if not.
 */
int BluetoothBluez::getNextServiceSize ( MABtServiceSize* d )
{
    CBtService *s;
    Lock lck( mServiceListMutex );

    if ( mServiceList.empty( ) == true )
        return 0;

    s = mServiceList.front( );
    d->nUuids      = s->getUUIDCount( );
    d->nameBufSize = s->getNameLength( );

    return 1;
}


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
int BluetoothBluez::cancelDeviceDiscovery ( void )
{
	Lock lck( mDeviceDiscoveryCritical );

	if ( mWorkState != DEVICE_DISC )
		return 0;

	mWorkState = DEVICE_DISC_CANCELED;
	return 1;
}


/**
 * Returns the current state
 *
 */
int BluetoothBluez::getState ( void )
{
    return mState;
}


/**
 * Returns the local bluetooth interface adress
 *
 * @param a    Structure to fill with adress
 *
 * @return < 0    Operation failed.
 */
int BluetoothBluez::getLocalAddress ( MABtAddr& a )
{
    int             sock;
    hci_dev_info    devinf;

    // Check if there's a bluetooth device
    if ( mDevID < 0 )
    {
        LOGBT( "Invalid interface ID" );
        return CONNERR_INTERNAL;
    }

    // Open a socket for operation
    sock = hci_open_dev( mDevID );
    if ( sock < 0 )
    {
        LOGBT( "Failed to open sock" );
        return CONNERR_INTERNAL;
    }
    DescriptorCloser sockCloser( sock );

    // Get interface info
    if ( hci_devinfo( mDevID, &devinf ) < 0 )
    {
        LOGBT( "Failed to get interface info" );
        return CONNERR_INTERNAL;
    }

    // Convert to MoSync byte order
    ba2ma( a.a, devinf.bdaddr.b );
    return 1;
}

/**
 * This method is used for discovering new devices
 *
 * @param cb    The callback to invoke onces that the discovery
 *              has finished.
 * @param n 	Attempt to discover device names
 */
void BluetoothBluez::discoverDevices ( MABtCallback cb,
                                       bool n )
{
    int             i;
    int             numRsp;
    int             devSock;
    MABtAddr        btaddr;
    inquiry_info*   pQueryInfo;
    int             maxRsp = 255;
    char            devName[256] = { 0 };

    mState = 0;

    // Create a socket
    devSock = hci_open_dev( mDevID );
    if ( devSock < 0 )
    {
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        LOGBT( "Failed to open BT device" );
        cb( );
        return;
    }
    DescriptorCloser sockCloser( devSock );

    // Allocate memory for the query and perform it
    pQueryInfo = new inquiry_info[maxRsp];
    if ( pQueryInfo == NULL )
    {
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        cb( );
        return;
    }

	// Check if discovery has been canceled
	if ( mWorkState == DEVICE_DISC_CANCELED )
	{
        delete[] pQueryInfo;
		doCleanupDiscoverDevices( );
        cb( );
        return;
	}

    // Do a standard 10 sec discovery
    numRsp = hci_inquiry( mDevID, 8, maxRsp, NULL, &pQueryInfo, IREQ_CACHE_FLUSH );
    if ( numRsp < 0 )
    {
        delete[] pQueryInfo;
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        LOGBT( "Inquiry failed" );
        cb( );
        return;
    }

    // Get device info
    {
        Lock lock( mDeviceListMutex );

        // Get device name and insert into list
        for ( i = 0; i < numRsp; i++ )
        {
			// Check if discovery has been canceled
			if ( mWorkState == DEVICE_DISC_CANCELED )
				break;

            devName[0] = NULL;

			// Perform a name query
			if ( n == true )
			{            
		        if ( hci_read_remote_name( devSock, &pQueryInfo[i].bdaddr,
		                                   sizeof( devName ), devName, NULL ) < 0 )
		        {
		            LOGBT( "Device name inquiry failed" );
		        }
			}

            ba2ma( btaddr.a, pQueryInfo[i].bdaddr.b );
            mDeviceList.push_back( new CBtDevice( &btaddr, devName ) );
        }
    }
    delete[] pQueryInfo;


    // Invoke the callback to send the event to the user
	{
		Lock lck( mDeviceDiscoveryCritical );

		if ( mWorkState == DEVICE_DISC_CANCELED )
			doCleanupDiscoverDevices( );
		else
			mState = mDeviceList.size( )+1;

		mWorkState = IDLE;
		cb( );
	}
}


/**
 * This method is to clean up after a canceled discovery. After
 * it has run, it will change the work state to IDLE.
 *
 */
void BluetoothBluez::doCleanupDiscoverDevices ( void )
{
    Lock lck( mServiceListMutex );

	mWorkState = IDLE;
	mState = CONNERR_CANCELED;
    LOGBT( "On going discovery canceled" );

    if ( mServiceList.empty( ) == true )
        return;

	while ( mServiceList.empty( ) == true )
	{
		delete mServiceList.front( ); 
		mServiceList.pop_front( );
	}

}

/**
 * This method is used for performing a service discovery.
 *
 * @param cb    The callback to invoke onces that the discovery
 *              has finished.
 * @param a     Address of the device to do discovery on.
 * @param u     The UUID to search for.
 */
void BluetoothBluez::discoverServices ( MABtCallback cb, MABtAddr a, MAUUID u )
{
    int             errCode;
    MAUUID          mauuid;
    uuid_t*         uuid128;
    
    bdaddr_t        devAddr;
    uuid_t          servUuid;
    CBtService*     service;
    char            serviceName[256];
    sdp_list_t*     respList   = NULL;
    sdp_list_t*     searchList = NULL;
    sdp_list_t*     attridList = NULL;
    sdp_session_t*  session    = NULL;


    mState = 0;

    // Convert address and service uuid to BlueZ byte order
    ma2ba( devAddr.b, a.a );
    mauuid_to_uuid128( servUuid, u );

    // connect to the SDP server running on the remote machine
    bdaddr_t addrAny = {{0, 0, 0, 0, 0, 0}};//*BDADDR_ANY;
    session = sdp_connect( &addrAny, &devAddr, SDP_RETRY_IF_BUSY );
    if ( session == NULL )
    {
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        LOGBT( "Could not connect to SDP server\n" );
        cb( );
        return;
    }

    searchList = sdp_list_append( NULL, &servUuid );
    if ( searchList == NULL )
    {
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        LOGBT( "Unable to create search list, out of memory?" );
        sdp_close( session );
        cb( );
        return;
    }

    // specify that we want a list of all the matching
    // applications attributes
    uint32_t range = 0x0000ffff;
    attridList  = sdp_list_append( NULL, &range );
    if ( searchList == NULL )
    {
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        sdp_close( session );
        sdp_list_free( searchList, 0 );
        LOGBT( "Unable to create attrid list, out of memory?" );
        cb( );
        return;
    }

    // get a list of service records that have the requested UUID
    errCode = sdp_service_search_attr_req( session, searchList,
                                           SDP_ATTR_REQ_RANGE, attridList,
                                           &respList );
    if ( errCode == -1 )
    {
        mWorkState = IDLE;
        mState = CONNERR_INTERNAL;
        sdp_close( session );
        sdp_list_free( searchList, 0 );
        sdp_list_free( attridList, 0 );
        LOGBT( "Search timed out" );
        cb( );
        return;
    }

    sdp_list_free( searchList, 0 );
    sdp_list_free( attridList, 0 );


    //
    // Go through each of the service records
    //
    {
        Lock lck( mServiceListMutex );

        for ( sdp_list_t *r = respList; r != NULL; r = r->next )
        {
            sdp_list_t*   protoList;
            sdp_record_t* record = (sdp_record_t*)r->data;

            // get a list of the protocol sequences
            if ( sdp_get_access_protos( record, &protoList ) == 0 )
            {
                sdp_get_service_name( record, serviceName, 256 );
                service = new CBtService( serviceName );

                // go through each protocol sequence
                for ( sdp_list_t *p = protoList; p != NULL; p = p->next )
                {
                    // go through each protocol list of the protocol sequence
                    for ( sdp_list_t *pds = (sdp_list_t*)p->data; pds; pds = pds->next )
                    {
                        // check the protocol attributes
                        for ( sdp_data_t *d = (sdp_data_t*)pds->data; d != NULL; d = d->next )
                        {
                            //
                            // Tuples
                            //  - {RFCOMM, CHAN}
                            //
                            switch( d->dtd )
                            {
                                case SDP_UUID16:
                                case SDP_UUID32:
                                case SDP_UUID128:                                    
                                    // Add the service UUID
                                    uuid128 = sdp_uuid_to_uuid128( &d->val.uuid );
                                    if ( uuid128 == NULL )
                                    {
                                        mWorkState = IDLE;
                                        mState = CONNERR_INTERNAL;
                                        sdp_close( session );
                                        LOGBT( "Out of memory" );
                                        cb( );
                                        return;
                                    }
                                    uuid128_to_mauuid( mauuid, *uuid128 );
                                    bt_free( uuid128 );
                                    service->addUUID( mauuid );

                                    // Check if it's a RFCOMM
                                    if ( sdp_uuid_to_proto( &d->val.uuid ) == RFCOMM_UUID )
                                    {
                                        // This actually an error, should never happen
                                        if ( d->next == NULL || d->next->dtd != SDP_UINT8 )
                                            continue;

                                        d = d->next;
                                        service->setPort( d->val.int8 );
                                    }
                                    break;
                            }
                        }
                    }

                    sdp_list_free( (sdp_list_t*)p->data, 0 );
                }
                
                sdp_list_free( protoList, 0 );
                mServiceList.push_back( service );
            }

            sdp_record_free( record );
        }
    }

    sdp_close( session );
    
    // Invoke the callback to send the event to the user
    mWorkState = IDLE;
    mState     = mServiceList.size( )+1;
    cb( );
}


/**
 * Thread stub, runs tasks until it has been signaled to quit.
 *
 */
void BluetoothBluez::threadStub ( void )
{
    Functor* task;

    while ( true )
    {
        // Check for quit signal
        if ( mThreadQuit == true )
            break;

        // Attempt to fetch a task
        task = NULL;
        {            
            Lock lck( mTaskListMutex );

            if ( mTaskList.empty( ) == false )
            {
                task = mTaskList.front( );
                mTaskList.pop_front( );
            }
        }

        // Perform the task
        if ( task != NULL )
        {
            (*task)( );
            delete task;         
        }

        // Sleep for a while
        Thread::sleep( 100 );
    }
}

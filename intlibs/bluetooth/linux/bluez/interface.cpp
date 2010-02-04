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
 * File:   interface.cpp
 * Author: Ali Mosavian
 *
 * Created on July 29, 2009
 */
#include <cstdlib>
#include <bluetooth/bluetooth.h>

#include "../../config_bluetooth.h"
#include "../../btinit.h"
#include "../../discovery.h"
#include "bluetoothbluez.hpp"
#include "helpers/helpers.h"





using namespace Bluetooth;

/**
 * Globals
 */
static BlueZ::BluetoothBluez* gInstance = NULL;


namespace Bluetooth
{
    BluetoothStack gBluetoothStack = BTSTACK_BLUEZ;


/**
 * Will initialize the bluetooth system
 *  
 */
void MABtInit ( void )
{
    MAASSERT( gInstance == NULL );
    gInstance = new BlueZ::BluetoothBluez( );
}

/**
 * Will close the bluetooth system
 *
 */
void MABtClose ( void )
{
    if ( gInstance == NULL )
    {
        LOGBT( "gInstance is NULL" );
        return;
    }

    delete gInstance;
}


/**
 * Returns the adress of the bluetooth interface
 *
 * @param a     Structure to fill with interface address
 *
 * @return < 0    On failure.
 */
int getLocalAddress ( MABtAddr& a )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getLocalAddress( a );
}


/**
 * Returns the device discovery state.
 *
 * @return	0 - Still working
 * 		1 - Finished successfully
 *            < 0 - (CONNERR) Failed.
 */
int maBtDiscoveryState ( void )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getState( );
}

int maBtCancelDiscovery ( void )
{
    DEBIG_PHAT_ERROR;
}

/**
 * Start a new device discovery operation in the background
 * and invokes the callback once it has finished.
 *
 * Note: Only one discovery operation can be active at
 * 	 a time.
 *
 * @param cb	- The callback which will be invoked once the
 *                discovery operation has finished.
 * @param n 	- Attempt to discover device names
 */
int maBtStartDeviceDiscovery ( MABtCallback cb,
                               bool n )
{
    MAASSERT( gInstance != NULL );
    return gInstance->startDiscovery( cb, n );
}

/**
 * Returns the next discovered device.
 *
 * @param d Pointer to the structure to fill.
 *
 * @return 1 if there was a device, 0 if not
 */
int maBtGetNewDevice ( MABtDevice* d )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getNextDevice( d );
}


/**
 * Starts a new service discovery operation given a device
 * adress and a family of services to search for.
 *
 * @param a    The address of the device to do service discovery on.
 * @param u    The service family UUID to search for.
 * @param cb   The callback to invoke once discovery has finished.
 */
int maBtStartServiceDiscovery ( const MABtAddr* a,
                                const MAUUID* u,
                                MABtCallback cb )
{
    MAASSERT( gInstance != NULL );
    return gInstance->startServiceDiscovery(cb, a, u );
}

/**
 * Returns the next service, can be called once a service
 * discovery has finished.
 *
 * @param d   Pointer to structure to fill
 *
 * @return > 0 If there was any more services to get
 */
int maBtGetNewService ( MABtService* d )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getNextService( d );
}


/**
 * Returns the size of the next service, of any.
 *
 * @param d     Pointer to the structure to fill.
 *
 * @return > 0 if there was a service, 0 if not
 */
int maBtGetNextServiceSize ( MABtServiceSize* d )
{
    MAASSERT( gInstance != NULL );
    return gInstance->getNextServiceSize( d );
}

}

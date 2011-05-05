/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file Common.h
 * @author Mikael Kindborg
 *
 * Description:
 *
 *  This file contains variables and functions that
 *  are shared between the BluetoothServer project and
 *  the BluetoothClient project.
 *
 *  These projects demonstrate a how to write a Bluetooth
 *  server and client application. You need to build both
 *  projects and deploy to two devices to use this code
 *  example.
 *
 *  When making your own projects that use Bluetooth,
 *  note that you need to enable Bluetooth in
 *  Project/Properties/MoSync Project/Application Permissions.
 *
 * How to use:
 *
 *  You need two Bluetooth capable devices. Make sure that
 *  Bluetooth is turned on and that the devices are paired
 *  before running the server and the client applications.
 *
 *  Build and run the server app:
 *   - Build and run project BluetoothServer.
 *   - Deploy and launch the server app on the first device.
 *   - Note the Bluetooth device address that is displayed,
 *     it is to entered as the value for the variable
 *     sServerAddress in Client.cpp.
 *
 *  Build and run the client app:
 *   - Edit the value of the variable sServerAddress to be
 *     the address of the server in file Client.cpp in
 *     project BluetoothClient. (If you have not entered
 *     this address, a panic message is shown, and the
 *     app closes).
 *   - Build and run the project BluetoothClient.
 *   - Deploy and launch the client on the second device.
 *
 * How to verify that it works:
 *
 *  In the client app, press keys on key pad or touch the screen,
 *  this sends data to the server app. The data should be displayed
 *  on the server screen.
 */

#ifndef COMMON_H
#define COMMON_H

/**
 * Size of buffer used for writing (server) and reading (client).
 */
static const int sBufferSize = 64;

/**
 * This is the service UUID string, shared between client and server.
 */
static const char* sServiceUUID = "102030405060708090A0B0C0D0E0F010";

/**
 * Helper function that prints a divider to increase output readability.
 */
static void printDivider()
{
	printf("-----------------------------------------\n");
}

#endif // COMMON_H

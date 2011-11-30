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
 * @file ScreenListeners.h
 * @author Bogdan Iusco
 *
 * Listeners for DisplayNotificationScreen and SettingsScreen.
 */

#ifndef SCREENLISTENERS_H_
#define SCREENLISTENERS_H_

#include <MAUtil/String.h>

/**
 * @brief Listener for SettingsScreen.
 */
class SettingsScreenListener
{
public:
	/**
	 * Called when connect button is pressed.
	 * @param ipAddress Server's ip address written by the user.
	 * @param port Server's port written by the user.
	 */
	virtual void connectToServer(const MAUtil::String& ipAddress,
			const MAUtil::String& port) = 0;
};

/**
 * @brief Listener for TCP connection.
 */
class TCPListener
{
public:
    /**
     * Called when the application is connected to the server.
     */
	virtual void connectionEstablished() = 0;
};

#endif /* SCREENLISTENERS_H_ */

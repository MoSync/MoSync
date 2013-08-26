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

/**
 * @brief Listener for HTTP Connection.
 */
class HTTPListener
{
public:
	/**
	 * Called when the server has received the authorization key.
	 */
	virtual void messageSent() = 0;
};

#endif /* SCREENLISTENERS_H_ */

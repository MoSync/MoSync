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
 * @file PhoneGapFile.h
 * @author Mikael Kindborg
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#ifndef PHONEGAP_FILE_H_
#define PHONEGAP_FILE_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include "PhoneGapMessage.h"

class PhoneGapMessageHandler;

/**
 * Class that implements PhoneGap File API.
 */
class PhoneGapFile
{
public:
	/**
	 * Constructor.
	 */
	PhoneGapFile(PhoneGapMessageHandler* messageHandler);

	/**
	 * Destructor.
	 */
	virtual ~PhoneGapFile();

	/**
	 * Implementation of File API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	bool handleMessage(PhoneGapMessage& message);

private:
	PhoneGapMessageHandler* mMessageHandler;
};

#endif

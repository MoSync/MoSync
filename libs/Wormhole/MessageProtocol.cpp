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
 * @file MessageProtocol.cpp
 * @author Mikael Kindborg
 *
 * Class for reading the protocol of a message from a WebView.
 */

#include <ma.h>				// MoSync API
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions
#include <conprint.h>

#include "MessageProtocol.h"

namespace Wormhole
{
	MessageProtocol::MessageProtocol(MAHandle dataHandle)
	{
		// Get the protocol.
		maReadData(dataHandle, mProtocol, 0, 3);
	}

	MessageProtocol::~MessageProtocol()
	{
	}

	bool MessageProtocol::matches(const char* protocol)
	{
		return
			(mProtocol[0] == protocol[0]) &&
			(mProtocol[1] == protocol[1]) &&
			(mProtocol[2] == protocol[2]);
	}

	bool MessageProtocol::isMessageStream()
	{
		return
			(mProtocol[0] == 'm') &&
			(mProtocol[1] == 's') &&
			(mProtocol[2] == ':');
	}

	bool MessageProtocol::isMessageArrayJSON()
	{
		return
			(mProtocol[0] == 'm') &&
			(mProtocol[1] == 'a') &&
			(mProtocol[2] == ':');
	}
} // namespace

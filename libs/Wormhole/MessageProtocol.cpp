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

	bool MessageProtocol::isMessageStreamJSON()
	{
		return
			(mProtocol[0] == 'm') &&
			(mProtocol[1] == 'a') &&
			(mProtocol[2] == ':');
	}
} // namespace

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
 * @file PhoneGapFile.cpp
 * @author Mikael Kindborg
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#include <conprint.h>
#include <MAUtil/String.h>
#include "PhoneGapFile.h"
#include "PhoneGapMessageHandler.h"

using namespace MAUtil;

// Error codes.
#define NOT_FOUND_ERR 1
#define SECURITY_ERR 2
#define ABORT_ERR 3
#define NOT_READABLE_ERR 4
#define ENCODING_ERR 5
#define NO_MODIFICATION_ALLOWED_ERR 6
#define INVALID_STATE_ERR 7
#define SYNTAX_ERR 8
#define INVALID_MODIFICATION_ERR 9
#define QUOTA_EXCEEDED_ERR 10
#define TYPE_MISMATCH_ERR 11
#define PATH_EXISTS_ERR 12

// File system options.
#define LOCALFILESYSTEM_TEMPORARY 0
#define LOCALFILESYSTEM_PERSISTENT 1
#define LOCALFILESYSTEM_RESOURCE 2
#define LOCALFILESYSTEM_APPLICATION 3

/**
 * Constructor.
 */
PhoneGapFile::PhoneGapFile(PhoneGapMessageHandler* messageHandler)
	: mMessageHandler(messageHandler)
{
}

/**
 * Destructor.
 */
PhoneGapFile::~PhoneGapFile()
{
}

String PhoneGapFile::emitFileSystemInfo(String& name, String& rootEntry)
{
	return "{\"name\"=" + name + ", \"root\"=" + rootEntry + "}";
}

String PhoneGapFile::emitFileEntry(
	String& isFile, String& isDirectory, String& name, String& fullPath)
{
	return
		"{\"isFile\"=" + isFile + ", " +
		"\"isDirectory\"=" + isDirectory + ", " +
		"\"name\"=" + name + ", " +
		"\"fullPath\"=" + fullPath + "}";
}

void PhoneGapFile::passResult()
{

}

/**
 * Implementation of File API exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
bool PhoneGapFile::handleMessage(PhoneGapMessage& message)
{
	if (message.getParam("action") == "requestFileSystem")
	{
		lprintfln("@@@ requestFileSystem\n");
		// We ignore type and size parameters and use persistent storage.
		String rootEntry = emitFileEntry(true, true, "sdcard", "/sdcard/");
		String fileSystemInfo = emitFileSystemInfo("persistent", rootEntry);

	}
	else if (message.getParam("action") == "testFileExists")
	{
		String fileName = message.getParam("fileName");
		lprintfln("testFileExists: %s\n", fileName.c_str());
	}

	return true;
}

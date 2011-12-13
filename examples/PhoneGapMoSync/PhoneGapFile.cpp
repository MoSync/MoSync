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

// File error codes.
#define FILEERROR_NOT_FOUND_ERR "1"
#define FILEERROR_SECURITY_ERR "2"
#define FILEERROR_ABORT_ERR "3"
#define FILEERROR_NOT_READABLE_ERR "4"
#define FILEERROR_ENCODING_ERR "5"
#define FILEERROR_NO_MODIFICATION_ALLOWED_ERR "6"
#define FILEERROR_INVALID_STATE_ERR "7"
#define FILEERROR_SYNTAX_ERR "8"
#define FILEERROR_INVALID_MODIFICATION_ERR "9"
#define FILEERROR_QUOTA_EXCEEDED_ERR "10"
#define FILEERROR_TYPE_MISMATCH_ERR "11"
#define FILEERROR_PATH_EXISTS_ERR "12"

// FileTransfer error codes.
#define FILETRANSERERROR_FILE_NOT_FOUND_ERR "1"
#define FILETRANSERERROR_INVALID_URL_ERR "2"
#define FILETRANSERERROR_CONNECTION_ERR "3"

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

String PhoneGapFile::emitFileSystemInfo(
	const String& name,
	const String& rootEntry)
{
	return "{'name':'" + name + "', 'root':" + rootEntry + "}";
}

String PhoneGapFile::emitFileEntry(
	const String& isFile,
	const String& isDirectory,
	const String& name,
	const String& fullPath)
{
	return
		"{'isFile':" + isFile + "," +
		"'isDirectory':" + isDirectory + "," +
		"'name':'" + name + "'," +
		"'fullPath':'" + fullPath + "'}";
}

void PhoneGapFile::callSuccess(
	const String& callbackID,
	const String& args,
	const String& castFunction
	)
{
	mMessageHandler->callSuccess(
		callbackID,
		PHONEGAP_CALLBACK_STATUS_OK,
		args,
		false,
		castFunction);
}

void PhoneGapFile::callError(
	const String& callbackID,
	const String& errorCode
	)
{
	String args = "{'code':" + errorCode + "}";

	mMessageHandler->callError(
		callbackID,
		args);
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

		String callbackID = message.getParam("PhoneGapCallBackId");

		// We support only persistent storage.
		int type = message.getArgsFieldInt("type");
		if (LOCALFILESYSTEM_PERSISTENT != type)
		{
			callError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return true;
		}

		// Size parameter must be zero.
		int size = message.getArgsFieldInt("size");
		if (0 != size)
		{
			callError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return true;
		}

		String rootEntry = emitFileEntry("true", "true", "/", "/");
		String fileSystemInfo = emitFileSystemInfo("persistent", rootEntry);
		callSuccess(
			callbackID,
			fileSystemInfo,
			"window.localFileSystem._castFS");

	}
	else if (message.getParam("action") == "getFile")
	{
		lprintfln("@@@ getFile\n");

		String callbackID = message.getParam("PhoneGapCallBackId");

		// We support only persistent storage.
		String fullPath = message.getArgsField("fullPath");
		String path = message.getArgsField("path");
		String options = message.getArgsField("options");

		lprintfln("@@@ fullPath: %s path: %s options: %s",
			fullPath.c_str(),
			path.c_str(),
			options.c_str());
	}

	return true;
}

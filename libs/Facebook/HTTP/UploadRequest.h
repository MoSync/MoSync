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

/*
 * UploadRequest.h
 */

#ifndef UPLOADREQUEST_H_
#define UPLOADREQUEST_H_


#include <MAUtil/String.h>
#include "FacebookRequest.h"

class UploadRequest : public FacebookRequest
{
public:
	UploadRequest (	const String& path,
					String& accessToken,
					const byte *fileRawData,
					int fileSize,
					const MAUtil::String &contentType,
					const MAUtil::String &fileName,
					const MAUtil::String &message);

	const byte *getFileRawData()const;
	int getFileRawDataSize() const;
	const MAUtil::String &getContentType() const;
	const MAUtil::String &getFileName() const;


protected:
	const byte			*mFileRawData;
	int					mFileSize;
	MAUtil::String		mContentType;
	MAUtil::String		mFileName;
};

#endif /* UPLOADREQUEST_H_ */

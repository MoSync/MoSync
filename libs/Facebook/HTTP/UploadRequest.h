/*
 * UploadRequest.h
 *
 *  Created on: Aug 12, 2011
 *      Author: gabi
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
	const byte 			*mFileRawData;
	int 			mFileSize;
	MAUtil::String 	mContentType;
	MAUtil::String  mFileName;
};

#endif /* UPLOADREQUEST_H_ */

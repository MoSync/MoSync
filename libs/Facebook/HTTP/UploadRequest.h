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

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

#include "FileLister.h"
#include <maassert.h>

int FileLister::start(const char* dir, const char* filter, int sorting) {
	close();
	return mList = maFileListStart(dir, filter, sorting);
}

int FileLister::next(MAUtil::String& dst) {
	int len = maFileListNext(mList, NULL, 0);
	if(len <= 0)
		return len;
	dst.resize(len);
	len = maFileListNext(mList, dst.pointer(), len+1);
	MAASSERT(len > 0);
	return len;
}

void FileLister::close() {
	if(mList > 0) {
		int res = maFileListClose(mList);
		MAASSERT(res == 0);
		mList = -1;
	}
}

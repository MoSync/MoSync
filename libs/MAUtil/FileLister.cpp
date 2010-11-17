#include "FileLister.h"
#include <maassert.h>

int FileLister::start(const char* dir, const char* filter) {
	close();
	return mList = maFileListStart(dir, filter);
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

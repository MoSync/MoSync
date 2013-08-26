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

/** \file FileLister.h
* \brief Lists the contents of a device's file system.
* \author Fredrik Eldh
*/

#ifndef FILELISTER_H
#define FILELISTER_H

#include <MAUtil/String.h>
#include <maapi.h>

/**
* \brief Lists the contents of a device's file system.
* It can be reused for multiple listings.
* \see maFileListStart()
*/
class FileLister {
public:
	FileLister() : mList(-1) {}
	~FileLister() { close(); }

	/**
	* \see maFileListStart()
	* Closes the active list, if any.
	* Returns \< 0 on error.
	*/
	int start(const char* dir, const char* filter = "*", int sorting = MA_FL_SORT_NONE);

	/**
	* Writes the name of the next file in the list to \a dst.
	* \a dst's old contents are overwritten.
	* Returns the length of the name, 0 if there are no more files, or \< 0 on error.
	* On error, \a dst is not modified.
	* \see maFileListNext()
	*/
	int next(MAUtil::String& dst);

	/** \see maFileListClose() */
	void close();
private:
	MAHandle mList;
};

#endif	//FILELISTER_H

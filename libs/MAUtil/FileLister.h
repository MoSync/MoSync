/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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

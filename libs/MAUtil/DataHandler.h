/* Copyright (C) 2009 Mobile Sorcery AB

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

/** \file DataHandler.h
 * \brief Facilitates consecutive reads and writes to a MoSync data object.
 */

#include <ma.h>

#ifndef DATAHANDLER_H
#define DATAHANDLER_H

namespace MAUtil {

	/** \brief Facilitates consecutive reads and writes to a MoSync data object.
	*
	* Keeps a handle and a position.
	*/
	class DataHandler {
	public:
		/**
		* Initializes DataHandler::size and sets the position to 0.
		*/
		DataHandler(MAHandle data) : size(maGetDataSize(data)), mData(data), mPos(0) {}

		/**
		* Copies \a len bytes from the data object to memory pointed to by \a dst.
		* \returns True on success. False if the operation could not be completed
		* because there's not enough data left.
		* \see maReadData()
		*/
		bool read(void* dst, int len) {
			if(mPos + len > size)
				return false;
			if(len == 0)
				return true;
			maReadData(mData, dst, mPos, len);
			mPos += len;
			return true;
		}

		/**
		* Moves the position forward \a len bytes.
		* \returns True on success. False if the operation could not be completed
		* because the data object was too small.
		*/
		bool skip(int len) {
			if(mPos + len > size)
				return false;
			mPos += len;
			return true;
		}

		/**
		* Copies \a len bytes to the data object from memory pointed to by \a src.
		* \see maWriteData()
		*/
		void write(const void* src, int len) {
			if(len == 0)
				return;
			maWriteData(mData, src, mPos, len);
			mPos += len;
		}

		/**
		* The size, in bytes, of the data object.
		*/
		const int size;
	private:
		const MAHandle mData;
		int mPos;
	};

}

#endif	//DATAHANDLER_H

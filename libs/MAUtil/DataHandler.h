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

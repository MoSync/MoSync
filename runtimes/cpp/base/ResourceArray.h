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

#ifndef _BASE_RESOURCE_ARRAY_H_
#define _BASE_RESOURCE_ARRAY_H_

#include <helpers/array.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_RESOURCE_TYPES.h>

#include "base_errors.h"
using namespace MoSyncError;

// platform specific
#include "ResourceDefs.h"

namespace Base {

	class Label {
	public:
		Label(const char *name, int index) {
			int len = strlen(name) + 1;
			mName = new char[len];
			memcpy(mName, name, len);
			mIndex = index;
		}

		~Label() {
			delete mName;
		}

		const char* getName() {
			return mName;
		}

		int getIndex() {
			return mIndex;
		}

	private:
		char *mName;
		int mIndex;
	};

#ifdef RESOURCE_MEMORY_LIMIT
		//each platform must define some of these functions.
		//the sizes they return need not be exact.
#define DECLARE_SIZEFUNCS(R, T, D) uint size_##R(T*);
		TYPES(DECLARE_SIZEFUNCS);
#endif

#define ROOM(func) if((func) == RES_OUT_OF_MEMORY) { BIG_PHAT_ERROR(ERR_RES_OOM); }

	//Internally, "resource" and "object" are used interchangably.
	class ResourceArray {	//type and bound-safe
	public:
		ResourceArray(
#ifdef RESOURCE_MEMORY_LIMIT
			uint resMax
#endif
			) : mN(0), mRes(NULL), mTypes(NULL), 
#ifdef RESOURCE_MEMORY_LIMIT
			mResmemMax(resMax),
			mResmem(0),
#endif
			dynResSize(1),
			dynResCapacity(1), 
			dynRes(NULL)
			{}
		void init(unsigned nRes) {  //saves all the old objects
			unsigned oldN = mN;
			mN = MAX(nRes + 1, oldN);
			void** oldRes = mRes;
			byte* oldTypes = mTypes;
			mRes = new void*[mN];
			MYASSERT(mRes != NULL, ERR_OOM);
			mTypes = new byte[mN];
			MYASSERT(mTypes != NULL, ERR_OOM);
			if(oldRes) {
				memcpy(mRes, oldRes, oldN*sizeof(void*));
				memcpy(mTypes, oldTypes, oldN*sizeof(byte));
				delete[] oldRes;
				delete[] oldTypes;
			}
			if(mN > oldN) { //clear the new objects
				//pointer arithmetic bug
				//memset(mRes + oldN*sizeof(void*), 0, (mN - oldN)*sizeof(void*));
				memset(&mRes[oldN], 0, (mN - oldN) * sizeof(void*));

				memset(mTypes + oldN, RT_PLACEHOLDER, (mN - oldN));
			}
		}

#define DYNAMIC_PLACEHOLDER_BIT 0x40000000

		~ResourceArray() {
			for(unsigned i=1; i<mN; i++) {
				LOGD("RA %i\n", i);
				_destroy(i);
			}
			delete[] mRes;
			delete[] mTypes;
			
			for(unsigned i=1; i<dynResSize; i++) {
				LOGD("DA %i\n", i);
				_destroy(i | DYNAMIC_PLACEHOLDER_BIT);
			}
			if(dynRes) {
				delete[] dynRes;
				delete[] dynResTypes;
			}
		}

#define DEFINE_ADD(R, T, D) int add_##R(unsigned index, T* o) { return _add(index, o, R); }\
	int dadd_##R(unsigned index, T* o) { return _dadd(index, o, R); }
#define DEFINE_GET(R, T, D) T* get_##R(unsigned index) { return (T*)_get(index, R); }
#define DEFINE_EXTRACT(R, T, D) T* extract_##R(unsigned index) { return (T*)_extract(index, R); }
		TYPES(DEFINE_ADD);
		TYPES(DEFINE_GET);
		TYPES(DEFINE_EXTRACT);

#define TESTINDEX(index, size) { MYASSERT(size>1, ERR_RES_NO_RESOURCES); if((index) >= size || (index) == 0 ) {\
	LOG("Bad resource index: %i. size=%i.\n", index, size);\
	BIG_PHAT_ERROR(ERR_RES_INVALID_INDEX); } }

		void destroy(unsigned index) {
			_destroy(index);
		}

		byte get_type(unsigned index) {
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				index&=~DYNAMIC_PLACEHOLDER_BIT;
				TESTINDEX(index, dynResSize);
				return dynResTypes[index];
			} else {
				TESTINDEX(index, mN);
				return mTypes[index];
			}
		}

		unsigned size() {
			return mN;
		}

		// used for maCreatePlaceholder
		unsigned int create_RT_PLACEHOLDER() {
			if(dynResSize+1>dynResCapacity) {
				void ** oldPlaceholders = dynRes;
				byte *oldPlaceholderTypes = dynResTypes;
				dynRes = new void*[(dynResCapacity)*2];
				MYASSERT(dynRes != NULL, ERR_OOM);
				dynResTypes = new byte[(dynResCapacity)*2];
				MYASSERT(dynResTypes != NULL, ERR_OOM);
				memset(dynRes, 0, sizeof(void*)*(dynResCapacity*2));

				if(oldPlaceholders != NULL) {
					memcpy(dynRes, oldPlaceholders, dynResCapacity*sizeof(void*));
					memcpy(dynResTypes, oldPlaceholderTypes, dynResCapacity*sizeof(byte));

					delete []oldPlaceholders;
					delete []oldPlaceholderTypes;
				}

				dynResCapacity = (dynResCapacity)*2;
			}

			dynResTypes[dynResSize] = RT_PLACEHOLDER;
			dynResSize++;
			return (dynResSize-1)|DYNAMIC_PLACEHOLDER_BIT;
		}

#ifdef RESOURCE_MEMORY_LIMIT
		uint getResmemMax() const { return mResmemMax; }
		uint getResmem() const { return mResmem; }
#endif
		
		void logEverything() {
#define RESOURCE_STRINGS(R, T, D) #R,
			
			const char *resourceStrings[] = {
				TYPES(RESOURCE_STRINGS)
			};
			
			LOG("Num static resources: %d\n", mN);
			LOG("Num dynamic resources: %d\n", dynResSize);
			for(int i = 0; i < mN; i++) {
				byte type = mTypes[i];
				LOG("Static resource %d is of type %s\n", i, resourceStrings[type]);
			}
			
			for(int i = 0; i < dynResSize; i++) {
				byte type = dynResTypes[i];
				LOG("Dynamic resource %d is of type %s\n", i, resourceStrings[type]);
			}
			
			
		}

	private:

		//**************************************************************************
		// size calculation functions
		//**************************************************************************

		int _dadd(unsigned index, void* o, byte type) {
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				unsigned pIndex = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(pIndex, dynResSize);
				if(dynRes[pIndex] != NULL) {
					_destroy(index);
				}
				return _add(index, o, type);
			} else {
				TESTINDEX(index, mN);
				if(mRes[index] != NULL) {
					_destroy(index);
				}
				return _add(index, o, type);
			}
		}

		int _add(unsigned index, void* o, byte type) {
			void **res = mRes;
			byte *types = mTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = dynRes;
				types = dynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, dynResSize);
			} else {
				TESTINDEX(index, mN);
			}

			if(o == NULL && (type != RT_PLACEHOLDER && type != RT_FLUX)) {
#ifdef _android
				// On Android JNI RT_IMAGE is stored on the Java side
				if(type != RT_IMAGE) DEBIG_PHAT_ERROR;
#else
				DEBIG_PHAT_ERROR;
#endif
			}

			if(res[index] != NULL || types[index] != RT_PLACEHOLDER) {
				BIG_PHAT_ERROR(ERR_RES_OVERWRITE);
			}

#ifdef RESOURCE_MEMORY_LIMIT
			int oldResmem = mResmem;
			switch(type) {
#define CASE_ADDMEM(R, T, D) case R: mResmem += size_##R((T*)o); break;
				TYPES(CASE_ADDMEM);
			}
			if(mResmem >= mResmemMax) {
				//BIG_PHAT_ERROR(ERR_RES_OOM);
				mResmem = oldResmem;
				__destroy(o, type, index);	//avoids memory leaks
				return RES_OUT_OF_MEMORY;
			}
#endif	//RESOURCE_MEMORY_LIMIT
			res[index] = o;
			types[index] = type;
			return RES_OK;
		}

		void* _get(unsigned index, byte R) {
			void **res = mRes;
			byte *types = mTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = dynRes;
				types = dynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, dynResSize);
			} else {
				TESTINDEX(index, mN);
			}			

			if(types[index] != R) {
				BIG_PHAT_ERROR(ERR_RES_INVALID_TYPE);
			}
			return res[index];
		}

		void* _extract(unsigned index, byte R) {
			void **res = mRes;
			byte *types = mTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = dynRes;
				types = dynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, dynResSize);
			} else {
				TESTINDEX(index, mN);
			}			

			if(types[index] != R) {
				BIG_PHAT_ERROR(ERR_RES_INVALID_TYPE);
			}

#ifdef RESOURCE_MEMORY_LIMIT
			switch(types[index]) {
#define CASE_SUBMEM(R, T, D) case R: mResmem -= size_##R((T*)res[index]); break;
				TYPES(CASE_SUBMEM);
			}
			if(mResmem > mResmemMax) {
				DEBIG_PHAT_ERROR;
			}
#endif	//RESOURCE_MEMORY_LIMIT

			void* temp = res[index];
			res[index] = NULL;
			types[index] = RT_PLACEHOLDER;
			return temp;
		}

		void __destroy(void* o, byte type, unsigned index) {
			switch(type) {
#define CASE_DELETE(R, T, D) case R: D ((T*)o);\
	LOGD("%s: %s %s %i\n", #R, #D, #T, index); break;
				TYPES(CASE_DELETE);
			}
		}

		void _destroy(unsigned index) {
			void **res = mRes;
			byte *types = mTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = dynRes;
				types = dynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, dynResSize);
			} else {
				TESTINDEX(index, mN);
			}

			MYASSERT(types[index] != RT_FLUX, ERR_RES_DESTROY_FLUX);

#ifdef RESOURCE_MEMORY_LIMIT
			switch(types[index]) {
#define CASE_SUBMEM(R, T, D) case R: mResmem -= size_##R((T*)res[index]); break;
				TYPES(CASE_SUBMEM);
			}
			if(mResmem > mResmemMax) {
				DEBIG_PHAT_ERROR;
			}
#endif	//RESOURCE_MEMORY_LIMIT

			__destroy(res[index], types[index], index);

			res[index] = NULL;
			types[index] = RT_PLACEHOLDER;
		}

		unsigned mN;
		void** mRes;
		byte* mTypes;

#ifdef RESOURCE_MEMORY_LIMIT
		const uint mResmemMax;
		uint mResmem;
#endif

		byte*  dynResTypes;
		unsigned dynResSize;
		unsigned dynResCapacity;
		void** dynRes;
	};
}

#endif // _BASE_RESOURCE_ARRAY_H_

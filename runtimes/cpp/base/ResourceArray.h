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

// This file is platform specific and contains the TYPES
// macro. It expands by applying another macro to define
// types and delete operations on types. This file is located
// at different places in each runtime, but typically it
// should be found in the main cpp directory for the platform.
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

#define DECLARE_RESOURCE_TYPES(R, T, D) typedef T R##_Type;
    TYPES(DECLARE_RESOURCE_TYPES);

#define ROOM(func) if((func) == RES_OUT_OF_MEMORY) { BIG_PHAT_ERROR(ERR_RES_OOM); }

	//Internally, "resource" and "object" are used interchangably.
	class ResourceArray {	//type and bound-safe
	public:
		ResourceArray(
#ifdef RESOURCE_MEMORY_LIMIT
			uint resMax
#endif
			) :
#ifdef RESOURCE_MEMORY_LIMIT
			mResmemMax(resMax),
			mResmem(0),
#endif
			mResSize(0),
			mRes(NULL),
			mResTypes(NULL),
			mDynResSize(1),
			mDynResCapacity(1),
			mDynRes(NULL),
			mDynResTypes(NULL),
			mDynResPoolSize(0),
			mDynResPoolCapacity(0),
			mDynResPool(NULL)
		{
		}

		/**
		 * Initialize the static resource array.
		 * This function saves the old objects present
		 * in the array.
		 * @param numResources The number of resource in the new array.
		 */
		void init(unsigned numResources) {
			unsigned oldResSize = mResSize;
			mResSize = MAX(numResources + 1, oldResSize);
			void** oldRes = mRes;
			byte* oldTypes = mResTypes;
			mRes = new void*[mResSize];
			MYASSERT(mRes != NULL, ERR_OOM);
			mResTypes = new byte[mResSize];
			MYASSERT(mResTypes != NULL, ERR_OOM);

			if(oldRes) {
				memcpy(mRes, oldRes, oldResSize*sizeof(void*));
				memcpy(mResTypes, oldTypes, oldResSize*sizeof(byte));
				delete[] oldRes;
				delete[] oldTypes;
			}

			if(mResSize > oldResSize) {
				// Clear the new objects.
				//pointer arithmetic bug
				//memset(mRes + oldResSize*sizeof(void*), 0, (mResSize - oldResSize)*sizeof(void*));
				memset(&mRes[oldResSize], 0, (mResSize - oldResSize) * sizeof(void*));
				// Set to placeholder type.
				memset(mResTypes + oldResSize, RT_PLACEHOLDER, (mResSize - oldResSize));
			}
		}

// If this bit is set in the handle, the handle is a dynamic placeholder.
// The non-dynamic handles are the ones refering to application resources.
#define DYNAMIC_PLACEHOLDER_BIT 0x40000000

		~ResourceArray() {
			// Destroy static resources
			for(unsigned i=1; i<mResSize; ++i) {
				LOGD("RA %i\n", i);
				_destroy(i);
			}
			delete[] mRes;
			delete[] mResTypes;

			// Destroy dynamic resources.
			for(unsigned i=1; i<mDynResSize; ++i) {
				LOGD("DA %i\n", i);
				_destroy(i | DYNAMIC_PLACEHOLDER_BIT);
			}
			if(mDynRes) {
				delete[] mDynRes;
				delete[] mDynResTypes;
			}
			if(mDynResPool) {
				delete[] mDynResPool;
			}
		}

// Define functions for each resource data type, to add a resource,
// to get a resource, and to extract a resource.
// TODO: What does extract do?
#define DEFINE_ADD(R, T, D) int add_##R(unsigned index, T* obj) { return _add(index, obj, R); }\
	int dadd_##R(unsigned index, T* obj) { return _dadd(index, obj, R); }
#define DEFINE_GET(R, T, D) T* get_##R(unsigned index) { return (T*)_get(index, R); }
#define DEFINE_EXTRACT(R, T, D) T* extract_##R(unsigned index) { return (T*)_extract(index, R); }
		TYPES(DEFINE_ADD);
		TYPES(DEFINE_GET);
		TYPES(DEFINE_EXTRACT);

// Make sure the given array index is valid.
#define TESTINDEX(index, size) {\
	MYASSERT(size>1, ERR_RES_NO_RESOURCES);\
	if((index) >= size || (index) == 0 ) {\
		LOG("Bad resource index: %i. size=%i.\n", index, size);\
		BIG_PHAT_ERROR(ERR_RES_INVALID_INDEX); } }

		void destroy(unsigned index) {
			_destroy(index);
		}

		byte get_type(unsigned index) {
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				index&=~DYNAMIC_PLACEHOLDER_BIT;
				TESTINDEX(index, mDynResSize);
				return mDynResTypes[index];
			} else {
				TESTINDEX(index, mResSize);
				return mResTypes[index];
			}
		}

		/**
		 * It is used to see if a resource is already loaded.
		 * Works with both static and dynamic resources.
		 * @param index The handle to the resource.
		 * @return true if a resource is already loaded, false if not.
		 */
		bool is_loaded(unsigned index) {
			void **res = mRes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = mDynRes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, mDynResSize);
			} else {
				TESTINDEX(index, mResSize);
			}

			if (res[index] != NULL)
			{
				return true;
			}
			return false;
		}

		unsigned size() {
			return mResSize;
		}

		/**
		 * Implementation of maCreatePlaceholder.
		 * @return A placeholder handle.
		 */
		unsigned int create_RT_PLACEHOLDER() {
			// Get a placeholder from the pool, if available.
			if (mDynResPoolSize > 0) {
				--mDynResPoolSize;
				unsigned handle = mDynResPool[mDynResPoolSize];
				int placeholderIndex = handle & (~DYNAMIC_PLACEHOLDER_BIT);
				mDynResTypes[placeholderIndex] = RT_PLACEHOLDER;
				return handle;
			}

			// Expand the dynamic resource array if needed.
			if (mDynResSize + 1 > mDynResCapacity) {
				void** oldPlaceholders = mDynRes;
				byte* oldPlaceholderTypes = mDynResTypes;
				mDynRes = new void*[mDynResCapacity * 2];
				MYASSERT(mDynRes != NULL, ERR_OOM);
				mDynResTypes = new byte[mDynResCapacity * 2];
				MYASSERT(mDynResTypes != NULL, ERR_OOM);
				memset(mDynRes, 0, sizeof(void*) * (mDynResCapacity * 2));

				if (oldPlaceholders != NULL) {
					// Copy from old to new.
					memcpy(
						mDynRes,
						oldPlaceholders,
						mDynResCapacity * sizeof(void*));
					memcpy(
						mDynResTypes,
						oldPlaceholderTypes,
						mDynResCapacity * sizeof(byte));

					// Delete old arrays.
					delete []oldPlaceholders;
					delete []oldPlaceholderTypes;
				}

				// Set new capacity.
				mDynResCapacity = mDynResCapacity * 2;
			}

			// This is the index of the new placeholder.
			int placeholderIndex = mDynResSize;

			// Increment placeholder array size.
			++mDynResSize;

			// Add a placeholder type to the new array item.
			mDynResTypes[placeholderIndex] = RT_PLACEHOLDER;

			// Return the handle id, this is the index of the
			// array item with the dynamic resource bit set.
			return placeholderIndex | DYNAMIC_PLACEHOLDER_BIT;
		}

		/**
		 * Check if a handle refers to an existing dynamic resource object.
		 * @param index Placeholder handle.
		 * @return Non zero if data object exists, zero if it does not exist.
		 */
		int isDynamicResource(unsigned index) {

			// The handle must be a dynamic placeholder.
			if (!(index & DYNAMIC_PLACEHOLDER_BIT)) {
				return 0;
			}

			// Get the index into the dynamic resource array.
			int i = index & (~DYNAMIC_PLACEHOLDER_BIT);
			TESTINDEX(i, mDynResSize);

			// The placeholder must not have been destroyed.
			if (0 == mDynResTypes[i])
			{
				return 0;
			}

			// If the resource is a non-NULL object it exists.
			return NULL != mDynRes[i];
		}

		/**
		 * Destroy a placeholder. Mark this placeholder as free
		 * and store the index in the pool of free placeholders.
		 * @param index The placeholder handle.
		 * @return Status code.
		 */
		int _maDestroyPlaceholder(unsigned index) {
			// The handle must be a dynamic placeholder.
			if (!(index & DYNAMIC_PLACEHOLDER_BIT)) {
				// TODO: Panic
				return -2;
			}

			// Get the index into the dynamic resource array.
			int i = index & (~DYNAMIC_PLACEHOLDER_BIT);
			TESTINDEX(i, mDynResSize);

			// The placeholder must not have been destroyed.
			if (0 == mDynResTypes[i])
			{
				// TODO: Panic
				return -2;
			}

			// Set the handle type to 0. This marks the
			// placeholder as destroyed.
			mDynResTypes[i] = 0;

			// Put handle into the pool.

			// Create or expand the pool as needed.
			if (0 == mDynResPoolCapacity) {
				// Create the initial pool.
				mDynResPoolCapacity = 2;
				mDynResPool = new unsigned[mDynResPoolCapacity];
				MYASSERT(mDynResPool != NULL, ERR_OOM);
			}
			else if (mDynResPoolSize + 1 > mDynResPoolCapacity) {
				// Expand the pool.
				unsigned* oldDynResPool = mDynResPool;
				mDynResPool = new unsigned[mDynResPoolCapacity * 2];
				MYASSERT(mDynResPool != NULL, ERR_OOM);

				// Copy from old to new and delete old.
				memcpy(
					mDynResPool,
					oldDynResPool,
					mDynResPoolCapacity * sizeof(unsigned));
				delete []oldDynResPool;

				// Set new capacity.
				mDynResPoolCapacity = mDynResPoolCapacity * 2;
			}

			// Increment pool size.
			++mDynResPoolSize;

			// Add free handle index last in array (push to stack).
			mDynResPool[mDynResPoolSize - 1] = index;

			return RES_OK;
		}

#ifdef RESOURCE_MEMORY_LIMIT
		uint getResmemMax() const { return mResmemMax; }
		uint getResmem() const { return mResmem; }
#endif

		void logEverything() {
#define RESOURCE_STRINGS(R, T, D) resourceStrings[R] = #R;
			const char *resourceStrings[128] = {0};
			TYPES(RESOURCE_STRINGS)

#ifdef LOGGING_ENABLED
			LOG("Num static resources: %d\n", mResSize);
			LOG("Num dynamic resources: %d\n", mDynResSize);
			for(unsigned int i = 0; i < mResSize; i++) {
				byte type = mResTypes[i];
				LOG("Static resource %d is of type %s\n", i, resourceStrings[type]);
			}

			for(unsigned int i = 0; i < mDynResSize; i++) {
				byte type = mDynResTypes[i];
				LOG("Dynamic resource %d is of type %s\n", i, resourceStrings[type]);
			}
#endif
		}

	private:

		/**
		 * Delete and add a resource ("dadd").
		 * @param index Resource index.
		 * @param obj Pointer to object data.
		 * @param type Resource type.
		 */
		int _dadd(unsigned index, void* obj, byte type) {
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				unsigned pIndex = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(pIndex, mDynResSize);
				if(mDynRes[pIndex] != NULL) {
					_destroy(index);
				}
				return _add(index, obj, type);
			} else {
				TESTINDEX(index, mResSize);
				if(mRes[index] != NULL) {
					_destroy(index);
				}
				return _add(index, obj, type);
			}
		}

		/**
		 * Add a resource.
		 * @param index Resource index.
		 * @param obj Pointer to object data.
		 * @param type Resource type.
		 */
		int _add(unsigned index, void* obj, byte type) {
			void **res = mRes;
			byte *types = mResTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = mDynRes;
				types = mDynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, mDynResSize);
			} else {
				TESTINDEX(index, mResSize);
			}

			// obj is the resource data. If the resource is NULL
			// and not a placeholder or in flux (resource is changing)
			// then create a panic.
			if (obj == NULL && (type != RT_PLACEHOLDER && type != RT_FLUX)) {
				DEBIG_PHAT_ERROR;
			}

			// Resource at this index must not be in use, but it can be a
			// placeholder. If the resource is in use, a panic is generated.
			if(res[index] != NULL || types[index] != RT_PLACEHOLDER) {
				BIG_PHAT_ERROR(ERR_RES_OVERWRITE);
			}

#ifdef RESOURCE_MEMORY_LIMIT
			int oldResmem = mResmem;
			switch(type) {
#define CASE_ADDMEM(R, T, D) case R: mResmem += size_##R((T*)obj); break;
				TYPES(CASE_ADDMEM);
			}
			if(mResmem >= mResmemMax) {
				//BIG_PHAT_ERROR(ERR_RES_OOM);
				mResmem = oldResmem;
				__destroy(obj, type, index);	//avoids memory leaks
				return RES_OUT_OF_MEMORY;
			}
#endif	//RESOURCE_MEMORY_LIMIT
			res[index] = obj;
			types[index] = type;
			return RES_OK;
		}

		/**
		 * Gets a pointer to the data for the given handle.
		 * @param index The resource handle.
		 * @param R The resource type.
		 * @return a pointer to the data for the given handle.
		 */
		void* _get(unsigned index, byte R) {
			void **res = mRes;
			byte *types = mResTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = mDynRes;
				types = mDynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, mDynResSize);
			} else {
				TESTINDEX(index, mResSize);
			}

			if(types[index] != R) {
				BIG_PHAT_ERROR(ERR_RES_INVALID_TYPE);
			}
			return res[index];
		}

		void* _extract(unsigned index, byte R) {
			void **res = mRes;
			byte *types = mResTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = mDynRes;
				types = mDynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, mDynResSize);
			} else {
				TESTINDEX(index, mResSize);
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

		void __destroy(void* obj, byte type, unsigned index) {
			switch(type) {
#define CASE_DELETE(R, T, D) case R: D ((T*)obj);\
	LOGD("%s: %s %s %i\n", #R, #D, #T, index); break;
				TYPES(CASE_DELETE);
			}
		}

		void _destroy(unsigned index) {
			void **res = mRes;
			byte *types = mResTypes;
			if(index&DYNAMIC_PLACEHOLDER_BIT) {
				res = mDynRes;
				types = mDynResTypes;
				index = index&(~DYNAMIC_PLACEHOLDER_BIT);
				TESTINDEX(index, mDynResSize);
			} else {
				TESTINDEX(index, mResSize);
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

#ifdef RESOURCE_MEMORY_LIMIT
		// Max size of all resource data.
		const uint mResmemMax;

		// Current size of all resource data.
		uint mResmem;
#endif

		// ****** Static resources ****** //

		// Static resources are predefined resources defined
		// at build time, using .res files.

		// Number of resources (size of resource array).
		unsigned mResSize;
		// Reseource array (pointers to resource data).
		void** mRes;
		// Resource type info array.
		byte* mResTypes;

		// ****** Dynamic resources ****** //

		// Dynamic resources are allocated at runtime, and use
		// placeholders (handles) to reference data. Dynamic
		// resources are stored in an array. The array grows
		// if neccessary. A handle is an index into this array.
		// Dynamic resources start at index 1, so first element
		// of the array is unused (?).

		// Number of dynamic resources.
		unsigned mDynResSize;
		// Total size of dynamic resource array.
		unsigned mDynResCapacity;
		// Dynamic resource array.
		void** mDynRes;
		// Dynamic resource type info array.
		byte* mDynResTypes;

		// ****** Dynamic resource handle pool ****** //

		// This is a pool of free handles (array indexes) that
		// can be reused by maCreatePlaceholder. Free indexes
		// are stored in an array that can grow dynamically.
		// This array is used as a stack.

		// Number of free handles stored in the array.
		// This points to the top of the stack.
		unsigned mDynResPoolSize;
		// Total size of the array.
		unsigned mDynResPoolCapacity;
		// The array with free handle indexes.
		unsigned* mDynResPool;
	};
	// End of class ResourceArray
}

#endif // _BASE_RESOURCE_ARRAY_H_

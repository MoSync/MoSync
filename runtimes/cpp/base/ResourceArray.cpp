/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file ResourceArray.cpp
 *
 * This file contains the class ResourceArray, which holds
 * statically and dynamically allocated resources.
 */

#include "Syscall.h"
#include "base_errors.h"

using namespace MoSyncError;

namespace Base {

// If this bit is set in the handle, the handle is a dynamic placeholder.
// The non-dynamic handles are the ones refering to application resources.
#define DYNAMIC_PLACEHOLDER_BIT 0x40000000

// Make sure the given array index is valid.
#define TESTINDEX(index, size) {\
	MYASSERT(size>1, ERR_RES_NO_RESOURCES);\
	if((index) >= size || (index) == 0 ) {\
		LOG("Bad resource index: %i. size=%i.\n", index, size);\
		BIG_PHAT_ERROR(ERR_RES_INVALID_INDEX); } }

	/**
	 * Constructor.
	 */
	ResourceArray::ResourceArray(
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
	void ResourceArray::init(unsigned numResources) {
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

	/**
	 * Destructor.
	 */
	ResourceArray::~ResourceArray() {
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
			if (RT_NIL != mDynResTypes[i]) {
				_destroy(i | DYNAMIC_PLACEHOLDER_BIT);
			}
		}
		if(mDynRes) {
			delete[] mDynRes;
			delete[] mDynResTypes;
		}
		if(mDynResPool) {
			delete[] mDynResPool;
		}
	}

	void ResourceArray::destroy(unsigned index) {
		_destroy(index);
	}

	byte ResourceArray::get_type(unsigned index) {
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
	bool ResourceArray::is_loaded(unsigned index) {
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

	unsigned ResourceArray::size() {
		return mResSize;
	}

	/**
	 * Implementation of maCreatePlaceholder.
	 * @return A placeholder handle.
	 */
	unsigned int ResourceArray::create_RT_PLACEHOLDER() {
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
	int ResourceArray::isDynamicResource(unsigned index) {

		// The handle must be a dynamic placeholder.
		if (!(index & DYNAMIC_PLACEHOLDER_BIT)) {
			return 0;
		}

		// Get the index into the dynamic resource array.
		unsigned i = index & (~DYNAMIC_PLACEHOLDER_BIT);
		TESTINDEX(i, mDynResSize);

		// The placeholder must not have been destroyed.
		if (RT_NIL == mDynResTypes[i])
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
	int ResourceArray::_maDestroyPlaceholder(unsigned index) {
		// The handle must be a dynamic placeholder.
		if (!(index & DYNAMIC_PLACEHOLDER_BIT)) {
			MYASSERT_IF_PANICS_ENABLED(
				false, // Always trigger this panic.
				ERR_RES_PLACEHOLDER_NOT_DYNAMIC);
			return -2;
		}

		// Get the index into the dynamic resource array.
		unsigned i = index & (~DYNAMIC_PLACEHOLDER_BIT);
		TESTINDEX(i, mDynResSize);

		// The placeholder must not have been destroyed.
		if (RT_NIL == mDynResTypes[i])
		{
			MYASSERT_IF_PANICS_ENABLED(
				false, // Always trigger this panic.
				ERR_RES_PLACEHOLDER_ALREADY_DESTROYED);
			return -2;
		}

		// Set the handle type to RT_NIL. This marks the
		// placeholder as destroyed.
		mDynResTypes[i] = RT_NIL;

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

	void ResourceArray::logEverything() {
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

	/**
	 * Delete and add a resource ("dadd").
	 * @param index Resource index.
	 * @param obj Pointer to object data.
	 * @param type Resource type.
	 */
	int ResourceArray::_dadd(unsigned index, void* obj, byte type) {
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
	int ResourceArray::_add(unsigned index, void* obj, byte type) {
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
	void* ResourceArray::_get(unsigned index, byte R) {
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

	void* ResourceArray::_extract(unsigned index, byte R) {
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

	void ResourceArray::__destroy(void* obj, byte type, unsigned index) {
		switch(type) {
#define CASE_DELETE(R, T, D) case R: D ((T*)obj);\
LOGD("%s: %s %s %i\n", #R, #D, #T, index); break;
			TYPES(CASE_DELETE);
		}
	}

	void ResourceArray::_destroy(unsigned index) {
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
}
// End of namespace Base

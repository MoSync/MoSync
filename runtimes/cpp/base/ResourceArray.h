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
 * @file ResourceArray.h
 *
 * This file contains the class ResourceArray, which holds
 * statically and dynamically allocated resources.
 */

#ifndef _BASE_RESOURCE_ARRAY_H_
#define _BASE_RESOURCE_ARRAY_H_

#include <helpers/array.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_RESOURCE_TYPES.h>

#include "base_errors.h"
using namespace MoSyncError;

// ResourceDefs.h is platform specific and contains the TYPES
// macro. It expands by applying another macro to define
// types and delete operations on types. This file is located
// at different places in each runtime, but typically it
// should be found in the main cpp directory for the platform.
#include "ResourceDefs.h"

namespace Base {

	/**
	 * TODO: Document what this class is used for.
	 */
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

	/**
	 * Class that holds resources.
	 * Internally, "resource" and "object" are used interchangably.
	 */
	class ResourceArray {	//type and bound-safe
	public:
		/**
		 * Constructor.
		 */
		ResourceArray(
#ifdef RESOURCE_MEMORY_LIMIT
			uint resMax
#endif
			);

#ifdef RESOURCE_MEMORY_LIMIT
		uint getResmemMax() const { return mResmemMax; }
		uint getResmem() const { return mResmem; }
#endif

		/**
		 * Initialize the static resource array.
		 * This function saves the old objects present
		 * in the array.
		 * @param numResources The number of resource in the new array.
		 */
		void init(unsigned numResources);

		/**
		 * Destructor.
		 */
		~ResourceArray();

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

		void destroy(unsigned index);

		byte get_type(unsigned index);

		/**
		 * It is used to see if a resource is already loaded.
		 * Works with both static and dynamic resources.
		 * @param index The handle to the resource.
		 * @return true if a resource is already loaded, false if not.
		 */
		bool is_loaded(unsigned index);

		unsigned size();

		/**
		 * Implementation of maCreatePlaceholder.
		 * @return A placeholder handle.
		 */
		unsigned int create_RT_PLACEHOLDER();

		/**
		 * Check if a handle refers to an existing dynamic resource object.
		 * @param index Placeholder handle.
		 * @return Non zero if data object exists, zero if it does not exist.
		 */
		int isDynamicResource(unsigned index);

		/**
		 * Destroy a placeholder. Mark this placeholder as free
		 * and store the index in the pool of free placeholders.
		 * @param index The placeholder handle.
		 * @return Status code.
		 */
		int _maDestroyPlaceholder(unsigned index);

		void logEverything();

	private:

		/**
		 * Delete and add a resource ("dadd").
		 * @param index Resource index.
		 * @param obj Pointer to object data.
		 * @param type Resource type.
		 */
		int _dadd(unsigned index, void* obj, byte type);

		/**
		 * Add a resource.
		 * @param index Resource index.
		 * @param obj Pointer to object data.
		 * @param type Resource type.
		 */
		int _add(unsigned index, void* obj, byte type);

		/**
		 * Gets a pointer to the data for the given handle.
		 * @param index The resource handle.
		 * @param R The resource type.
		 * @return a pointer to the data for the given handle.
		 */
		void* _get(unsigned index, byte R);

		void* _extract(unsigned index, byte R);

		void __destroy(void* obj, byte type, unsigned index);

		void _destroy(unsigned index);

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
// End of namespace Base

#endif // _BASE_RESOURCE_ARRAY_H_

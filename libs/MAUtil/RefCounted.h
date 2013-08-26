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

/** \file RefCounted.h
* \brief Mix-in class for reference counting.
*/

#ifndef _SE_MSAB_MAUTIL_REFCOUNTED_H_
#define _SE_MSAB_MAUTIL_REFCOUNTED_H_

namespace MAUtil {

	/// A base class for reference counting.
	class RefCounted {
	public:
		/// Constructor, initializes the initial reference count to startCount.
		RefCounted(int startCount=1);
		/// Increments the reference count by one.
		void addRef();
		/// Decrements the reference count by one.
		void release();
		/// Returns the current reference count.
		int getRefCount();

		virtual ~RefCounted() {}
	private:
		/// A variable that keeps track of the reference count.
		int mRefCount;
	};

}

#endif

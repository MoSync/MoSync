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

/** \file ListenerSet.h
 * \brief Safe container for listeners
 * \author Fredrik Eldh
 */

#ifndef _LISTENER_SET_H_
#define _LISTENER_SET_H_

#include <maassert.h>

#include "Vector.h"

#define ListenerSet_each(type,itr,set)\
	for(ListenerSet<type>::iterator itr = set.begin(); itr.hasNext(); ++itr)

#define ListenerSet_fire(type, listenerset, with)\
	if(listenerset.size()) {\
		listenerset.setRunning(true);\
		ListenerSet_each(type, i, listenerset) {\
			i->with;\
		}\
		listenerset.setRunning(false);\
	}\

namespace MAUtil {
	/**
	* \brief A listener set.
	*/
	template<typename T> class ListenerSet {
	private:
		struct Combo {
			T* listener;
			int refCount;
		};

	public:
		/**
		* \brief An iterator for a listener set.
		*/
		class iterator {
		public:
			T& operator*() { return *mVec[mIndex].listener; }	//unsafe if !hasNext()
			T* operator->() { return mVec[mIndex].listener; }	//unsafe if !hasNext()
			iterator& operator++() {	//unsafe if !hasNext()
				do {
					mIndex++;
					if(mIndex == mCount)
						break;
				} while(mVec[mIndex].refCount == 0);
				return *this;
			}
			bool hasNext() { return mIndex < mCount; }

			iterator(Vector<Combo>& vec, int index) : mVec(vec), mCount(vec.size()), mIndex(index) {
				if(index < mCount) if(vec[index].refCount == 0)
					++(*this);
			}
		private:
			Vector<Combo>& mVec;
			const int mCount;
			int mIndex;
		};

		ListenerSet(bool shouldDelete) : mRunning(false), mUpdateRequired(false),
			mShouldDelete(shouldDelete)
		{
		}

		iterator begin() {
			return iterator(mVec, 0);
		}

		void add(T* t) {
			templateVector_each(Combo, i, mVec) {
				if(i->listener == t) {
					i->refCount = 1;
					return;
				}
			}
			Combo c = { t, 1 };
			mVec.add(c);
		}

		void remove(T* t) {
			templateVector_each(Combo, i, mVec) {
				if(i->listener == t) {
					if(mRunning) {
						i->refCount = 0;
						mUpdateRequired = true;
					} else {
						if(mShouldDelete)
							delete i->listener;
						mVec.remove(i);
					}
					return;
				}
			}
		}

		bool contains(T* t) {
			templateVector_each(Combo, i, mVec)
				if(i->listener == t)
					return true;
			return false;
		}

		void setRunning(bool r=true) {
			mRunning = r;
			if(mRunning == false && mUpdateRequired) {
				int s = mVec.size();
				//for(int i=vec.size(); i>=0; i--)
				templateVector_each(Combo, itr, mVec) {
					if(itr->refCount <= 0) {
						if(mShouldDelete)
							delete itr->listener;
						*itr = mVec[--s];
					}
				}
				mVec.resize(s);
				mUpdateRequired = false;
			}
		}

		int size() const {	//unsafe if running
			return mVec.size();
		}

	private:
		bool mRunning, mUpdateRequired, mShouldDelete;
		Vector<Combo> mVec;
	};
} // namespace MAUtil

#endif /* _LISTENER_SET_H_ */

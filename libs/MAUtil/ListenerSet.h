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

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

/** \file List.h
* \brief Generic, dynamic list container behaving like a subset of std::list.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUTIL_LIST_H_
#define _SE_MSAB_MAUTIL_LIST_H_

#include <maassert.h>

#ifdef MAUTIL_LIST_DEBUGGING
#define MAUTIL_LIST_LOG lprintfln
#else
#define MAUTIL_LIST_LOG(...)
#endif

namespace MAUtil {

	/**
	* \brief A generic, dynamic list container
	*
	* You can retrieve either a ListIterator or ConstListIterator to iterate through the list.
	*/
	template<typename Type> class List {
	public:
		friend class ListIterator;
		friend class ConstListIterator;
		/**
		* \brief A node in a list container.
		*/
		class ListNode {
		public:
			ListNode(const Type &data, ListNode *prev, ListNode *next) :
				mData(data), mPrev(prev), mNext(next)
				{
				}

			  Type mData;
			  ListNode *mPrev, *mNext;
		};

		class ConstListIterator;

		/**
		* \brief An iterator for a list container.
		*/
		class ListIterator {
		public:

			/**
			 * Function used to tell if there is a next element.
			 * \returns 'true' if there is a next element, otherwise 'false'.
			 */
			bool hasNext() const {
				return mCurrent!=NULL && (mFirst || mCurrent->mNext);
			}

			/**
			 * Function used to tell if there is a previous element.
			 * \returns 'true' if there is a previous element, otherwise 'false'.
			 */
			bool hasPrev() const {
				return mCurrent!=NULL && (mFirst || mCurrent->mPrev);
			}

			/**
			* Returns the data the iterator points to and attempts to move the iterator backwards.
			* \returns a const reference to the data the iterator points to.
			*/
			Type& prev() {
				if(!mFirst && mCurrent->mPrev) {
					mCurrent = mCurrent->mPrev;
				} else {
					mFirst = false;
				}
				return mCurrent->mData;
			}

			/**
			* Returns the data the iterator points to and attempts to move the iterator forward.
			* \returns a const reference to the data the iterator points to.
			*/
			Type& next() {
				if(!mFirst && mCurrent->mNext) {
					mCurrent = mCurrent->mNext;
				} else {
					mFirst = false;
				}
				return mCurrent->mData;
			}

		private:
			friend class ConstListIterator;
			friend class List;

			ListIterator(ListNode *cur) : mFirst(true) {
				mCurrent = cur;
			}

			bool mFirst;
			ListNode *mCurrent;
		};

		/**
		* \brief A constant iterator for a list container.
		*/
		class ConstListIterator {
		public:
			ConstListIterator(const ListIterator& listIterator) : mCurrent(listIterator.mCurrent), mFirst(true) {

			}

			/**
			 * Function used to tell if there is a next element.
			 * \returns 'true' if there is a next element, otherwise 'false'.
			 */
			bool hasNext() const {
				return mCurrent!=NULL && (mFirst || mCurrent->mNext);
			}

			/**
			 * Function used to tell if there is a previous element.
			 * \returns 'true' if there is a previous element, otherwise 'false'.
			 */
			bool hasPrev() const {
				return mCurrent!=NULL && (mFirst || mCurrent->mPrev);
			}

			/**
			* Returns the data the iterator points to and attempts to move the iterator backwards.
			* \returns a const reference to the data the iterator points to.
			*/
			const Type& prev() {
				if(!mFirst && mCurrent->mPrev) {
					mCurrent = mCurrent->mPrev;
				} else {
					mFirst = false;
				}
				return mCurrent->mData;
			}

			/**
			* Returns the data the iterator points to and attempts to move the iterator forward.
			* \returns a const reference to the data the iterator points to.
			*/
			const Type& next() {
				if(!mFirst && mCurrent->mNext) {
					mCurrent = mCurrent->mNext;
				} else {
					mFirst = false;
				}
				return mCurrent->mData;
			}
		private:
			friend class List;

			ConstListIterator(const ListNode* cur) {
				mCurrent = cur;
			}

			bool mFirst;
			const ListNode *mCurrent;
		};

		/// Constructs an empty list.
		List()
			:	mHead(NULL), mTail(NULL), mSize(0)
		{
		}

		/**
		* Adds an element to the beginning of the list.
		*/
		void addFirst(const Type &data) {
			ListNode *newNode = new ListNode(data, NULL, mHead);
			mSize++;
			if(!mHead && !mTail) {
				mHead = mTail = newNode;
				return;
			}
			mHead->mPrev = newNode;
			mHead = newNode;
		}

		/**
		* Adds an element to the end of the list.
		*/
		void addLast(const Type &data) {
			ListNode *newNode = new ListNode(data, mTail, NULL);
			mSize++;
			if(!mHead && !mTail) {
				mHead = mTail = newNode;
				return;
			}
			mTail->mNext = newNode;
			mTail = newNode;
		}

		/**
		* Inserts an element at a specific position in the list.
		* \param iterator An iterator pointing to the element after which the element is to be added.
		* The iterator is invalidated by this operation.
		* \param data The element to be inserted.
		* \returns A new iterator pointing to the newly added element.
		*/
		ListIterator insert(ListIterator iterator, const Type& data) {
			ListNode *next = iterator.mCurrent->mNext;
			ListNode *newNode = new ListNode(data, iterator.mCurrent, next);
			iterator.mCurrent->mNext = newNode;
			if(next!=NULL) {
				next->mPrev = newNode;
			} else {
				mTail = newNode;
			}
			mSize++;
		}

		/**
		* Removes an element from list.
		* \param iterator An iterator pointing to the element to be removed.
		* The iterator is invalidated by this operation.
		* \returns A new iterator pointing to the element following the removed.
		*/
		ListIterator remove(ListIterator iterator) {
			if(mSize==0) return ListIterator(NULL);
			if(iterator.hasPrev()) {
				iterator.mCurrent->mPrev->mNext = iterator.mCurrent->mNext;
			} else {
				mHead = iterator.mCurrent->mNext;
			}
			if(iterator.hasNext()) {
				iterator.mCurrent->mNext->mPrev = iterator.mCurrent->mPrev;
			} else {
				mTail = iterator.mCurrent->mPrev;
			}
			mSize--;
			ListIterator nextItr(iterator.mCurrent->mNext);
			delete iterator.mCurrent;
			return nextItr;
		}

		/**
		* Returns an iterator pointing to the first element in the list.
		*/
		ListIterator begin() {
			return ListIterator(mHead);
		}

		/**
		* Returns an iterator pointing beyond the last element of the list.
		*/
		ListIterator end() {
			return ListIterator(mTail);
		}

		ConstListIterator begin() const {
			return ConstListIterator(mHead);
		}

		ConstListIterator end() const {
			return ConstListIterator(mTail);
		}

		/**
		* Returns the number of elements in the list.
		*/
		int	size() const {
			return mSize;
		}

	private:
		ListNode *mHead;
		ListNode *mTail;
		int mSize;
	};

}

#endif /* _SE_MSAB_MAUTIL_LIST_H_ */

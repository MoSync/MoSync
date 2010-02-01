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

/* 
* File:   list.h
* Author: Niklas Nummelin
*
* Created on January 29, 2010
*/

// this must be extended in the future, it's just a minimal subset for now.
// took the vector we wrote for MAUtil and modified it to have the STL interface.

#ifndef __MOSTL_VECTOR_H__
#define	__MOSTL_VECTOR_H__

#include "common.h"

NAMESPACE_BEGIN ( mostd )

template<typename Type>
class vector {

public:
	/// Defines a typesafe iterator for the template instance.
	typedef Type* iterator;
	typedef const Type* const_iterator;

	/** \brief Constructs the Vector and sets its capacity to \a initialCapacity.
	*  \param initialCapacity The initial capacity of the Vector.   
	*/
	vector(unsigned long initialCapacity=4) {
		mData = new Type[initialCapacity];
		mCapacity = initialCapacity;
		mSize = 0;
	}

	/**
	* Copies the \a other vector.
	*/
	vector(const vector& other) {
		mCapacity = other.mCapacity;
		mSize = other.mSize;
		mData = new Type[mCapacity];
		for(int i=0; i<mSize; i++) {
			mData[i] = other.mData[i];
		}
	}

	/// Destructor
	~vector() {
		delete[] mData;
	}

	/**
	* Copies the \a other vector.
	* \returns A reference to this vector.
	*/
	vector& operator=(const vector& other) {
		delete[] mData;
		mCapacity = other.mCapacity;
		mSize = other.mSize;
		mData = new Type[mCapacity];
		for(int i=0; i<mSize; i++) {
			mData[i] = other.mData[i];
		}
		return *this;
	}

	/** \brief Adds an element to the end of the Vector.
	*  \param val The element to be added.
	*/
	void push_back(const Type& val) {
		if(mSize >= mCapacity-1) {
			if(mCapacity != 0)
				reserve(mCapacity*2);
			else
				reserve(4);
		}
		mData[mSize++] = val;
	}

	/** \brief Adds several elements to the end of the Vector.
	*  \param ptr A pointer to the elements.
	*  \param num The number of elements.
	*/
	/*
	void push_back(const Type* ptr, int num) {
		int neededCapacity = mSize + num;
		if(mCapacity < neededCapacity) {
			do {
				mCapacity *= 2;
			} while(mCapacity < neededCapacity);
			reserve(mCapacity);
		}
		for(int i=0; i<num; i++) {
			mData[mSize++] = *(ptr++);
		}
	}
	*/

	/** \brief Removes the element pointed to by iterator \a i.
	* \param i An iterator pointing to the element that should be removed.
	*/
	void erase(iterator i) {
		iterator e = end();

		while(i != (e-1)) {
			*i = *(i+1);
			i++;
		}

		//mSize--;
		resize(mSize-1);
	}

	/** \brief Removes the element at \a index.
	*  \param index The index of the element that should be removed.
	*/
	/*
	void remove(int index) {
		remove(mData + index);
	}
	*/

	/** \brief Inserts the element at \a index, moving all existing elements beginning at 'index' one step forward.
	*  \param pos Position in the vector where the new element should be inserted
	*  \param t The element itself.
	*/
	void insert(iterator pos, const Type& t) {
		resize(mSize+1);
		iterator e = end()-1;
		iterator i = pos;

		while(e != i)
		{
			*(e) = *(e-1);
			e--;
		}
		*i = t;
	}

	/** \brief Returns the number of elements.
	*  \return Returns the number of elements currently in the Vector.
	*  \see capacity
	*/

	 unsigned long size() const {
		return mSize;
	}

	/** \brief Resizes the Vector to contain \a size elements.
	*  \param newSize The desired size of the Vector.
	*/
	void resize(unsigned long newSize) {
		reserve(newSize);
		for(unsigned long i = newSize; i < mSize; i++) {
			mData[i] = Type();
		}
		mSize = newSize;
	}

	/** \brief Reserves space in the Vector.
	*  \param newCapacity The desired capacity of the Vector.
	*  \note If \a newCapacity is less than the current capacity of the Vector, nothing will happen.
	*/
	void reserve(unsigned long newCapacity) {
		if(newCapacity <= mCapacity)
			return;
		Type* newData = new Type[newCapacity];
		for(unsigned long i=0; i < mSize; i++) {
			newData[i] = mData[i];
		}
		mCapacity = newCapacity;
		delete[] mData;
		mData = newData;
	}

	/** \brief Clears the Vector, setting its size to 0 but not altering its capacity
	*/
	void clear() {
		resize(0);
	}

	/** \brief Returns true iff the Vector is empty().
	*/
	bool empty() const {
		return mSize == 0;
	}

	/** \brief Returns the Vector's current capacity. 
	* \see size
	*/
	unsigned long capacity() const {
		return mCapacity;
	}

	/** \brief Returns an iterator pointing to the first element of the Vector.
	*/

	iterator begin() {
		return mData;
	}

	/** \brief Returns an iterator pointing beyond the last element of the Vector.
	*/

	iterator end() {
		return mData + mSize;
	}

	/** \brief Returns an iterator pointing to the first element of the Vector.
	*/

	const_iterator begin() const {
		return mData;
	}

	/** \brief Returns an iterator pointing beyond the last element of the Vector.
	*/

	const_iterator end() const {
		return mData + mSize;
	}

	/** \brief Returns a reference to the element at \a index. No range-checking is done.
	*  \param index The index of the element to be returned.
	*/
	Type& operator[](unsigned long index) {
		return mData[index];
	}

	/** \brief Returns a const reference to the element at \a index. No range-checking is done.
	*  \param index The index of the element to be returned.
	*/
	const Type& operator[](unsigned long index) const {
		return mData[index];
	}

	/** \brief Returns a const pointer to the Vector's storage array.
	*  \note The pointer becomes invalid when the Vector is changed.
	*/
	/*
	const Type* pointer() const {
		return mData;
	}
	*/

	/** \brief Returns a pointer to the Vector's storage array.
	*  \note The pointer becomes invalid when the Vector is changed.
	*/
	/*
	Type* pointer() {
		return mData;
	}
	*/

protected:
	unsigned long mSize;
	unsigned long mCapacity;
	Type* mData;

};

NAMESPACE_END

#endif /* __MOSTL_VECTOR_H__ */
// e32tools\petran\Szip\farray.h
//
// Copyright (c) 1998-2004 Symbian Software Ltd. All rights reserved.
//

#ifndef __FARRAY_H__
#define __FARRAY_H__
#include <assert.h>

template <class T,TInt S> 
class TFixedArray
	// Range checking wrapper+ class for C++ arrays
	// Can be embedded in C-objects, or used on the stack: use Reset() to zero it
{
	typedef TFixedArray<T,S> ThisClass;
public:
	inline TFixedArray();
	inline TFixedArray(const T* aList, TInt aLength);
	//
	inline void Copy(const T* aList, TInt aLength);
	inline void Reset();		// zero fill
	inline void DeleteAll();
	//
	inline TInt Count() const;
	inline TInt Length() const;
	// Accessors - debug range checking
	inline T& operator[](TInt aIndex);
	inline const T& operator[] (TInt aIndex) const;
	// Accessors - always range checking
	inline T& At(TInt aIndex);
	inline const T& At(TInt aIndex) const;
	// Provides pointers to the beginning and end of the array
	inline T* Begin();
	inline T* End();
	inline const T* Begin() const;
	inline const T* End() const;
	//
protected:
	inline static TBool InRange(TInt aIndex);
protected:
	T iRep[S];
};

// Template class TFixedArray
IMPORT_C void PanicTFixedArray();

template <class T,TInt S>
inline TFixedArray<T,S>::TFixedArray()
{}
template <class T,TInt S>
inline void TFixedArray<T,S>::Copy(const T* aList,TInt aLength)
{assert(TUint(aLength)<=TUint(S));HMem::Copy(iRep,aList,aLength*sizeof(T));}
template <class T,TInt S>
inline TFixedArray<T,S>::TFixedArray(const T* aList,TInt aLength)
{Copy(aList,aLength);}
template <class T,TInt S>
inline void TFixedArray<T,S>::Reset()
{HMem::FillZ(iRep,sizeof(iRep));}
template <class T,TInt S>
inline TInt TFixedArray<T,S>::Count() const
{return S;}
template <class T,TInt S>
inline TInt TFixedArray<T,S>::Length() const
{return sizeof(T);}
template <class T,TInt S>
inline TBool TFixedArray<T,S>::InRange(TInt aIndex)
{return TUint(aIndex)<S;}
template <class T,TInt S>
inline T& TFixedArray<T,S>::operator[](TInt aIndex)
{assert(InRange(aIndex));return iRep[aIndex];}
template <class T,TInt S>
inline const T& TFixedArray<T,S>::operator[](TInt aIndex) const
{return const_cast<ThisClass&>(*this)[aIndex];}
template <class T,TInt S>
inline T& TFixedArray<T,S>::At(TInt aIndex)
{verify(InRange(aIndex));return iRep[aIndex];}
template <class T,TInt S>
inline const T& TFixedArray<T,S>::At(TInt aIndex) const
{return const_cast<ThisClass&>(*this).At(aIndex);}
template <class T,TInt S>
inline T* TFixedArray<T,S>::Begin()
{return &iRep[0];}
template <class T,TInt S>
inline T* TFixedArray<T,S>::End()
{return &iRep[S];}
template <class T,TInt S>
inline const T* TFixedArray<T,S>::Begin() const
{return &iRep[0];}
template <class T,TInt S>
inline const T* TFixedArray<T,S>::End() const
{return &iRep[S];}
#endif

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

/**
 * \file Enumerator.h
 * \brief Generic interface for enumerations.
 * \author Lars Ake Vinberg
 */

#ifndef ENUMERATOR_H_
#define ENUMERATOR_H_

#include "MemoryMgr.h"

namespace MAPUtil
{
	/**
	* \brief Generic Enumerable interface template
	*/
	template<class T>
	class IEnumerable
	{
	public:
		virtual int size( ) = 0;
		virtual T getItem( int index ) = 0;
	};

	/**
	* \brief Generic enumerator class.
	*/
	template<class T>
	class Enumerator
	{
	public:
		Enumerator( IEnumerable<T>& source ) :
			mSource( source )
		{
			mCurrent = -1;
		}

		virtual ~Enumerator( ) { }

		virtual bool moveNext( )
		{
			mCurrent++;
			return mCurrent < mSource.size( ); 
		}

		virtual void reset( )
		{
			mCurrent = 0;
		}

		virtual T current( ) 
		{
			T t = mSource.getItem( mCurrent );
			return t;
		}

		virtual int indexOfCurrent( )
		{
			return mCurrent;
		}

		virtual void setIndexOfCurrent( int index )
		{
			if ( index < 0 ) 
				mCurrent = 0;
			else
				mCurrent = index;
		}

		//virtual void getItem( int index, T& item )
		//{
		//	if ( index < 0 ) 
		//		return;
		//	if ( index >= mSource->size( ) )
		//		return;
		//	mSource->getItem( index, item );
		//}

		virtual T operator[]( int index )
		{
			//if ( index < 0 ) 
			//	return NULL;
			//if ( index >= mSource->size( ) )
			//	return NULL;
			return mSource.getItem( index );
		}

		virtual int size( )
		{
			return mSource.size( );
		}

	private:
		IEnumerable<T>& mSource;
		int mCurrent;
	};
}

#endif // ENUMERATOR_H_

/* Copyright (C) 2010 Mobile Sorcery AB

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

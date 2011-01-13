//
// Enumerator.h
//
// Author: Lars Ake Vinberg
//

#ifndef ENUMERATOR_H_
#define ENUMERATOR_H_

#include <MAP/MemoryMgr.h>

namespace MAPUtil
{
	//=========================================================================
	//
	// Generic Enumerable interface template
	//
	template<class T>
	class IEnumerable
	//=========================================================================
	{
	public:
		virtual int size( ) = 0;
		virtual T getItem( int index ) = 0;
	};

	//=========================================================================
	//
	// Generic enumerator class.
	//
	template<class T>
	class Enumerator
	//=========================================================================
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

/*
 * handleguard.cpp
 *
 *  Created on: 28 okt 2010
 *      Author: Ali Mosavian
 */
#ifndef __CONNECTION_CPP__
#define __CONNECTION_CPP__

#include <maapi.h>

class Connection
{
private:
	MAHandle m_handle;


public:
	Connection ( const char *url )
	{
		m_handle = maConnect( url );
	}

	~Connection ( void )
	{
		if ( isValid( ) == false )
			return;

		maConnClose( m_handle );
	}

	bool isValid ( void )
	{
		return m_handle != 0;
	}

	void read ( void *dst, int maxSize )
	{
		maConnRead( m_handle, dst, maxSize );
	}

	void write ( const void *src, int size )
	{
		maConnWrite( m_handle, src, size );
	}
};


#endif /* __CONNECTION_CPP__ */

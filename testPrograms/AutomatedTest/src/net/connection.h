/* Copyright (C) 2010 MoSync AB

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

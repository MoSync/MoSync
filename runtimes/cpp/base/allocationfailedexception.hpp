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
 * File:   AllocationFailedException.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __ALLOCATIONFAILEDEXCEPTION_HPP__
#define	__ALLOCATIONFAILEDEXCEPTION_HPP__

#include <string>
#include <exception>


namespace Base
{
    /**
     * This class is thrown when a allocation operation fails.
     *
     */
    class AllocationFailedException : public std::exception
    {
    private:
        std::string     m_msg;

    public:
        /**
         * Constructor
         *
         * @param m     Message (default is empty string)
         */
        AllocationFailedException ( const char * m = "" )
        : std::exception(), m_msg ( m )
        {
        }

        /**
         * Copy constructor
         *
         * @param o     Instance to copy
         */
        AllocationFailedException ( const AllocationFailedException &o )
        : std::exception(), m_msg ( o.m_msg )
        {
        }

		/**
		 * Destructor
		 */
		~AllocationFailedException( void )
		throw ( )
		{
		}

        /**
         * Overides std::exception what method, returns the
         * reason for the exception.
         *
         * @return Pointer to a const asciiz string.
         */
        virtual const char * what ( void )
        throw( )
        {
            return m_msg.c_str( );
        }
    };
}


#endif	/* _ALLOCATIONFAILEDEXCEPTION_HPP */


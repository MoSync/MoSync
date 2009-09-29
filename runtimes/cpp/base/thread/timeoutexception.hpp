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
 * File:   timeoutexception.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __TIMEOUTEXCEPTION_HPP__
#define	__TIMEOUTEXCEPTION_HPP__

#include <string>
#include <exception>


namespace Base
{
    namespace Thread
    {
        /**
         * This class is thrown when blocking operations with time out
         * run out of time with out success.
         *
         */
        class TimeoutException : public std::exception
        {
        private:
            std::string     m_msg;

        public:
            /**
             * Constructor
             *
             * @param m     Message (default is empty string)
             */
            TimeoutException ( char * m = "" )
            : std::exception(), m_msg ( m )
            {
            }

            /**
             * Copy constructor
             *
             * @param o     Instance to copy
             */
            TimeoutException ( const TimeoutException &o )
            : std::exception(), m_msg ( o.m_msg )
            {
            }
			
			/**
			 * Destructor
			 */
			~TimeoutException ( void )
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
}




#endif	/* _TIMEOUTEXCEPTION_HPP */


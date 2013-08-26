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


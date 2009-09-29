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
 * File:   bootstrap.hpp
 * Author: Ali Mosavian
 *
 * Created on July 20, 2009
 */

#ifndef __BOOTSTRAP_HPP__
#define	__BOOTSTRAP_HPP__

namespace Base
{
    namespace Thread
    {
        /**
         * Inits the thread sub systems in the correct order.
         * Note: This should be called first in MALibInit
         *
         */
        void startup ( void );

        /**
         * Shuts down the thread sub systems in the correct order.
         * Note: This should be called last in MALibQuit
         *
         */
        void shutdown ( void );
    }
}


#endif	/* __BOOTSTRAP_HPP__ */


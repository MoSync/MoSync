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
 * File:   cbtservice.hpp
 * Author: Ali Mosavian
 *
 * Created on August 5, 2009
 */
#ifndef __CBTSERVICE_HPP__
#define	__CBTSERVICE_HPP__

#include <list>
#include <string>
#include <helpers/types.h>
#include <helpers/cpp_defs.h>

namespace Bluetooth
{
    namespace BlueZ
    {
        /**
         * A container for a bluetooth service.
         * 
         */
        class CBtService
        {
        private:
            u8                  mPort;
            std::string         mName;
            std::list<MAUUID>   mUuidList;

        public:
            /**
             * Constructor, sets the service name
             *
             * @param n     Service name
             */
            CBtService ( const char* n )
            : mName( n )
            { }

            /**
             * Sets the service port.
             * Note: This is for the RFCOMM channel only.
             *
             * @param p     Port number
             */
            void setPort ( u8 p )
            {
                mPort = p;
            }

            /**
             * Adds another UUID (MoSync byte order).
             *
             * @param u     The UUID to add
             */
            void addUUID ( const MAUUID &u )
            {
                mUuidList.push_back( u );
            }

            /**
             * Returns the number of UUIDs the service has
             *
             * @return UUID count
             */
            int getUUIDCount ( void )
            {
                return mUuidList.size( );
            }

            /**
             * Returns the service name length
             *
             * @return Number of chars
             */
            int getNameLength ( void )
            {
                return mName.size( );
            }

            /**
             * Fills the MABtService structure with the service info.
             *
             * @param s     Pointer to the strcture to fill.
             */
            void getInfo ( MABtServiceNative* d )
            {
                int i = 0;
                std::list<MAUUID>::iterator it;

                d->port = mPort;
                strncpy( d->name, mName.c_str( ), d->nameBufSize );
                
                for ( it = mUuidList.begin( ); it != mUuidList.end( ); ++it )
                    d->uuids[i++] = *it;
            }
        };
    }
}


#endif	/* __CBTSERVICE_HPP__ */


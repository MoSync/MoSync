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


/* Copyright (C) 2013 Mobile Sorcery AB

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

#include "CameraEventManager.h"

namespace MAUtil
{
    CameraEventManager* CameraEventManager::sInstance = NULL;


    CameraEventManager::CameraEventManager()
    {
    }


    CameraEventManager::~CameraEventManager()
    {
    }


    CameraEventManager* CameraEventManager::getInstance()
    {
        if ( NULL == CameraEventManager::sInstance )
        {
            CameraEventManager::sInstance = new CameraEventManager();
        }

        return sInstance;
    }


    void CameraEventManager::destroyInstance()
    {
        if ( NULL != CameraEventManager::sInstance )
        {
            delete CameraEventManager::sInstance;
            CameraEventManager::sInstance = NULL;
        }
    }


    void CameraEventManager::customEvent(const MAEvent& event)
    {
        // Check if this is a camera event.
        if (EVENT_TYPE_CAMERA_SNAPSHOT == event.type ||
            EVENT_TYPE_CAMERA_PREVIEW == event.type)
        {
            for ( int i = 0; i < mListeners.size(); ++i )
            {
                mListeners[i]->handleCameraEvent(event);
            }
        }
    }


    void CameraEventManager::addCameraListener(ICameraListener* listener)
    {
        mListeners.add(listener);

        // When the first listener is registered, start receiving custom events.
        if ( 1 == mListeners.size() )
        {
            MAUtil::Environment::getEnvironment().addCustomEventListener(this);
        }
    }


    void CameraEventManager::removeCameraListener(ICameraListener* listener)
    {
        for ( int i = 0; i < mListeners.size(); ++i )
        {
            if ( mListeners[i] == listener )
            {
                mListeners.remove(i);

                // Stop listening to custom events if no listener is attached.
                if ( 1 > mListeners.size() )
                {
                    MAUtil::Environment::getEnvironment().removeCustomEventListener(this);
                }
                break; //Exit for loop.
            }
        }
    }

} // namespace MAUtil

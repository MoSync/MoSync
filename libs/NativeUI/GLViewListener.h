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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file GLViewListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for GLView events.
 */

#ifndef NATIVEUI_GL_VIEW_LISTENER_H_
#define NATIVEUI_GL_VIEW_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class GLView;

    /**
     * \brief Listener for GLView events.
     */
    class GLViewListener
    {
    public:
        /**
         * This method is called when the OpenGL view has been initialized
         * and is ready for setup.
         * @param glView The OpenGL view object that generated the event.
         */
        virtual void glViewReady(GLView* glView) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_GL_VIEW_LISTENER_H_ */

/*! @} */

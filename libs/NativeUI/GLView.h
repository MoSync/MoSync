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
 * @file GLView.h
 * @author Mikael Kindborg and Emma Tresanszki
 *
 * \brief Class for gl view.
 */

#ifndef NATIVEUI_GL_VIEW_H_
#define NATIVEUI_GL_VIEW_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class GLViewListener;

	/**
	 * \brief A GL view is a widget that is used to display graphics rendered
	 * by the GPU using OpenGL calls.
	 * Note: This widget is not available on Windows Phone 7.
	 */
	class GLView : public Widget
	{
	public:
		/**
		 * Constructor.
		 * @param type One of the constants:
		 * #MAW_GL_VIEW
		 * #MAW_GL2_VIEW
		 */
		GLView(const MAUtil::String& type);

		/**
		 * Destructor.
		 */
		virtual ~GLView();

		/**
		 * Tell the GL view that it should be redrawn.
		 */
		virtual void redraw();

		/**
		 * Tell the gl view that all following gl calls will apply to this view.
		 */
		virtual void bind();

        /**
         * Add an GL view event listener.
         * @param listener The listener that will receive GL view events.
         */
        virtual void addGLViewListener(GLViewListener* listener);

        /**
         * Remove the GL view listener.
         * @param listener The listener that receives GL view events.
         */
        virtual void removeGLViewListener(GLViewListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with GL view listeners.
         */
        MAUtil::Vector<GLViewListener*> mGLViewListeners;
	};

} // namespace NativeUI

#endif

/*! @} */

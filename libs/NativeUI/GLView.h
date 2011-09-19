/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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

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
 * @file GLView.cpp
 * @author Mikael Kindborg
 *
 * Class for GL view.
 */

#include "GLView.h"

#include "GLViewListener.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	GLView::GLView(const MAUtil::String type):
		Widget(type)
	{
		// Set some common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	GLView::~GLView()
	{
        mGLViewListeners.clear();
	}

	/**
	 * Tell the GL view that it should be redrawn.
	 */
	void GLView::redraw()
	{
		setProperty(MAW_GL_VIEW_INVALIDATE,"");
	}

	/**
	 * Tell the gl view that all following gl calls will apply to this view.
	 */
	void GLView::bind()
	{
		setProperty(MAW_GL_VIEW_BIND, "");
	}

    /**
     * Add an GL view event listener.
     * @param listener The listener that will receive GL view events.
     */
    void GLView::addGLViewListener(GLViewListener* listener)
    {
        addListenerToVector(mGLViewListeners, listener);
    }

    /**
     * Remove the GL view listener.
     * @param listener The listener that receives GL view events.
     */
    void GLView::removeGLViewListener(GLViewListener* listener)
    {
        removeListenerFromVector(mGLViewListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void GLView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_GL_VIEW_READY == widgetEventData->eventType)
        {
            for (int i = 0; i < mGLViewListeners.size(); i++)
            {
                mGLViewListeners[i]->glViewReady(this);
            }
        }
    }

} // namespace NativeUI

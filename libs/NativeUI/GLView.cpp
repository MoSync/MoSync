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

/**
 * @file GLView.cpp
 * @author Mikael Kindborg and Emma Tresanszki
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
	GLView::GLView(const MAUtil::String& type):
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

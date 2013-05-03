/*
 The MIT License (MIT)

 Copyright (c) 2013 Gregor Gullwi

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

/**
 * @file NavigationBarButton.cpp
 * @author Gregor Gullwi
 *
 * Class for navigation bar buttons.
 */

#include "NavigationBarButton.h"
#include "ButtonListener.h"

namespace NativeUI
{
    NavigationBarButton::NavigationBarButton(NavigationBarButtonType buttonType) :
        Widget(MAW_NAV_BAR_BUTTON, buttonType)
    {
    }

    NavigationBarButton::NavigationBarButton(Widget* widget) :
        Widget(MAW_NAV_BAR_BUTTON, NAV_BAR_BUTTON_TYPE_WIDGET)
    {
        this->addChild(widget);
    }

    NavigationBarButton::~NavigationBarButton()
    {
        mButtonListeners.clear();
    }

    void NavigationBarButton::setText(const MAUtil::String& text)
    {
        this->setProperty(MAW_BUTTON_TEXT, text);
    }

    MAUtil::String NavigationBarButton::getText()
    {
        return this->getPropertyString(MAW_BUTTON_TEXT);
    }

    int NavigationBarButton::setTintColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.8X", color);
        return setProperty(MAW_WIDGET_BACKGROUND_COLOR, buffer);
    }

    int NavigationBarButton::setTintColor(const int red, const int blue, const int green, const int alpha)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.2X%.2X%.2X%.2X", alpha, red, green, blue);
        return setProperty(MAW_WIDGET_BACKGROUND_COLOR, buffer);
    }

    void NavigationBarButton::addButtonListener(ButtonListener* listener)
    {
        addListenerToVector(mButtonListeners, listener);
    }

    void NavigationBarButton::removeButtonListener(ButtonListener* listener)
    {
        removeListenerFromVector(mButtonListeners, listener);
    }

    void NavigationBarButton::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_POINTER_PRESSED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonPressed(this);
            }
        }
        else if (MAW_EVENT_POINTER_RELEASED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonReleased(this);
            }
        }
        else if (MAW_EVENT_CLICKED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonClicked(this);
            }
        }
    }

} // namespace NativeUI

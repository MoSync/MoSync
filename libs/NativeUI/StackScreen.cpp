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
 * @file StackScreen.cpp
 * @author Niklas Nummelin & Mikael Kindborg
 */

#include "StackScreen.h"

#include "StackScreenListener.h"

namespace NativeUI
{

    /**
     * Constructor.
     */
    StackScreen::StackScreen() :
        Screen(MAW_STACK_SCREEN)
    {
    }

    /**
     * Destructor.
     */
    StackScreen::~StackScreen()
    {
        mStackScreenListeners.clear();
    }

    /*
     * Get the total number of the screens in the stack.
     */
    int StackScreen::getStackSize()
    {
        return mStack.size();
    }

    /*
     * Push a screen inside the stack.
     * @param screen The new screen.
     */
    void StackScreen::push(Screen* screen)
    {
        maWidgetStackScreenPush(getWidgetHandle(), screen->getWidgetHandle());
        mStack.add(screen);
    }

    /**
     * Pops a screen from a screen stack, hides the current screen and shows
     * the previous screen.
     * If there is no previous screen in the screen stack,
     * an empty screen will be shown.
     */
    void StackScreen::pop()
    {
        if (mStack.size() > 0)
        {
            maWidgetStackScreenPop(getWidgetHandle());
        }
    }

    /*
     * Specify whether the back button automatically
     * should pop the stack screen.
     * This can have different behaviors on different platforms:
     * on iPhone the UI back button is disabled,
     * on Android the stack screen ignores back button events.
     * @param enabledState True if the back button is enabled, false otherwise.
     */
    void StackScreen::enableBackButton(const bool enabledState)
    {
        this->setProperty(MAW_STACK_SCREEN_BACK_BUTTON_ENABLED,
            (enabledState ? "true" : "false") );
    }

    /**
     * TODO: Check this code! We might not need this. Unsure about how
     * it works.
     *
     * This method is called when there is an event for this widget.
     * It passes on the event to the widget's listener if one is set.
     * Note: You can either use an event listener or override this
     * method in a sublclass to handle events.
     * @param widgetEventData The data for the widget event.
     */
    void StackScreen::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Screen::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_STACK_SCREEN_POPPED == widgetEventData->eventType)
        {
            MAWidgetHandle fromScreenHandle = widgetEventData->fromScreen;
            MAWidgetHandle toScreenHandle = widgetEventData->toScreen;
            Screen* fromScreen = NULL;
            Screen* toScreen = NULL;

            for (int i = 0; i < mStack.size(); i++)
            {
                Widget* widget = mStack[i];
                if (widget->getWidgetHandle() == fromScreenHandle)
                {
                    fromScreen = (Screen*) widget;
                }
                else if (widget->getWidgetHandle() == toScreenHandle)
                {
                    toScreen = (Screen*) widget;
                }
            }

            if (mStack.size() > 0)
            {
                mStack.resize(mStack.size() - 1);
            }

            for (int i = 0; i < mStackScreenListeners.size(); i++)
            {
                mStackScreenListeners[i]->stackScreenScreenPopped(
                    this,
                    fromScreen,
                    toScreen);
            }
        }
    }

    /**
     * Add an stack screen event listener.
     * @param listener The listener that will receive stack screen events.
     */
    void StackScreen::addStackScreenListener(StackScreenListener* listener)
    {
        addListenerToVector(
            mStackScreenListeners,
            listener);
    }

    /**
     * Remove the stack screen listener.
     * @param listener The listener that receives stack screen events.
     */
    void StackScreen::removeStackScreenListener(StackScreenListener* listener)
    {
        removeListenerFromVector(
            mStackScreenListeners,
            listener);
    }

} // namespace NativeUI

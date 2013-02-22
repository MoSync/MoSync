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
 * @author Emma Tresanszki
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
     * Specify the screen transition type and duration used when pushing a screen.
     * This is available only Android and Windows Phone. On iOS the native
     * navigation animation is used when pushing a screen.
     *
     * @param screenTransitionType The type of the pop screen transition. See available
     * screen transitions types \link #MAW_TRANSITION_TYPE_NONE here \endlink.
     * @param screenTransitionDuration The duration of the screen transition in milliseconds.
     * This argument is not used on the Windows Phone platform due to the constant duration of
     * the WP screen transitions.
     *
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the show with transition operation was successful.
     * - #MAW_RES_INVALID_SCREEN_TRANSITION_TYPE if the screen transition type is not available
     * on current platform. Show operation is still completed without screen transition.
     * - #MAW_RES_INVALID_SCREEN_TRANSITION_DURATION if the screen transition is not a positive
     * integer. This error code is not returned on the Windows Phone platform
     * due to the constant duration of the WP screen transitions.
     */
    int StackScreen::setPushTransition(const MAWScreenTransitionType transitionType,
        const int transitionsDuration)
    {
        char valueBuffer[32];
        sprintf(valueBuffer, "%d", transitionType);
        int returnVal = setProperty(MAW_STACK_SCREEN_PUSH_TRANSITION_TYPE, valueBuffer);
        if ( MAW_RES_OK != returnVal )
        {
            return returnVal;
        }
        sprintf(valueBuffer, "%d", transitionsDuration);
        return setProperty(MAW_STACK_SCREEN_PUSH_TRANSITION_DURATION, valueBuffer);
    }

    /**
     * Specify the screen transition type and duration used when popping a screen.
     * This is available only Android and Windows Phone. On iOS the native
     * navigation animation is used when pushing a screen.
     *
     * @param screenTransitionType The type of the push screen transition. See available
     * screen transitions types \link #MAW_TRANSITION_TYPE_NONE here \endlink.
     * @param screenTransitionDuration The duration of the screen transition in milliseconds.
     * This argument is not used on the Windows Phone platform due to the constant duration of
     * the WP screen transitions.
     *
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the show with transition operation was successful.
     * - #MAW_RES_INVALID_SCREEN_TRANSITION_TYPE if the screen transition type is not available
     * on current platform. Show operation is still completed without screen transition.
     * - #MAW_RES_INVALID_SCREEN_TRANSITION_DURATION if the screen transition is not a positive
     * integer. This error code is not returned on the Windows Phone platform
     * due to the constant duration of the WP screen transitions.
     */
    int StackScreen::setPopTransition(const MAWScreenTransitionType transitionType,
        const int transitionsDuration)
    {
        char valueBuffer[32];
        sprintf(valueBuffer, "%d", transitionType);
        int returnVal = setProperty(MAW_STACK_SCREEN_POP_TRANSITION_TYPE, valueBuffer);
        if ( MAW_RES_OK != returnVal )
        {
            return returnVal;
        }
        sprintf(valueBuffer, "%d", transitionsDuration);
        return setProperty(MAW_STACK_SCREEN_POP_TRANSITION_DURATION, valueBuffer);
    }


    /**
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

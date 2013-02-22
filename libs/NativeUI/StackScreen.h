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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file StackScreen.h
 * @author Emma Tresanszki
 *
 * \brief A stack screen is a special type of screen that manages navigation
* between a set of screens.
 */

#ifndef NATIVEUI_STACK_SCREEN_H_
#define NATIVEUI_STACK_SCREEN_H_

#include <ma.h>

#include <MAUtil/Vector.h>

#include <IX_WIDGET.h>

#include "Screen.h"

namespace NativeUI
{
    // Forward declaration.
    class StackScreenListener;

    /**
     * \brief A stack screen is a special type of screen that manages navigation
     * between a set of screens.
     *
     * The screen stack can be pushed or popped. Pushing a given screen will
     * hide the current screen and display the pushed screen. Popping a screen
     * hides the current screen and shows the previous screen in the stack.
     * See push() and pop().
     * Navigation between the screens in the stack is handled according to the
     * norm on the specific platform. On iPhone a navigation bar is added to
     * each pushed screen that can be used to go back, while on Android there
     * is no extra UI added and the back button is used to go back to the
     * previous screen.
     */
    class StackScreen : public Screen
    {
    public:
        /**
         * Constructor.
         */
        StackScreen();

        /**
         * Destructor.
         */
        virtual ~StackScreen();

        /*
         * Get the total number of the screens in the stack.
         */
        virtual int getStackSize();

        /*
         * Push a screen inside the stack.
         * @param screen The new screen.
         */
        virtual void push(Screen* screen);

        /**
         * Pops a screen from a screen stack, hides the current screen and shows
         * the previous screen.
         * If there is no previous screen in the screen stack,
         * an empty screen will be shown.
         */
        virtual void pop();

        /**
         * Specify whether the back button automatically
         * should pop the stack screen.
         * This can have different behaviors on different platforms:
         * on iPhone the UI back button is disabled,
         * on Android the stack screen ignores back button events.
         * @param enabledState True if the back button is enabled, false otherwise.
         */
        virtual void enableBackButton(const bool enabledState);

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
        virtual int setPushTransition(const MAWScreenTransitionType transitionType,
            const int transitionsDuration);

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
        virtual int setPopTransition(const MAWScreenTransitionType transitionType,
            const int transitionsDuration);

        /**
         * Add an stack screen event listener.
         * @param listener The listener that will receive stack screen events.
         */
        virtual void addStackScreenListener(StackScreenListener* listener);

        /**
         * Remove the stack screen listener.
         * @param listener The listener that receives stack screen events.
         */
        virtual void removeStackScreenListener(StackScreenListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with stack screen listeners.
         */
        MAUtil::Vector<StackScreenListener*> mStackScreenListeners;

        /**
         * A Stack of Screens, the top of the stack must always be the screen
         * that is being shown.
         */
        MAUtil::Vector<Screen*> mStack;
    };

} // namespace NativeUI

#endif /* NATIVEUI_STACK_SCREEN_H_ */

/*! @} */

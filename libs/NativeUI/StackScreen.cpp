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
	 * Check if a stack screen is shown.
	 * @return true if the stack screen is visible, false otherwise.
	 */
	bool StackScreen::isShown()
	{
        MAUtil::String value = this->getPropertyString(MAW_STACK_SCREEN_IS_SHOWN);
        return (strcmp(value.c_str(), "true") == 0) ? true : false;
	}

    /**
     * Specify the screen transition type and duration used when pushing a screen.
     * This is available only on Android and Windows Phone. On iOS the native
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
     * This is available only on Android and Windows Phone. On iOS the native
     * navigation animation is used when popping a screen.
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
     * Get a child screen widget of this widget. Overrides 'Widget' class
     * 'getChild' method.
     * @param index The index of the child.
     * @return The child screen widget at the given index, or null if the
     * index is invalid.
     * The ownership of the result is not passed to the caller.
     */
    Widget* StackScreen::getChild(const int index) const
    {
        if (0 > index || index > mStack.size())
        {
            return NULL;
        }

        return mStack[index];
    }

    /**
     * Get a child screen widget of this widget.
     * @param index The index of the screen.
     * @return The screen widget at the given index.
     * The ownership of the result is not passed to the caller.
     */
    Screen* StackScreen::getScreen(const int index) const
    {
        if (0 > index || index > mStack.size())
        {
            return NULL;
        }

        return mStack[index];
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

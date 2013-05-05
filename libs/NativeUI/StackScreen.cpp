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

    void StackScreen::setBackButtonTintColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.8X", color);
        setProperty(MAW_STACK_SCREEN_BACK_BUTTON_COLOR, buffer);
    }

    void StackScreen::setBackButtonTintColor(const int red, const int blue, const int green, const int alpha)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.2X%.2X%.2X%.2X", alpha, red, green, blue);
        setProperty(MAW_STACK_SCREEN_BACK_BUTTON_COLOR, buffer);
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
     * Set the typeface and style in which the title should be displayed.
     * @param fontHandle A font handle received from loading fonts using
     *  #maFontGetName and #maFontLoadWithName syscalls.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     * @par Example
     * \code
     *   int nrFonts = maFontGetCount();
     *    char buf[256];
     *    // Get first font name.
     *    maFontGetName(1, buf, 256);
     *    // Load that font for size 10.
     *    int fontHandle = maFontLoadWithName(buf, 10);
     *    // Set that font to the stack screen.
     *	  maWidgetSetProperty(
     *      stackScreenHandle, MAW_STACK_SCREEN_TITLE_FONT_HANDLE, toString(fontHandle));
     * \endcode
     */
    int StackScreen::setTitleFont(const MAHandle fontHandle)
    {
        return this->setPropertyInt(MAW_STACK_SCREEN_TITLE_FONT_HANDLE, fontHandle);
    }

    int StackScreen::setTitleBackgroundImage(const MAHandle imageHandle)
    {
        return this->setPropertyInt(MAW_STACK_SCREEN_TITLE_BACKGROUND_IMAGE_HANDLE, imageHandle);
    }

    void StackScreen::setTitleBackgroundScaleMode(const TitleBackgroundScaleMode scaleMode)
    {
        MAUtil::String scaleModeString;
        switch (scaleMode)
        {
            case TITLE_BACKGROUND_SCALE_REPEAT_XY:
                scaleModeString = "scaleAndRepeatXY";
                break;
            case TITLE_BACKGROUND_SCALE_NONE:
            default:
                scaleModeString = "none";
                break;
        }

        this->setProperty(MAW_STACK_SCREEN_TITLE_BACKGROUND_SCALE_MODE, scaleModeString);
    }

    void StackScreen::showToolbar(const bool visibleState)
    {
        this->setProperty(MAW_STACK_SCREEN_TOOLBAR_VISIBLE, visibleState ? "true" : "false");
    }

    int StackScreen::setToolbarBackgroundImage(const MAHandle imageHandle)
    {
        return this->setPropertyInt(MAW_STACK_SCREEN_TOOLBAR_BACKGROUND_IMAGE_HANDLE, imageHandle);
    }

    void StackScreen::setToolbarBackgroundScaleMode(const TitleBackgroundScaleMode scaleMode)
    {
        MAUtil::String scaleModeString;
        switch (scaleMode)
        {
            case TITLE_BACKGROUND_SCALE_REPEAT_XY:
                scaleModeString = "scaleAndRepeatXY";
                break;
            case TITLE_BACKGROUND_SCALE_NONE:
            default:
                scaleModeString = "none";
                break;
        }

        this->setProperty(MAW_STACK_SCREEN_TOOLBAR_BACKGROUND_SCALE_MODE, scaleModeString);
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

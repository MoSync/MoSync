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
 * @file PlayScreen.h
 *
 * This file contains the main screen of the game.
 *
 * @author Emma Tresanszki
 */

#ifndef PLAYSCREEN_H_
#define PLAYSCREEN_H_

// Include MoSync NativeUI API.
#include <IX_WIDGET.h>

// Include the library for event handling listener.
#include "MAUtil/Environment.h"

// Include header file for the game engine class.
#include "Game.h"

namespace RockPaperScissors
{

// Colors used for fonts & background.
enum Colors
{
	// This is the default background color.
	SEA_GREEN = 0xC1FFC1 ,
	// If user losses, background color is changed to grey.
	LIGHT_GRAY = 0xD9D9D9,
	// If user wins, background color is changed to pink.
	HOT_PINK = 0xFF6EB4 ,
	DARK_GREY = 0x333333 ,
	LIGHT_RED = 0xEE4000 ,
	DARK_GREEN = 0x2F4F2F
};

// The sizes of the available images.
// NOTE: On small screens, images are scaled automatically
// by the parent widget.
enum WidgetSize
{
	BREAKLINE_HEIGHT     = 2 ,
	PADDING_SMALL        = 5 ,
	PADDING              = 10 ,
	/** There are two font sizes used:
	 * a rather big font for the win state and the score,
	 * and a smaller one for other widgets.\
	 * These are set depending to the screen size. */
	FONT_TITLE_SMALL     = 15 ,
	FONT_REGULAR_SMALL   = 13 ,
	FONT_TITLE_LARGE     = 20 ,
	FONT_REGULAR_LARGE   = 18
};

/** Messages for the UI screen. */
const MAUtil::String MESSAGE_CHOOSE_WEAPON = " Choose your WEAPON! ";
const MAUtil::String MESSAGE_GO            = " GO! ";

/*
 * PlayScreen: The Native UI screen. Contains vertical and horizontal
 * layouts with widgets for user selection, as well as for the game results.
 * When user wins or losses, background color changes.
 */
class PlayScreen :
	public MAUtil::CustomEventListener,
	public MAUtil::TimerListener
{
public:
	/**
	 * Constructor for the main screen class.
	 * Here instance variables are initialized.
	 */
	PlayScreen();

	/**
	 * The destructor deletes the main widget
	 * and all of its children.
	 */
	virtual ~PlayScreen();

	/**
	 * Show the screen with the game user interface.
	 * NOTE: If we had more screens, it could have been necessary
	 * to do some UI initialization here, before the screen is displayed.
	 */
	void showScreen();

	/**
	 * Handle widget events.
	 * @param event A MoSync event data structure.
	 */
	void customEvent(const MAEvent& event);

	/**
	 * Handle widget click events.
	 * @param widgetHandle The handle (reference) to the widget
	 * that was clicked.
	 */
	void widgetClicked(MAHandle widgetHandle);

	/**
	 * Compute result or the current turn based on user selection.
	 * @param selectedWeapon The weapon selected by the user.
	 */
	void computeGameResult(eWeapons selectedWeapon);

	/**
	 * Display the game results after a game round finishes.
	 */
	void displayGameResults();

	/**
	 * When starting over the game, initialize the weapon buttons.
	 */
	void initWeaponArea();

	/**
	 * Called on timer events.
	 * The timer callback that makes sure to update
	 * the clock every half second.
	 */
	void runTimerEvent();

	/**
	 * When user selects a weapon, timer can be stopped.
	 * If user does not select a weapon, timer is still running,
	 * and a warning message is displayed.
	 */
	void stopTimer();

	/**
	 * Utility function to create a label.
	 * The height of the label is computed automatically.
	 * @param width Label width.
	 * @param fontColor Text color.
	 * @param fontSize Text size.
	 * @param text Label text.
	 * @return Handle to the new label.
	 */
	MAWidgetHandle createLabel(
		int width,
		int fontColor,
		int fontSize,
		const char* text);

	/*
	 * Set a text for a label.
	 * @param aLabel Handle to the label.
	 * @param aText Label text.
	 */
	void setLabelText(MAWidgetHandle aLabel, MAUtil::String aText);

	/*
	 * Apply an int property for a widget.
	 * @param aWidget The handle for the widget.
	 * @param aProperty A string representing which property to set.
	 * @param aValue The value which will be assigned to the property.
	 * @param base  The output radix. Default is 10, 16 is to be used
	 * for color properties.
	 *
	 * * \returns Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int setWidgetProperty(
		MAWidgetHandle aWidget,
		const char* aProperty,
		int aValue,
		int base=10);

	/**
	 * Creates a button with centered text by default,
	 * font size is 20, and color is DARK GREY.
	 * @param text The button text.
	 * @return Handle to the new button.
	 */
	MAWidgetHandle createButton(const char* text);

	/**
	 * Utility function to create an image widget with the given resource.
	 * @param imageResource The image resource handle to be used for the widget.
	 * @param width Widget width.
	 * @param height Widget height.
	 * @return Handle to the new image widget.
	 */
	MAWidgetHandle createImageWidget(
		MAHandle imageResource,
		int width,
		int height);

	/**
	 * Utility function to create image button.
	 * @param width Button width.
	 * @param height Button height.
	 * @return Handle to the new image button.
	 */
	MAWidgetHandle createImageButton(
		MAHandle imageResource,
		int width,
		int height);

	/**
	 * Utility function to create a vertical empty "spacer".
	 * @param width Spacer width.
	 * @param height Spacer height.
	 * @return Handle to the new spacer.
	 */
	MAWidgetHandle createSpacer(int width, int height);

	/**
	 * Utility function to create the layout
	 * with 5 buttons for user selection.
	 */
	MAWidgetHandle createWeaponSelectionLayout();

	/*
	 * Sets the font size and the padding size depending on the screen size.
	 */
	void setSizes();

private: // methods
	/**
	 * Create all the widgets in the UI.
	 * The start button, count down label, flickering label,
	 * weapon buttons, rival weapon, spacer, score label,
	 * exit button.
	 */
	MAWidgetHandle createMainLayout();

	/**
	 * Layout the widgets (portrait mode).
	 */
	void setupUI();

private: // members
	/** NativeUI widgets handles. */
	MAWidgetHandle mScreen;

	/** Main screen layout. */
	MAWidgetHandle mMainLayout;

	/**
	 * Label indicating the user to choose a weapon,
	 * or prompting the game explanation.
	 */
	MAWidgetHandle mFlickrLabel;

	/** Label indicating the win state. */
	MAWidgetHandle mWinStateLabel;

	/** Rock Weapon button. */
	MAWidgetHandle mRockButton;

	/** Paper Weapon button. */
	MAWidgetHandle mPaperButton;

	/** Scissors Weapon button. */
	MAWidgetHandle mScissorsButton;

	/** The selected weapon. */
	MAWidgetHandle mMyWeapon;

	/** Rival's weapon: image. */
	MAWidgetHandle mRivalWeapon;

	/** Start button. */
	MAWidgetHandle mStartButton;

	/** Score label. */
	MAWidgetHandle mScoreInfo;

	/** Exit button. */
	MAWidgetHandle mExitButton;

	/** Screen size. */
	int mScreenWidth;
	int mScreenHeight;

	/** The font size for big widgets like: win state and score. */
	int mFontTitleSize;

	/** The font size for smaller widgets. */
	int mFontRegularSize;

	/** The padding size. **/
	int mPaddingSize;

	/** Game engine contains the game logic. */
	Game mGame;

	/** Flag that tells if the timer is started. */
	bool mTimerStarted;

	/** Timer elapsed milliseconds. */
	int mTimerElapsedMilliSeconds;
};

} // namespace RockPaperScissors

#endif // PLAYSCREEN_H_

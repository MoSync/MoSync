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
 * @file PlayScreen.cpp
 *
 * This file contains the main screen of the game.
 *
 * @author Emma Tresanszki
 */

// Include library for string conversions.
#include <mastdlib.h>

// Include util class for integer to string conversions.
// Include utility class for integer-to-string conversions.
#include <MAUtil/util.h>

#include <maprofile.h>
#include <ma.h>
#include <mastring.h>

// Include the resources for images.
#include "MAHeaders.h"

// Include application's Moblet class.
#include "Moblet.h"

// Include the game engine class.
#include "Game.h"

// Include the game screen.
#include "PlayScreen.h"

namespace RockPaperScissors
{

/**
 * Constructor for the main screen class.
 * Here instance variables are initialized.
 */
PlayScreen::PlayScreen() :
	mScreen(-1),
	mMainLayout(-1),
	mFlickrLabel(-1),
	mWinStateLabel(-1),
	mRockButton(-1),
	mPaperButton(-1),
	mScissorsButton(-1),
	mRivalWeapon(-1),
	mStartButton(-1),
	mScoreInfo(-1),
	mExitButton(-1),
	mScreenWidth(-1),
	mScreenHeight(-1),
	mFontTitleSize(-1),
	mFontRegularSize(-1),
	mPaddingSize(-1),
	mTimerStarted(false),
	mTimerElapsedMilliSeconds(0)
{
	// Initialize the native UI widgets.
	// NOTE: There is no scaling required, because that is done
	// automatically by the platform at widget creation.
	setupUI();

	// Make the play screen listen for widget events.
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * The destructor deletes the main widget
 * and all of its children.
 */
PlayScreen::~PlayScreen()
{
	// Delete the main widget, also deletes child widgets.
	maWidgetDestroy(mMainLayout);

	// If game is playing, stop the timer.
	if (mTimerStarted)
	{
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}
}

/**
 * Show the screen with the game user interface.
 * NOTE: If we had more screens, it could have been necessary
 * to do some UI initialization here, before the screen is displayed.
 */
void PlayScreen::showScreen()
{
	maWidgetScreenShow(mScreen);
}

/**
 * Handle widget events.
 * @param event A MoSync event data structure.
 */
void PlayScreen::customEvent(const MAEvent& event)
{
	// If the event does not come from a widget, we just ignore it.
	if(event.type != EVENT_TYPE_WIDGET)
	{
		return;
	}

	// Get the information sent by the widget.
	MAWidgetEventData* widgetEventData = (MAWidgetEventData*) event.data;

	// Check that the event was a click (touch) event.
	if (widgetEventData->eventType == MAW_EVENT_CLICKED)
	{
		// Handle the event emitted by the widget
		widgetClicked(widgetEventData->widgetHandle);
	}
}

/**
 * Handle widget click events.
 * @param widgetHandle The handle (reference) to the widget
 * that was clicked.
 */
void PlayScreen::widgetClicked(MAHandle widgetHandle)
{
	// Exit the application when the exit button is clicked.
	if (widgetHandle == mExitButton)
	{
		RockPaperScissorsMoblet::getInstance()->closeEvent();
	}
	// Start a game round if start button is clicked.
	else if (widgetHandle == mStartButton && !mTimerStarted)
	{
		// Restore the background color to default.
		setWidgetProperty(
			mMainLayout, MAW_WIDGET_BACKGROUND_COLOR, SEA_GREEN, 16);

		// Show all weapons.
		initWeaponArea();

		// Start playing the game.
		setLabelText(mFlickrLabel, MESSAGE_CHOOSE_WEAPON.c_str());

		// Start a timer that notifies this class at every half second.
		// Now, the user can pick a weapon, and the flickering label indicates this.
		mTimerElapsedMilliSeconds = 0;
		mTimerStarted = true;
		MAUtil::Environment::getEnvironment().addTimer(this, 500, -1);

		// Tell the user that it is time to pick a weapon.
		setLabelText(mWinStateLabel, MESSAGE_GO.c_str());
		maWidgetSetProperty(mWinStateLabel, MAW_WIDGET_VISIBLE, "true");

		// Hide START button.
		maWidgetSetProperty(mStartButton, MAW_WIDGET_VISIBLE, "false");
	}
	// If the timer is started, the user can pick a weapon.
	else if ((widgetHandle == mRockButton
		|| widgetHandle == mPaperButton
		|| widgetHandle == mScissorsButton)
		&&
		mTimerStarted )
	{
		// Stop timer. Now the game results can be computed.
		stopTimer();

		// If weapon was chosen, compute the results.
		if (widgetHandle == mRockButton)
		{
			computeGameResult(ROCK);
		}
		else if (widgetHandle == mPaperButton)
		{
			computeGameResult(PAPER);
		}
		else if (widgetHandle == mScissorsButton)
		{
			computeGameResult(SCISSORS);
		}
	}
}

/**
 * Compute result or the current turn based on user selection.
 * @param selectedWeapon The weapon selected by the user.
 */
void PlayScreen::computeGameResult(eWeapons selectedWeapon)
{
	// Hide the weapons that aren't selected.
	if (ROCK != selectedWeapon)
	{
		maWidgetSetProperty(mRockButton, MAW_WIDGET_VISIBLE, "false");
	}

	if (PAPER != selectedWeapon)
	{
		maWidgetSetProperty(mPaperButton, MAW_WIDGET_VISIBLE, "false");
	}

	if (SCISSORS != selectedWeapon)
	{
		maWidgetSetProperty(mScissorsButton, MAW_WIDGET_VISIBLE, "false");
	}

	// Set user & rival weapons
	mGame.setMyWeapon(selectedWeapon);
	mGame.setRandomOpponentWeapon();

	// Display the opponent weapon.
	eWeapons opponentWeapon = mGame.getOpponentWeapon();
	int weaponResource(RES_IMAGE_ROCK);
	switch (opponentWeapon)
	{
		case 1:
			weaponResource = RES_IMAGE_ROCK;
			break;
		case 2:
			weaponResource = RES_IMAGE_PAPER;
			break;
		case 3:
			weaponResource = RES_IMAGE_SCISSORS;
			break;
	}
	// This sets the image.
	setWidgetProperty(
		mRivalWeapon, MAW_IMAGE_BUTTON_BACKGROUND_IMAGE, weaponResource);

	// Compute the game result.
	mGame.computeGameResult();

	// Finally, display the results
	displayGameResults();
}

/**
 * Display the game results after a game round finishes.
 */
void PlayScreen::displayGameResults()
{
	// Change background color to pink if user wins, or in misty otherwise.
	// Set to green color by default, that is also applied in case of TIE.
	int color(SEA_GREEN);
	if (mGame.getWinState() == YOU_WIN)
	{
		color = HOT_PINK;
	}
	else if (mGame.getWinState() == YOU_LOSE)
	{
		color = LIGHT_GRAY;
	}
	setWidgetProperty(mMainLayout, MAW_WIDGET_BACKGROUND_COLOR, color, 16);

	// Display game status.
	maWidgetSetProperty(mFlickrLabel, MAW_WIDGET_VISIBLE, "true");

	// Display who has win, and why.
	setLabelText(mWinStateLabel, mGame.getWinState().c_str());
	setLabelText(mFlickrLabel, mGame.getExplanation().c_str());

	// Display score info.
	MAUtil::String score;
	score = "Win: ";
	score += MAUtil::integerToString(mGame.getWins());
	score += " Tie: ";
	score += MAUtil::integerToString(mGame.getTies());
	score += " Loss: ";
	score += MAUtil::integerToString(mGame.getLosesNr());
	setLabelText(mScoreInfo, score.c_str());

	// Make the start button visible again.
	maWidgetSetProperty(mStartButton, MAW_WIDGET_VISIBLE, "true");
}

/**
 * When starting over the game, initialize the weapon buttons.
 */
void PlayScreen::initWeaponArea()
{
	// Show all weapons
	maWidgetSetProperty(mRockButton, MAW_WIDGET_VISIBLE, "true");
	maWidgetSetProperty(mPaperButton, MAW_WIDGET_VISIBLE, "true");
	maWidgetSetProperty(mScissorsButton, MAW_WIDGET_VISIBLE, "true");

	// Rival's weapon is unknown, display question mark icon.
	setWidgetProperty(
		mRivalWeapon,
		MAW_IMAGE_BUTTON_BACKGROUND_IMAGE,
		RES_IMAGE_QUESTION_MARK);
}

/**
 * Called on timer events.
 * The timer callback that makes sure to update
 * the clock every half second.
 */
void PlayScreen::runTimerEvent()
{
	mTimerElapsedMilliSeconds += 500;

	// Label flicker at each half second.
	div_t divide = div(mTimerElapsedMilliSeconds, 1000);
	if (divide.rem == 500)
	{
		maWidgetSetProperty(mFlickrLabel, MAW_WIDGET_VISIBLE, "false");
	}
	else
	{
		maWidgetSetProperty(mFlickrLabel, MAW_WIDGET_VISIBLE, "true");
	}
}

/**
 * When user selects a weapon, timer can be stopped.
 */
void PlayScreen::stopTimer()
{
	if (mTimerStarted)
	{
		MAUtil::Environment::getEnvironment().removeTimer(this);
		mTimerElapsedMilliSeconds = 0;
		// Initialize timer state to not started.
		mTimerStarted = false;
	}
}

/**
 * Utility function to create a label.
 * The height of the label is computed automatically.
 * @param width Label width.
 * @param fontColor Text color.
 * @param fontSize Text size.
 * @param text Label text.
 * @return Handle to the new label.
 */
MAWidgetHandle PlayScreen::createLabel(
	int width,
	int fontColor,
	int fontSize,
	const char* text)
{
	// Create the label.
	MAWidgetHandle label = maWidgetCreate(MAW_LABEL);

	// Set the label font color.
	setWidgetProperty(label, MAW_LABEL_FONT_COLOR, fontColor, 16);

	// Set the label font size.
	setWidgetProperty(label, MAW_LABEL_FONT_SIZE, fontSize);

	// Set the widget size.
	setWidgetProperty(label, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(label, MAW_WIDGET_HEIGHT, MAW_CONSTANT_WRAP_CONTENT);

	// Set the label text.
	maWidgetSetProperty(label, MAW_LABEL_TEXT, text);

	// Set text alignment.
	maWidgetSetProperty(label, MAW_LABEL_TEXT_VERTICAL_ALIGNMENT, "center");
	maWidgetSetProperty(label, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, "center");

	return label;
}

/*
 * Set a text for a label.
 * @param aLabel Handle to the label.
 * @param aText Label text.
 */
void PlayScreen::setLabelText(MAWidgetHandle aLabel, MAUtil::String aText)
{
	maWidgetSetProperty(aLabel,MAW_LABEL_TEXT, aText.c_str());
}

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
int PlayScreen::setWidgetProperty(
	MAWidgetHandle aWidget,
	const char* aProperty,
	int aValue,
	int base)
{
	// Buffer for property values.
	char buf[10];

	itoa( aValue, buf, base);
	// Apply the property to the widget.
	return maWidgetSetProperty(aWidget, aProperty, buf);
}

/**
 * Creates a button with centered text by default,
 * font size is 20, and color is DARK GREY.
 * @param text The button text.
 * @return Handle to the new button.
 */
MAWidgetHandle PlayScreen::createButton(const char* text)
{
	// Create button.
	MAWidgetHandle button = maWidgetCreate(MAW_BUTTON);

	// Set text and text alignment.
	maWidgetSetProperty(
		button, MAW_BUTTON_TEXT, text);
	maWidgetSetProperty(
		button, MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT, "center");
	maWidgetSetProperty(
		button, MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT, "center");

	// Set button width (height is set automatically).
	setWidgetProperty(button, MAW_WIDGET_LEFT, mScreenWidth/2);

	// Set font color.
	setWidgetProperty(button, MAW_BUTTON_FONT_COLOR, DARK_GREY, 16);

	// Set font size to small.
//	Not used: setWidgetProperty(button, MAW_BUTTON_FONT_SIZE, mFontRegularSize);

	return button;
}

/**
 * Utility function to create an image widget with the given resource.
 * @param imageResource The image resource handle to be used for the widget.
 * @param width Widget width.
 * @param height Widget height.
 * @return Handle to the new image widget.
 */
MAWidgetHandle PlayScreen::createImageWidget(
	MAHandle imageResource,
	int width,
	int height)
{
	MAWidgetHandle image = maWidgetCreate(MAW_IMAGE);

	// Set the image.
	setWidgetProperty(image,MAW_IMAGE_IMAGE, imageResource);

	// Set the scale mode to not scaling the image.
	maWidgetSetProperty(image, "scaleMode", "scalePreserveAspect");

	// Set the widget sizes.
	setWidgetProperty(image, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(image, MAW_WIDGET_HEIGHT, height);

	return image;
}

/**
 * Utility function to create image button.
 * @param width Button width.
 * @param height Button height.
 * @return Handle to the new image button.
 */
MAWidgetHandle PlayScreen::createImageButton(
	MAHandle imageResource,
	int width,
	int height)
{
	MAWidgetHandle imageButton = maWidgetCreate(MAW_IMAGE_BUTTON);

	// Set the background image.
	setWidgetProperty(
		imageButton,
		MAW_IMAGE_BUTTON_BACKGROUND_IMAGE,
		imageResource);

	// Set the scale mode to not scaling the image.
	maWidgetSetProperty(imageButton, "scaleMode", "scalePreserveAspect");

	// Set the widget size.
	setWidgetProperty(imageButton, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(imageButton, MAW_WIDGET_HEIGHT, height);

	maWidgetSetProperty(
		imageButton,
		MAW_IMAGE_BUTTON_TEXT_VERTICAL_ALIGNMENT,
		"center");
	maWidgetSetProperty(
		imageButton,
		MAW_IMAGE_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
		"center");

	return imageButton;
}

/**
 * Utility function to create a vertical empty "spacer".
 * @param width Spacer width.
 * @param height Spacer height.
 * @return Handle to the new spacer.
 */
MAWidgetHandle PlayScreen::createSpacer(int width, int height)
{
	MAWidgetHandle spacer = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Set the widget size.
	setWidgetProperty(spacer, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(spacer, MAW_WIDGET_HEIGHT, height);

	return spacer;
}

/**
 * Layout the widgets (portrait mode).
 */
void PlayScreen::setupUI()
{
	// Create the main screen
	mScreen = maWidgetCreate(MAW_SCREEN);

	// Error handling for devices that do not support NativeUI.
	if ( -1 == mScreen )
	{
		maPanic(0, " This application uses NativeUI, which currently is supported"
				   " only on Android and iOS devices.");
	}

	// Get the screen size.
	if (NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
	{
		MAExtent screenSize = maGetScrSize();
		mScreenWidth = EXTENT_X(screenSize);
		mScreenHeight = EXTENT_Y(screenSize);
	}
	else
	{
		char width[10];
		char height[10];
		maWidgetGetProperty(mScreen, MAW_WIDGET_WIDTH, width, 10);
		maWidgetGetProperty(mScreen, MAW_WIDGET_HEIGHT, height, 10);
		mScreenWidth = MAUtil::stringToInteger(width,10);
		mScreenHeight = MAUtil::stringToInteger(height,10);
	}

	// Set the necessary size for fonts and padding.
	setSizes();

	// Create the main layout.
	mMainLayout = createMainLayout();

	// Add equal spacers before and after the Start button,
	// and before and after score label (so that main play area
	// is centered).
	// The height of the spacer is set to MAW_CONSTANT_FILL_AVAILABLE_SPACE,
	// so that the spacer will fill the remaining space.
	maWidgetAddChild(
		mMainLayout,
		createSpacer(mScreenWidth, mScreenHeight / 40));

	// Add a horizontal line, then the score.
	MAWidgetHandle line = createSpacer(mScreenWidth, BREAKLINE_HEIGHT);

	// Set the background color for this spacer.
	setWidgetProperty(line, MAW_WIDGET_BACKGROUND_COLOR, DARK_GREEN, 16);

	maWidgetAddChild(mMainLayout, line);

	// Add a score label with a big font.
	mScoreInfo = createLabel(
		mScreenWidth,
		DARK_GREY,
		mFontTitleSize,
		"");
	// Set the default text to the score label.
	MAUtil::String defaultText = "Win:0 Tie:0 Loss:0";
	setLabelText(mScoreInfo, defaultText.c_str());
	maWidgetAddChild(mMainLayout, mScoreInfo);

	// Exit button in a horizontal layout.
	MAWidgetHandle bottomMenu = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);

	// Set the size for this layout;
	setWidgetProperty(
		bottomMenu, MAW_WIDGET_WIDTH, mScreenWidth);
	setWidgetProperty(
		bottomMenu, MAW_WIDGET_HEIGHT, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	// Arrange it's children vertically on bottom
	maWidgetSetProperty(bottomMenu,
			MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_BOTTOM);

	// Arrange the children widgets horizontally  at right.
	maWidgetSetProperty(
		bottomMenu,
		MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_RIGHT);

	// The left menu is empty now, but in the future it
	// could be Options for instance.
	maWidgetAddChild(
		bottomMenu,
		createLabel(MAW_CONSTANT_WRAP_CONTENT, 0, 0, ""));

	// Create exit button.
	mExitButton = createButton("  Exit  " );
	maWidgetAddChild(bottomMenu, mExitButton);
	maWidgetAddChild(mMainLayout, bottomMenu);

	// Add the main layout to the screen
	maWidgetAddChild(mScreen, mMainLayout);
}

/**
 * Create all the widgets in the UI.
 * The start button, count down label, flickering label,
 * weapon buttons, rival weapon, spacer, score label,
 * exit button.
 */
MAWidgetHandle PlayScreen::createMainLayout()
{
	// Create the main layout (the main widget that holds the other widgets).
	MAWidgetHandle mainLayout = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Set the size for this widget.
	setWidgetProperty(mainLayout, MAW_WIDGET_WIDTH, mScreenWidth);
	setWidgetProperty(mainLayout, MAW_WIDGET_HEIGHT, mScreenHeight);

	// Set the margins for the main layout.
	// NOTE: MAW_WIDGET_LEFT applies only to Android for the moment.
	// This property is applied here for example purposes.
	// The approx. same output can be created by applying the
	// MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT
	// property on the parent widget.
	setWidgetProperty(mainLayout, MAW_WIDGET_LEFT, mPaddingSize);
	setWidgetProperty(mainLayout, MAW_WIDGET_TOP, mPaddingSize);

	// All the child widgets of the mainLayout will be centered.
	maWidgetSetProperty(
		mainLayout,
		MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		mainLayout,
		MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Set a background color for the main layout.
	setWidgetProperty(mainLayout, MAW_WIDGET_BACKGROUND_COLOR, SEA_GREEN, 16);

	// Add some space before the start button.
	maWidgetAddChild(
		mainLayout,
		createSpacer(mScreenWidth, mScreenHeight / 40));

	// Create the start button, centered.
	// The image is round, so use equal width and height
	// (they are relatively small).
	mStartButton = createImageButton(
		RES_IMAGE_START,
		mScreenWidth / 5,
		mScreenWidth / 5);
	maWidgetAddChild(mainLayout, mStartButton);

	// Add spacer between START button and weapons area.
	maWidgetAddChild(
		mainLayout,
		createSpacer(mScreenWidth, mScreenHeight / 40));

	// MAW_CONSTANT_WRAP_CONTENT makes the label height is set so that it fits the available space.
	mWinStateLabel = createLabel(
		mScreenWidth,
		LIGHT_RED,
		mFontTitleSize, //  Set a big size, because this info is important.
		" ");
	maWidgetSetProperty(mWinStateLabel, MAW_WIDGET_VISIBLE, "false");
	maWidgetAddChild(mainLayout, mWinStateLabel);

	// After start is pressed, Choose your weapon! The label flickers.
	mFlickrLabel = createLabel(
		mScreenWidth,
		DARK_GREY,
		mFontRegularSize,
		"Press PLAY to start game");
	maWidgetAddChild(mainLayout, mFlickrLabel);

	// Add the layout that contains the buttons
	maWidgetAddChild(mainLayout, createWeaponSelectionLayout() );

	// The rival weapon area
	maWidgetAddChild(
		mainLayout,
		createLabel(
			mScreenWidth,
			DARK_GREY,
			mFontTitleSize,
			" VS."));

	// Set this image as big as the start button.
	mRivalWeapon = createImageButton(
		RES_IMAGE_QUESTION_MARK,
		mScreenWidth / 5,
		mScreenWidth / 5);
	maWidgetAddChild(mainLayout, mRivalWeapon);

	return mainLayout;
}

/**
 * Utility function to create the layout
 * with 5 buttons for user selection.
 */
MAWidgetHandle PlayScreen::createWeaponSelectionLayout()
{
	MAWidgetHandle buttonsLayout = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);

	// Set the size for this layout.
	setWidgetProperty(buttonsLayout, MAW_WIDGET_HEIGHT, mScreenWidth / 3);
	setWidgetProperty(buttonsLayout, MAW_WIDGET_WIDTH, mScreenWidth);

	// These properties allow you to center the widgets that are
	// inside the parent widget.
	maWidgetSetProperty(
		buttonsLayout,
		MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		buttonsLayout,
		MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Arrange 3 buttons horizontally.
	// Align each button centered in a layout, so that they will
	// be equally positioned.

	// The first layout, for the Rock weapon.
	MAWidgetHandle layoutRock = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Layout's height equals the height of the parent layout.
	setWidgetProperty(layoutRock, MAW_WIDGET_HEIGHT, mScreenWidth / 3);
	setWidgetProperty(layoutRock, MAW_WIDGET_WIDTH, mScreenWidth / 3);

	// The first button: for the Rock weapon.
	mRockButton = createImageButton(
		RES_IMAGE_ROCK,
		mScreenWidth / 3,
		mScreenWidth / 3);
	maWidgetAddChild(layoutRock, mRockButton);

	// The second layout, for the Paper weapon.
	MAWidgetHandle layoutPaper = maWidgetCreate(MAW_VERTICAL_LAYOUT);
	setWidgetProperty(layoutPaper, MAW_WIDGET_HEIGHT, mScreenWidth / 3);
	setWidgetProperty(layoutPaper, MAW_WIDGET_WIDTH, mScreenWidth / 3);

	// The second button: for the Paper weapon.
	mPaperButton = createImageButton(
		RES_IMAGE_PAPER,
		mScreenWidth / 3,
		mScreenWidth / 3);
	maWidgetAddChild(layoutPaper, mPaperButton);

	// The third layout, for the Scissors weapon.
	MAWidgetHandle layoutScissors = maWidgetCreate(MAW_VERTICAL_LAYOUT);
	setWidgetProperty(layoutScissors, MAW_WIDGET_HEIGHT, mScreenWidth / 3);
	setWidgetProperty(layoutScissors, MAW_WIDGET_WIDTH, mScreenWidth / 3);

	// The third button: for the Scissors weapon.
	mScissorsButton = createImageButton(
		RES_IMAGE_SCISSORS,
		mScreenWidth / 3,
		mScreenWidth / 3);
	maWidgetAddChild(layoutScissors, mScissorsButton);

	// Add the three layouts in a horizontal layout.
	maWidgetAddChild(buttonsLayout, layoutRock);
	maWidgetAddChild(buttonsLayout, layoutPaper);
	maWidgetAddChild(buttonsLayout, layoutScissors);

	return buttonsLayout;
}

/*
 * Sets the font size and the padding size depending on the screen size.
 */
void PlayScreen::setSizes()
{
	// For small screens, use small fonts and padding.
	if (mScreenHeight < 600 )
	{
		// Set small font sizes.
		mFontTitleSize = FONT_TITLE_SMALL;
		mFontRegularSize = FONT_REGULAR_SMALL;
		mPaddingSize = PADDING_SMALL;
	}
	else
	{
		// Use a bigger font size. Used for important widgets,
		// like win state and score.
		mFontTitleSize = FONT_TITLE_LARGE;
		mFontRegularSize = FONT_REGULAR_LARGE;
		mPaddingSize = PADDING;
	}
}

} // namespace RockPaperScissors


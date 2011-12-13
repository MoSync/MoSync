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
 * @file SettingsScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying widgets for capture syscalls.
 * Contains widgets(e.g. check box, button) used for setting and getting
 * values for taking photos or recording video.
 */

#define SCREEN_TITLE "Settings"

#define TAKE_PICTURE_BTN_TEXT "Take picture"
#define RECORD_VIDEO_BTN_TEXT "Record video"
#define VIDEO_MAX_DURATION_TEXT "Video max duration:"
#define MAX_DURATION_BTN_TEXT "Get max duration"
#define MAX_DURATION_LABEL_TEXT "Duration:"
#define SET_VIDEO_QUALITY_BTN_TEXT "Set video quality"
#define GET_VIDEO_QUALITY_BTN_TEXT "Get video quality"
#define VIDEO_QUALITY_LABEL_TEXT "Quality:"
#define SET_FLASH_MODE_BTN_TEXT "Set flash mode"
#define GET_FLASH_MODE_BTN_TEXT "Get flash mode"
#define SHOW_IMAGE_SCREEN_TEXT "Show taken picture"
#define SHOW_VIDEO_SCREEN_TEXT "Play recorded video"
#define CAMERA_ROLL_LABEL_TEXT "Use camera roll"
#define CAMERA_CONTROLS_LABEL_TEXT "Use camera controls"

#define VIDEO_QUALITY_LOW_TEXT "low"
#define VIDEO_QUALITY_MEDIUM_TEXT "medium"
#define VIDEO_QUALITY_HIGH_TEXT "high"

#define FLASH_MODE_AUTO_TEXT "Flash mode auto"
#define FLASH_MODE_ON_TEXT "Flash mode on"
#define FLASH_MODE_OFF_TEXT "Flash mode off"

#define RED_COLOR 0xFF0000
#define BLACK_COLOR 0x000000

#include <conprint.h>

#include <MAUtil/String.h>

#include "SettingsScreen.h"
#include "Util.h"

	/**
	 * Constructor.
	 * @param listener Listener for screen.
	 */
SettingsScreen::SettingsScreen(SettingsScreenListener& listener) :
	Screen(),
	mListener(listener),
	mMaxDurationEditBox(NULL),
	mMaxDurationLabel(NULL),
	mGetMaxDurationBtn(NULL),
	mSetVideoQualityBtn(NULL),
	mGetVideoQualityBtn(NULL),
	mVideoQualityLabel(NULL),
	mSetFlashModeBtn(NULL),
	mGetFlashModeBtn(NULL),
	mFlashModeLabel(NULL),
	mCameraRollCheckBox(NULL),
	mCameraControlsCheckBox(NULL),
	mTakePictureBtn(NULL),
	mRecordVideoBtn(NULL),
	mShowImageScreen(NULL),
	mShowVideoScreen(NULL)
{
	this->setTitle(SCREEN_TITLE);
	this->createMainLayout();

	mMaxDurationEditBox->addEditBoxListener(this);
	mGetMaxDurationBtn->addButtonListener(this);
	mSetVideoQualityBtn->addButtonListener(this);
	mGetVideoQualityBtn->addButtonListener(this);
	mSetFlashModeBtn->addButtonListener(this);
	mGetFlashModeBtn->addButtonListener(this);
	mCameraRollCheckBox->addCheckBoxListener(this);
	mCameraControlsCheckBox->addCheckBoxListener(this);
	mTakePictureBtn->addButtonListener(this);
	mRecordVideoBtn->addButtonListener(this);
	mShowImageScreen->addButtonListener(this);
	mShowVideoScreen->addButtonListener(this);
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	mMaxDurationEditBox->removeEditBoxListener(this);
	mGetMaxDurationBtn->removeButtonListener(this);
	mSetVideoQualityBtn->removeButtonListener(this);
	mGetVideoQualityBtn->removeButtonListener(this);
	mSetFlashModeBtn->removeButtonListener(this);
	mGetFlashModeBtn->removeButtonListener(this);
	mCameraRollCheckBox->removeCheckBoxListener(this);
	mCameraControlsCheckBox->removeCheckBoxListener(this);
	mTakePictureBtn->removeButtonListener(this);
	mRecordVideoBtn->removeButtonListener(this);
	mShowImageScreen->removeButtonListener(this);
	mShowVideoScreen->removeButtonListener(this);
}

/**
 * Create a new row for settings.
 * It contains two widgets separated by a spacer.
 */
HorizontalLayout* SettingsScreen::createRow(
	Widget* firstWidget,
	Widget* secondWidget)
{
	HorizontalLayout* hLayout = new HorizontalLayout();
	if ( firstWidget != NULL )
	{
		hLayout->addChild(firstWidget);
		HorizontalLayout* space = new HorizontalLayout();
		space->setWidth(10);
		hLayout->addChild(space);
	}
	hLayout->addChild(secondWidget);
	return hLayout;
}

/**
 * Add a button to a list view.
 * @param button The button that will be added.
 * @param listView The list view that will contain the button.
 */
void SettingsScreen::addButtonToListView(Button* button, ListView* listView)
{
	ListViewItem* listItem = new ListViewItem();
	listItem->addChild(button);
	listView->addChild(listItem);
}

/**
 * Adds a list row that contains widgets for setting the current
 * maximum video duration.
 */
void SettingsScreen::addSetDurationRow(ListView* listView)
{
	ListViewItem* listItem;
	Label* label;

	listItem = new ListViewItem();
	listView->addChild(listItem);
	mMaxDurationEditBox = new EditBox();
	mMaxDurationEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
	mMaxDurationEditBox->fillSpaceHorizontally();

	label = new Label();
	label->setText(VIDEO_MAX_DURATION_TEXT);
	listItem->addChild(this->createRow(label, mMaxDurationEditBox));
}

/**
 * Adds a list row that contains widgets for getting the current
 * maximum video duration.
 */
void SettingsScreen::addGetDurationRow(ListView* listView)
{
	ListViewItem* listItem = new ListViewItem();
	listView->addChild(listItem);

	mMaxDurationLabel = new Label();
	mMaxDurationLabel->setText(MAX_DURATION_LABEL_TEXT);
	mMaxDurationLabel->fillSpaceHorizontally();

	mGetMaxDurationBtn = new Button();
	mGetMaxDurationBtn->setText(MAX_DURATION_BTN_TEXT);
	listItem->addChild(this->createRow(mGetMaxDurationBtn, mMaxDurationLabel));
}

/**
 * Adds two list rows for setting and getting the video quality value.
 */
void SettingsScreen::addVideoQualityRows(ListView* listView)
{
	ListViewItem* listItem;

	// Add set option for video quality value.
	listItem = new ListViewItem();
	listView->addChild(listItem);
	mSetVideoQualityBtn = new Button();
	mSetVideoQualityBtn->setText(SET_VIDEO_QUALITY_BTN_TEXT);
	listItem->addChild(mSetVideoQualityBtn);

	// Add get option for video quality value.
	listItem = new ListViewItem();
	listView->addChild(listItem);
	mGetVideoQualityBtn = new Button();
	mGetVideoQualityBtn->setText(GET_VIDEO_QUALITY_BTN_TEXT);
	mVideoQualityLabel = new Label();
	mVideoQualityLabel->setText(VIDEO_QUALITY_LABEL_TEXT);
	mVideoQualityLabel->fillSpaceHorizontally();
	listItem->addChild(this->createRow(mGetVideoQualityBtn, mVideoQualityLabel));
}

/**
 * Adds two list rows for setting and getting the flash mode value.
 */
void SettingsScreen::addFlashModeRows(ListView* listView)
{
	// Add set option for flash mode value.
	ListViewItem* listItem = new ListViewItem();
	listView->addChild(listItem);
	mSetFlashModeBtn = new Button();
	mSetFlashModeBtn->setText(SET_FLASH_MODE_BTN_TEXT);
	listItem->addChild(mSetFlashModeBtn);

	// Add get option for flash mode value.
	listItem = new ListViewItem();
	listView->addChild(listItem);
	mGetFlashModeBtn = new Button();
	mGetFlashModeBtn->setText(GET_FLASH_MODE_BTN_TEXT);
	mFlashModeLabel = new Label();
	mFlashModeLabel->fillSpaceHorizontally();
	listItem->addChild(this->createRow(mGetFlashModeBtn, mFlashModeLabel));
}

/**
 * Adds a list row for setting the camera roll flag.
 */
void SettingsScreen::addCameraRollFlagRow(ListView* listView)
{
	ListViewItem* listItem;
	Label* label;

	listItem = new ListViewItem();
	listView->addChild(listItem);

	label = new Label();
	label->setText(CAMERA_ROLL_LABEL_TEXT);

	mCameraRollCheckBox = new CheckBox();
	mCameraRollCheckBox->setState(true);
	listItem->addChild(this->createRow(label, mCameraRollCheckBox));
	listView->addChild(label);
}

/**
 * Adds a list row for setting the camera controls flag.
 */
void SettingsScreen::addCameraControlsFlagRow(ListView* listView)
{
	ListViewItem* listItem;
	Label* label;

	listItem = new ListViewItem();
	listView->addChild(listItem);

	label = new Label();
	label->setText(CAMERA_CONTROLS_LABEL_TEXT);

	mCameraControlsCheckBox = new CheckBox();
	mCameraControlsCheckBox->setState(true);
	listItem->addChild(this->createRow(label, mCameraControlsCheckBox));
	listView->addChild(label);
}

/**
 * Creates and adds main layout to the screen.
 */
void SettingsScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);

	ListView* listView = new ListView();
	mainLayout->addChild(listView);

	// Add set duration option row.
	this->addSetDurationRow(listView);

	// Add get duration option row.
	this->addGetDurationRow(listView);

	// Add options for setting and getting the video quality value.
	this->addVideoQualityRows(listView);

	// Add options for setting and getting the flash mode value.
	this->addFlashModeRows(listView);

	// Add option for setting the camera roll flag.
	this->addCameraRollFlagRow(listView);

	// Add option for setting the camera controls flag.
	this->addCameraControlsFlagRow(listView);

	// Add take picture button.
	mTakePictureBtn = new Button();
	mTakePictureBtn->setText(TAKE_PICTURE_BTN_TEXT);
	this->addButtonToListView(mTakePictureBtn, listView);

	// Add record video button.
	mRecordVideoBtn = new Button();
	mRecordVideoBtn->setText(RECORD_VIDEO_BTN_TEXT);
	this->addButtonToListView(mRecordVideoBtn, listView);

	// Add show image button.
	mShowImageScreen = new Button();
	mShowImageScreen->setText(SHOW_IMAGE_SCREEN_TEXT);
	this->addButtonToListView(mShowImageScreen, listView);

	// Add show video screen button.
	mShowVideoScreen = new Button();
	mShowVideoScreen->setText(SHOW_VIDEO_SCREEN_TEXT);
	this->addButtonToListView(mShowVideoScreen, listView);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void SettingsScreen::buttonClicked(Widget* button)
{
	char buf[BUF_SIZE];
	int syscallResult;
	if (button == mGetMaxDurationBtn)
	{
		syscallResult = maCaptureGetProperty(
			MA_CAPTURE_MAX_DURATION, buf, BUF_SIZE);
		if (syscallResult == syscallResult)
		{
			String text = MAX_DURATION_LABEL_TEXT;
			text += buf;
			mMaxDurationLabel->setText(text);
		}
		else
		{
			printf("SettingsScreen::buttonClicked maCaptureGetProperty(MA_CAPTURE_MAX_DURATION) = %d",
				syscallResult);
		}
	}
	else if (button == mSetVideoQualityBtn)
	{
		mListener.showVideoQualityScreen();
	}
	else if (button == mGetVideoQualityBtn)
	{
		this->getVideoQualityValue();
	}
	else if (button == mSetFlashModeBtn)
	{
		mListener.showFlashModeScreen();
	}
	else if (button == mGetFlashModeBtn)
	{
		this->getFlashModeValue();
	}
	else if (button == mRecordVideoBtn)
	{
		if (this->isUserInputDataValid())
		{
			syscallResult = maCaptureAction(MA_CAPTURE_ACTION_RECORD_VIDEO);
			printf("result maCaptureAction(MA_CAPTURE_ACTION_RECORD_VIDEO) = %d", syscallResult);
		}
	}
	else if (button == mTakePictureBtn)
	{
		syscallResult = maCaptureAction(MA_CAPTURE_ACTION_TAKE_PICTURE);
		printf("result maCaptureAction(MA_CAPTURE_ACTION_TAKE_PICTURE) = %d", syscallResult);
	}
	else if (button == mShowImageScreen)
	{
		mListener.showImageScreen();
	}
	else if (button == mShowVideoScreen)
	{
		mListener.showVideoScreen();
	}
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void SettingsScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if (checkBox == mCameraRollCheckBox)
	{
		MAUtil::String value = state ? "true" : "false";
		int syscallResult = maCaptureSetProperty(
			MA_CAPTURE_CAMERA_ROLL,
			value.c_str());
		printf("maCaptureSetProperty(MA_CAPTURE_CAMERA_ROLL) = %d", syscallResult);
	}
	else if (checkBox == mCameraControlsCheckBox)
	{
		MAUtil::String value = state ? "true" : "false";
		int syscallResult = maCaptureSetProperty(
			MA_CAPTURE_CAMERA_CONTROLS,
			value.c_str());
		printf("maCaptureSetProperty(MA_CAPTURE_CAMERA_CONTROLS) = %d", syscallResult);
	}
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void SettingsScreen::editBoxReturn(EditBox* editBox)
{
	if (editBox == mMaxDurationEditBox)
	{
		if (this->isMaxVideoDurationValid())
		{
			editBox->hideKeyboard();
		}
	}
}

/**
 * Check if the data received from the user is valid.
 * #return True is data is valid, false otherwise.
 */
bool SettingsScreen::isUserInputDataValid()
{
	bool returnValue = true;

	// Verify max video duration value.
	returnValue = this->isMaxVideoDurationValid();

	return returnValue;
}

/**
 * Check if the max video duration value given by the user is valid.
 * @return True if data si valid, false otherwise.
 */
bool SettingsScreen::isMaxVideoDurationValid()
{
	int resultSyscall = MA_CAPTURE_RES_OK;
	bool returnValue = true;

	String maxDuration = mMaxDurationEditBox->getText();
	// If user didn't provide a value just ignore this option.
	if (maxDuration.length() != 0)
	{
		resultSyscall = maCaptureSetProperty(
			MA_CAPTURE_MAX_DURATION,
			maxDuration.c_str());
		if (resultSyscall != MA_CAPTURE_RES_OK)
		{
			returnValue = false;
			mMaxDurationEditBox->setFontColor(RED_COLOR);
		}
		else
		{
			mMaxDurationEditBox->setFontColor(BLACK_COLOR);
		}
	}

	return returnValue;
}

/**
 * Get the current video quality value and displays it into a label.
 */
void SettingsScreen::getVideoQualityValue()
{
	char buf[MAX_BUF_SIZE];
	int syscallResult = maCaptureGetProperty(
		MA_CAPTURE_VIDEO_QUALITY, buf, MAX_BUF_SIZE);
	if (syscallResult != MA_CAPTURE_RES_OK)
	{
		maMessageBox("Error", "Cannot get the video duration value");
		return;
	}

	int videoQualityInt = stringToInteger(buf);
	MAUtil::String videoQualityString;
	switch (videoQualityInt)
	{
	case MA_CAPTURE_VIDEO_QUALITY_LOW:
		videoQualityString = VIDEO_QUALITY_LOW_TEXT;
		break;
	case MA_CAPTURE_VIDEO_QUALITY_MEDIUM:
		videoQualityString = VIDEO_QUALITY_MEDIUM_TEXT;
		break;
	case MA_CAPTURE_VIDEO_QUALITY_HIGH:
		videoQualityString = VIDEO_QUALITY_HIGH_TEXT;
		break;
	default:
		maMessageBox("Error", "Invalid video quality value");
	}

	mVideoQualityLabel->setText(videoQualityString);
}

/**
 * Get the current flash mode value and displays it into a label.
 */
void SettingsScreen::getFlashModeValue()
{
	char buf[MAX_BUF_SIZE];
	int syscallResult = maCaptureGetProperty(
		MA_CAPTURE_FLASH,
		buf,
		MAX_BUF_SIZE);
	if (syscallResult != MA_CAPTURE_RES_OK)
	{
		maMessageBox("Error", "Cannot get the flash mode value");
		return;
	}

	int flashModeInt = stringToInteger(buf);
	MAUtil::String flashModeString;
	switch (flashModeInt)
	{
	case MA_CAPTURE_FLASH_AUTO:
		flashModeString = FLASH_MODE_AUTO_TEXT;
		break;
	case MA_CAPTURE_FLASH_ON:
		flashModeString = FLASH_MODE_ON_TEXT;
		break;
	case MA_CAPTURE_FLASH_OFF:
		flashModeString = FLASH_MODE_OFF_TEXT;
		break;
	default:
		maMessageBox("Error", "Invalid flash mode value");
	}

	mFlashModeLabel->setText(flashModeString);
}

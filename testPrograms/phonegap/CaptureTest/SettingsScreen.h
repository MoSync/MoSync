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
 * @file SettingsScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying widgets for capture syscalls.
 * Contains widgets(e.g. check box, button) used for setting and getting
 * values for taking photos or recording video.
 */

#ifndef SETTINGS_SCREEN_H_
#define SETTINGS_SCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <maapi.h>

#include "ScreenListeners.h"

using namespace NativeUI;
using namespace MAUtil;


/**
 * @brief Screen used for displaying widgets for capture syscalls.
 * Contains widgets(e.g. check box, button) used for setting and getting
 * values for taking photos or recording video.
 */
class SettingsScreen:
	public Screen,
	public ButtonListener,
	public EditBoxListener,
	public CheckBoxListener
{

public:
	/**
	 * Constructor.
	 * @param listener Listener for screen.
	 */
	SettingsScreen(SettingsScreenListener& listener);

	/**
	 * Destructor.
	 */
	virtual ~SettingsScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Create a new row for settings.
	 * It contains two widgets separated by a spacer.
	 */
	HorizontalLayout* createRow(Widget* firstWidget, Widget* secondWidget);

	/**
	 * Add a button to a list view.
	 * @param button The button that will be added.
	 * @param listView The list view that will contain the button.
	 */
	void addButtonToListView(Button* button, ListView* listView);

	/**
	 * Adds a list row that contains widgets for setting the current
	 * maximum video duration.
	 */
	void addSetDurationRow(ListView* listView);

	/**
	 * Adds a list row that contains widgets for getting the current
	 * maximum video duration.
	 */
	void addGetDurationRow(ListView* listView);

	/**
	 * Adds two list rows for setting and getting the video quality value.
	 */
	void addVideoQualityRows(ListView* listView);

	/**
	 * Adds two list rows for setting and getting the flash mode value.
	 */
	void addFlashModeRows(ListView* listView);

	/**
	 * Adds a list row for setting the camera roll flag.
	 */
	void addCameraRollFlagRow(ListView* listView);

	/**
	 * Adds a list row for setting the camera controls flag.
	 */
	void addCameraControlsFlagRow(ListView* listView);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the state of the check box was changed
     * by the user.
     * @param checkBox The check box object that generated the event.
     * @param state True if the check box is checked, false otherwise.
     */
    virtual void checkBoxStateChanged(
        CheckBox* checkBox,
        bool state);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

    /**
     * Check if the data received from the user is valid.
     * #return True is data is valid, false otherwise.
     */
    bool isUserInputDataValid();

    /**
     * Check if the max video duration value given by the user is valid.
     * @return True if data si valid, false otherwise.
     */
    bool isMaxVideoDurationValid();

    /**
     * Get the current video quality value and displays it into a label.
     */
    void getVideoQualityValue();

    /**
     * Get the current flash mode value and displays it into a label.
     */
    void getFlashModeValue();

private:
    /**
     * Listener for screen.
     * Used for displaying other screens.
     */
    SettingsScreenListener& mListener;

    /**
     * Used to get the video max duration value.
     */
    EditBox* mMaxDurationEditBox;

    /**
     * Used for displaying the current max video duration.
     */
    Label* mMaxDurationLabel;

    /**
     * Used for getting the current max video duration.
     */
    Button* mGetMaxDurationBtn;

    /**
     * Used for setting the video quality option.
     */
    Button* mSetVideoQualityBtn;

    /**
     * Used for getting the video quality option.
     */
    Button* mGetVideoQualityBtn;

    /**
     * Used for displaying the video quality value.
     */
    Label* mVideoQualityLabel;

    /**
     * Used for showing the flash mode screen.
     */
    Button* mSetFlashModeBtn;

    /**
     * Used for getting the flash mode value.
     */
    Button* mGetFlashModeBtn;

    /**
     * Used for displaying the flash mode value.
     */
    Label* mFlashModeLabel;

    /**
     * Used for setting the camera roll flag.
     */
    CheckBox* mCameraRollCheckBox;

    /**
     * Used for setting the camera controls flag.
     */
    CheckBox* mCameraControlsCheckBox;

    /**
     * Used for showing the Native Image Picker(image mode).
     */
    Button* mTakePictureBtn;

    /**
     * Used for showing the Native Image Picker(video mode).
     */
    Button* mRecordVideoBtn;

    /**
     * Used for showing a screen containing a taken picture.
     */
    Button* mShowImageScreen;

    /**
     * Used for showing a screen containing a recorded video.
     */
    Button* mShowVideoScreen;

};

#endif /* SETTINGS_SCREEN_H_ */

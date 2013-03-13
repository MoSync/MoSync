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
 * @file FirstScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Screen used for setting supported screen orientations.
 */

#define SCREEN_TITLE "Set orientation"

#define PORTRAIT_LABEL_TEXT "Portrait(SENSOR based)"
#define PORTRAIT_UP_LABEL_TEXT "Portrait Up"
#define PORTRAIT_UPSIDE_DOWN_LABEL_TEXT "Portrait upside down"
#define LANDSCAPE_LEFT_LABEL_TEXT "Landscape left"
#define LANDSCAPE_RIGHT_LABEL_TEXT "Landscape right"
#define LANDSCAPE_LABEL_TEXT "Landscape(SENSOR based)"
#define DYNAMIC_LABEL_TEXT "Dynamic(SENSOR based)"
#define ORIENTATION_LABEL "Orientation:"
#define ORIENTATION_PORTRAIT "Portrait"
#define ORIENTATION_PORTRAIT_UPSIDE_DOWN "Portrait upside down"
#define ORIENTATION_LANDSCAPE_LEFT "Landscape left"
#define ORIENTATION_LANDSCAPE_RIGHT "Landscape right"

#define SCREEN_ORIENTATION_LANDSCAPE_STRING "Landscape"
#define SCREEN_ORIENTATION_PORTRAIT_STRING "Portrait"
#define SCREEN_ORIENTATION_DYNAMIC_STRING "Dynamic (sensor based)"

#include <conprint.h>
#include <maapi.h>

#include "FirstScreen.h"
#include "Util.h"

namespace OrientationTest
{
	/**
	 * Constructor.
	 */
	FirstScreen::FirstScreen() :
		Screen(),
		mSetOrientationBtn(NULL),
		mPortraitCheckBox(NULL),
		mPortraitUpsideDownCheckBox(NULL),
		mLandscapeLeftCheckBox(NULL),
		mLandscapeRightCheckBox(NULL),
		mPortraitUpCheckBox(NULL),
		mLandscapeCheckBox(NULL),
		mDynamicCheckBox(NULL),
		mOrientationLabel(NULL),
		mSetOrientationDescriptionLabel(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		mSelectedOrientation = -1;

		createMainLayout();

//			mPortraitCheckBox->setState(true);
		mPortraitCheckBox->addCheckBoxListener(this);
		mPortraitUpsideDownCheckBox->addCheckBoxListener(this);
		mLandscapeLeftCheckBox->addCheckBoxListener(this);
		mLandscapeRightCheckBox->addCheckBoxListener(this);
		mLandscapeCheckBox->addCheckBoxListener(this);
		mPortraitUpCheckBox->addCheckBoxListener(this);
		mPortraitUpCheckBox->setState(true);
		mDynamicCheckBox->addCheckBoxListener(this);
		addScreenListener(this);
		mSetOrientationBtn->addButtonListener(this);

		mSupportedOrientations = MA_SCREEN_ORIENTATION_PORTRAIT_UP;
		// By default it's portrait.
//		maScreenSetSupportedOrientations(mSupportedOrientations);
//
		mOrientationLabel->setText(PORTRAIT_UP_LABEL_TEXT);
	}

	/**
	 * Destructor.
	 */
	FirstScreen::~FirstScreen()
	{
		mPortraitCheckBox->removeCheckBoxListener(this);
		mPortraitUpsideDownCheckBox->removeCheckBoxListener(this);
		mLandscapeLeftCheckBox->removeCheckBoxListener(this);
		mLandscapeRightCheckBox->removeCheckBoxListener(this);
		mLandscapeCheckBox->removeCheckBoxListener(this);
		mPortraitUpCheckBox->removeCheckBoxListener(this);
		mDynamicCheckBox->removeCheckBoxListener(this);
		removeScreenListener(this);
		mSetOrientationBtn->removeButtonListener(this);
	}

	/**
	 * Create a new horizontal layout.
	 * Will contain a label and a check box.
	 * @param label The given label.
	 * @param checkBox The given check box.
	 * @return The new created horizontal layout.
	 */
	HorizontalLayout* FirstScreen::createRow(Label* label, CheckBox* checkBox)
	{
		HorizontalLayout* hLayout = new HorizontalLayout();
		hLayout->wrapContentVertically();
		hLayout->addChild(label);
		if (checkBox)
		{
			HorizontalLayout* space = new HorizontalLayout();
			space->setWidth(10);
			hLayout->addChild(space);
			hLayout->addChild(checkBox);
		}
		return hLayout;
	}

	/**
	 * Creates and adds main layout to the screen.
	 */
	void FirstScreen::createMainLayout()
	{
		// Create and add the main layout to the screen.
		VerticalLayout* mainLayout = new VerticalLayout();
		Screen::setMainWidget(mainLayout);

		// Add widgets for displaying the current orientation.
		HorizontalLayout* hLayout = new HorizontalLayout();
		hLayout->wrapContentVertically();
		Label* label = new Label();
		label->setText(ORIENTATION_LABEL);
		hLayout->addChild(label);
		mOrientationLabel = new Label();
		hLayout->addChild(mOrientationLabel);
		mainLayout->addChild(hLayout);

		// Add widgets for enabling/disabling sensor portrait mode.
		label = new Label();
		label->setText(PORTRAIT_LABEL_TEXT);
		mPortraitCheckBox = new CheckBox();
		mainLayout->addChild(this->createRow(label, mPortraitCheckBox));

		HorizontalLayout* pLayout = new HorizontalLayout();
		pLayout->wrapContentVertically();
		// Add widgets for enabling/disabling portrait Up mode.
		label = new Label();
		label->setText(PORTRAIT_UP_LABEL_TEXT);
		mPortraitUpCheckBox = new CheckBox();
		pLayout->addChild(this->createRow(label, mPortraitUpCheckBox));

		// Add widgets for enabling/disabling portrait upside down mode.
		label = new Label();
		label->setText(PORTRAIT_UPSIDE_DOWN_LABEL_TEXT);
		mPortraitUpsideDownCheckBox = new CheckBox();
		pLayout->addChild(this->createRow(label, mPortraitUpsideDownCheckBox));
		mainLayout->addChild(pLayout);

		// Add widgets for enabling/disabling sensor landscape mode.
		label = new Label();
		label->setText(LANDSCAPE_LABEL_TEXT);
		mLandscapeCheckBox = new CheckBox();
		mainLayout->addChild(this->createRow(label, mLandscapeCheckBox));

		HorizontalLayout* lLayout = new HorizontalLayout();
		lLayout->wrapContentVertically();
		// Add widgets for enabling/disabling landscape left mode.
		label = new Label();
		label->setText(LANDSCAPE_LEFT_LABEL_TEXT);
		mLandscapeLeftCheckBox = new CheckBox();
		lLayout->addChild(this->createRow(label, mLandscapeLeftCheckBox));

		// Add widgets for enabling/disabling landscape right mode.
		label = new Label();
		label->setText(LANDSCAPE_RIGHT_LABEL_TEXT);
		mLandscapeRightCheckBox = new CheckBox();
		lLayout->addChild(this->createRow(label, mLandscapeRightCheckBox));
		mainLayout->addChild(lLayout);

		// Add widgets for enabling/disabling sensor mode.
		label = new Label();
		label->setText(DYNAMIC_LABEL_TEXT);
		mDynamicCheckBox = new CheckBox();
		mainLayout->addChild(this->createRow(label, mDynamicCheckBox));

		mSetOrientationBtn = new Button();
		mSetOrientationBtn->setText("Apply selected supported orientations");
		mSetOrientationBtn->setFontColor(0xFF0000);
		mainLayout->addChild(mSetOrientationBtn);
	}

	/**
	 * This method is called when the state of the check box was changed
	 * by the user.
	 * @param checkBox The check box object that generated the event.
	 * @param state True if the check box is checked, false otherwise.
	 */
	void FirstScreen::checkBoxStateChanged(
		CheckBox* checkBox,
		bool state)
	{
		bool isChecked;
		if (checkBox == mPortraitCheckBox)
		{
			isChecked = mPortraitCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set portrait mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
				MA_SCREEN_ORIENTATION_PORTRAIT,
				mPortraitCheckBox);
		}
		else if (checkBox == mPortraitUpCheckBox)
		{
			isChecked = mPortraitUpCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set portrait Up mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
				MA_SCREEN_ORIENTATION_PORTRAIT_UP,
				mPortraitUpCheckBox);
		}
		else if (checkBox == mPortraitUpsideDownCheckBox)
		{
			isChecked = mPortraitUpsideDownCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set portrait upside down mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
				MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN,
				mPortraitUpsideDownCheckBox);
		}
		else if (checkBox == mLandscapeLeftCheckBox)
		{
			isChecked = mLandscapeLeftCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set landscape left mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
				MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT,
				mLandscapeLeftCheckBox);
		}
		else if (checkBox == mLandscapeRightCheckBox)
		{
			isChecked = mLandscapeRightCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set landscape right mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
					MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT,
				mLandscapeRightCheckBox);
		}
		else if (checkBox == mLandscapeCheckBox)
		{
			isChecked = mLandscapeCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set landscape sensor mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
					MA_SCREEN_ORIENTATION_LANDSCAPE,
					mLandscapeCheckBox);
		}
		else if (checkBox == mDynamicCheckBox)
		{
			isChecked = mDynamicCheckBox->isChecked();
			printf("FirstScreen::checkBoxStateChanged set landscape sensor mode to %d",
				isChecked);
			this->changeOrientationBitmask(isChecked,
					MA_SCREEN_ORIENTATION_DYNAMIC,
					mDynamicCheckBox);
		}
//		int result = maScreenSetSupportedOrientations(mSupportedOrientations);
//		printf("FirstScreen::checkBoxStateChanged result maScreenSetSupportedOrientations = %d",
//			result);
	}

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS, Android, Windows Phone.
     * @param button The button object that generated the event.
     */
    void FirstScreen::buttonClicked(Widget* button)
    {
		if (mSetOrientationBtn == button)
		{
			int result = maScreenSetSupportedOrientations(mSupportedOrientations);
			printf("FirstScreen::checkBoxStateChanged result maScreenSetSupportedOrientations = %d",
				result);
		}

    }

	/**
	 * Changes the screen orientation bit mask.
	 * @param cond If true, the orientation flag will be added to the screen
	 * orientation bit mask, otherwise the flag is removed.
	 * @param orientation Orientation value that is needed to be add/removed from
	 * bit mask. Must be one of the MA_SCREEN_ORIENTATION constants.
	 * @param checkBox If the bit mask value will be zero after removing the
	 * orientation value than the checkBox will be set to checked.
	 */
	void FirstScreen::changeOrientationBitmask(const bool cond,
		const int orientation,
		CheckBox* checkBox)
	{
		if (cond)
		{
			mSupportedOrientations = mSupportedOrientations | orientation;
		}
		else
		{
			mSupportedOrientations = mSupportedOrientations & ~orientation;
		}

		if (0 == mSupportedOrientations)
		{
			mSupportedOrientations = orientation;
//			checkBox->setState(true);
		}
	}

	/**
	 * Called just before the screen begins rotating.
	 */
	void FirstScreen::orientationWillChange()
	{
		mOrientationLabel->setText(this->getOrientationString());
	}

	/**
	 * Called after the screen orientation has changed.
	 * Available only on iOS and Windows Phone 7.1 platforms.
	 */
	void FirstScreen::orientationDidChange()
	{
		//mOrientationLabel->setText(this->getOrientationString());
	}

	/**
	 * Called after the screen has finished rotating.
	 * Subclasses may override this method to perform additional actions
	 * after the rotation.
	 * @param screenOrientation The new screen orientation.
	 */
	void FirstScreen::orientationChanged(Screen* screen, int screenOrientation)
	{
		MAUtil::String orientationText;
		switch (screenOrientation)
		{
			case MA_SCREEN_ORIENTATION_PORTRAIT:
				orientationText = ORIENTATION_PORTRAIT;
				break;
			case MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
				orientationText = ORIENTATION_PORTRAIT_UPSIDE_DOWN;
				break;
			case MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT:
				orientationText = ORIENTATION_LANDSCAPE_LEFT;
				break;
			case MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT:
				orientationText = ORIENTATION_LANDSCAPE_RIGHT;
				break;
			case MA_SCREEN_ORIENTATION_PORTRAIT_UP:
				orientationText = PORTRAIT_UP_LABEL_TEXT;
				break;
		}

		mOrientationLabel->setText(orientationText);
	}

	/**
	 * Gets the current orientation as a string
	 */
	MAUtil::String FirstScreen::getOrientationString()
	{
		int orientation = maScreenGetCurrentOrientation();
		MAUtil::String orientationText;
		switch (orientation)
		{
			case MA_SCREEN_ORIENTATION_PORTRAIT:
				orientationText = ORIENTATION_PORTRAIT;
				break;
			case MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
				orientationText = ORIENTATION_PORTRAIT_UPSIDE_DOWN;
				break;
			case MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT:
				orientationText = ORIENTATION_LANDSCAPE_LEFT;
				break;
			case MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT:
				orientationText = ORIENTATION_LANDSCAPE_RIGHT;
				break;
			case MA_SCREEN_ORIENTATION_PORTRAIT_UP:
				orientationText = PORTRAIT_UP_LABEL_TEXT;
				break;
		}

		return orientationText;
	}

} // namespace OrientationTest

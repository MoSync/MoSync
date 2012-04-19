/*
 Copyright (C) 2012 MoSync AB

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
 * @file OrientationScreen.cpp
 * @author Bogdan Iusco
 * @date 2 Mar 2012
 *
 * @brief Screen used to show data received from orientation sensor.
 */


#define SCREEN_TITLE "Orientation"

#define VALUE_LABEL_TEXT  "Orientation value:"

#define START_BUTTON_TEXT "Start Orientation sensor"
#define STOP_BUTTON_TEXT "Stop Orientation sensor"

#define ORIENTATION_UNKNOWN_TEXT              "unknown"
#define ORIENTATION_PORTRAIT_TEXT             "portrait"
#define ORIENTATION_PORTRAIT_UPSIDE_DOWN_TEXT "portrait upside down"
#define ORIENTATION_LANDSCAPE_LEFT            "Landscape left"
#define ORIENTATION_LANDSCAPE_RIGHT           "Landscape right"
#define ORIENTATION_FACE_UP                   "Face up"
#define ORIENTATION_FACE_DOWN                 "Face down"

#include <NativeUI/HorizontalLayout.h>

#include "OrientationScreen.h"

namespace SensorNativeUI
{
	/**
	 * Constructor.
	 * @param applicationController Interface to controller.
	 */
	OrientationScreen::OrientationScreen(
			IApplicationController& applicationController) :
		NativeUI::Screen(),
		mApplicationController(applicationController),
		mMainLayout(NULL),
		mStartOrientationButton(NULL),
		mStopOrientationButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		this->createUI();
		mStartOrientationButton->addButtonListener(this);
		mStopOrientationButton->addButtonListener(this);

		mStopOrientationButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	OrientationScreen::~OrientationScreen()
	{
		mStartOrientationButton->removeButtonListener(this);
		mStopOrientationButton->removeButtonListener(this);
	}

	/**
	 * Set the new orientation value.
	 * @param value Orientation data value.
	 * Must be one of the next values:
	 * - UIDEVICE_ORIENTATION_UNKNOWN
	 * - UIDEVICE_ORIENTATION_PORTRAIT
	 * - UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN
	 * - UIDEVICE_ORIENTATION_LANDSCAPE_LEFT
	 * - UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT
	 * - UIDEVICE_ORIENTATION_FACE_UP
	 * - UIDEVICE_ORIENTATION_FACE_DOWN
	 */
	void OrientationScreen::setOrientationData(const int value)
	{
		MAUtil::String valueString = "";
		switch (value)
		{
		case UIDEVICE_ORIENTATION_UNKNOWN:
			valueString = ORIENTATION_UNKNOWN_TEXT;
			break;
		case UIDEVICE_ORIENTATION_PORTRAIT:
			valueString = ORIENTATION_PORTRAIT_TEXT;
			break;
		case UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
			valueString = ORIENTATION_PORTRAIT_UPSIDE_DOWN_TEXT;
			break;
		case UIDEVICE_ORIENTATION_LANDSCAPE_LEFT:
			valueString = ORIENTATION_LANDSCAPE_LEFT;
			break;
		case UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT:
			valueString = ORIENTATION_LANDSCAPE_RIGHT;
			break;
		case UIDEVICE_ORIENTATION_FACE_UP:
			valueString = ORIENTATION_FACE_UP;
			break;
		case UIDEVICE_ORIENTATION_FACE_DOWN:
			valueString = ORIENTATION_FACE_DOWN;
			break;
		}

		mOrientationValueLabel->setText(valueString);
	}

	/**
	 * Stop the sensor.
	 * Called by application controller to stop the sensor when user
	 * changes the screen from tab screen.
	 * Screens should update the UI(e.g. enable the start sensor button and
	 * disable the stop sensor button).
	 */
	void OrientationScreen::stopSensor()
	{
		mStartOrientationButton->setEnabled(true);
		mStopOrientationButton->setEnabled(false);
		mApplicationController.stopSensor(SENSOR_TYPE_ORIENTATION);
	}

	/**
	 * Create widgets and add them to screen.
	 */
	void OrientationScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		this->setMainWidget(mMainLayout);

		// Create widgets for orientation data.
		mOrientationValueLabel = new NativeUI::Label();
		this->createHorizontalWidgetsRow(mOrientationValueLabel,
			VALUE_LABEL_TEXT);

		// Add a spacer
		NativeUI::Label* spacer = new NativeUI::Label();
		spacer->setHeight(this->getHeight() / 6);
		mMainLayout->addChild(spacer);

		// Create button for start reading orientation values.
		mStartOrientationButton = new NativeUI::Button();
		mStartOrientationButton->setText(START_BUTTON_TEXT);
		mStartOrientationButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStartOrientationButton);

		// Create button for stop reading orientation values.
		mStopOrientationButton = new NativeUI::Button();
		mStopOrientationButton->setText(STOP_BUTTON_TEXT);
		mStopOrientationButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStopOrientationButton);
	}

	/**
	 * Create a horizontal layout that will contain a first label with a
	 * given text and a seconds label given as argument. Layout will be
	 * added to screen.
	 * @param dataLabel Second label that will be added to layout.
	 * Object's ownership is passed to this method.
	 * @param firstLabelText Text that will be set to the first label.
	 */
	void OrientationScreen::createHorizontalWidgetsRow(
		NativeUI::Label* dataLabel,
		const MAUtil::String& firstLabelText)
	{
		NativeUI::HorizontalLayout* layout = new NativeUI::HorizontalLayout();
		layout->wrapContentVertically();
		mMainLayout->addChild(layout);

		// Add first label to layout.
		NativeUI::Label* firstLabel = new NativeUI::Label();
		firstLabel->setText(firstLabelText);
		layout->addChild(firstLabel);

		// Add second label to layout.
		layout->addChild(dataLabel);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS, Android, Windows Phone.
	 * @param button The button object that generated the event.
	 */
	void OrientationScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mStartOrientationButton)
		{
			mApplicationController.startSensor(SENSOR_TYPE_ORIENTATION);
			mStopOrientationButton->setEnabled(true);
			mStartOrientationButton->setEnabled(false);
		}
		else if (button == mStopOrientationButton)
		{
			this->stopSensor();
		}
	}
}

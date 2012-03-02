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
 * @file GyroscopeScreen.cpp
 * @author Bogdan Iusco
 * @date 1 Mar 2012
 *
 * @brief Screen used to show data received from gyroscope.
 */

#define SCREEN_TITLE "Gyroscope"

#define VALUE_X_LABEL_TEXT  "X:"
#define VALUE_Y_LABEL_TEXT  "Y:"
#define VALUE_Z_LABEL_TEXT  "Z:"

#define START_BUTTON_TEXT "Start gyroscope"
#define STOP_BUTTON_TEXT "Stop gyroscope"

#define BUF_DATA_SIZE 30

#include <NativeUI/HorizontalLayout.h>

#include "GyroscopeScreen.h"

namespace SensorNativeUI
{
	/**
	 * Constructor.
	 * @param applicationController Interface to controller.
	 */
	GyroscopeScreen::GyroscopeScreen(
			IApplicationController& applicationController) :
		NativeUI::Screen(),
		mApplicationController(applicationController),
		mMainLayout(NULL),
		mStartGyroscopeButton(NULL),
		mStopGyroscopeButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		this->createUI();
		mStartGyroscopeButton->addButtonListener(this);
		mStopGyroscopeButton->addButtonListener(this);

		mStopGyroscopeButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	GyroscopeScreen::~GyroscopeScreen()
	{
		mStartGyroscopeButton->removeButtonListener(this);
		mStopGyroscopeButton->removeButtonListener(this);
	}

	/**
	 * Set the new gyroscope data.
	 * @param x Gyroscope x value.
	 * @param y Gyroscope y value.
	 * @param z Gyroscope z value.
	 */
	void GyroscopeScreen::setGyroscopeData(
		const float x,
		const float y,
		const float z)
	{
		char bufX[BUF_DATA_SIZE];
		sprintf(bufX, "%f", x);
		mValueXLabel->setText(bufX);

		char bufY[BUF_DATA_SIZE];
		sprintf(bufY, "%f", y);
		mValueYLabel->setText(bufY);

		char bufZ[BUF_DATA_SIZE];
		sprintf(bufZ, "%f", z);
		mValueZLabel->setText(bufZ);
	}

	/**
	 * Stop the sensor.
	 * Called by application controller to stop the sensor when user
	 * changes the screen from tab screen.
	 * Screens should update the UI(e.g. enable the start sensor button and
	 * disable the stop sensor button).
	 */
	void GyroscopeScreen::stopSensor()
	{
		mStartGyroscopeButton->setEnabled(true);
		mStopGyroscopeButton->setEnabled(false);
		mApplicationController.stopSensor(SENSOR_TYPE_GYROSCOPE);
	}

	/**
	 * Create widgets and add them to screen.
	 */
	void GyroscopeScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		this->setMainWidget(mMainLayout);

		// Create widgets for gyroscope data: x,y,z values.
		mValueXLabel = new NativeUI::Label();
		mValueYLabel = new NativeUI::Label();
		mValueZLabel = new NativeUI::Label();
		this->createHorizontalWidgetsRow(mValueXLabel, VALUE_X_LABEL_TEXT);
		this->createHorizontalWidgetsRow(mValueYLabel, VALUE_Y_LABEL_TEXT);
		this->createHorizontalWidgetsRow(mValueZLabel, VALUE_Z_LABEL_TEXT);

		// Add a spacer
		NativeUI::Label* spacer = new NativeUI::Label();
		spacer->setHeight(this->getHeight() / 6);
		mMainLayout->addChild(spacer);

		// Create button for start reading gyroscope values.
		mStartGyroscopeButton = new NativeUI::Button();
		mStartGyroscopeButton->setText(START_BUTTON_TEXT);
		mStartGyroscopeButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStartGyroscopeButton);

		// Create button for stop reading gyroscope values.
		mStopGyroscopeButton = new NativeUI::Button();
		mStopGyroscopeButton->setText(STOP_BUTTON_TEXT);
		mStopGyroscopeButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStopGyroscopeButton);
	}

	/**
	 * Create a horizontal layout that will contain a first label with a
	 * given text and a seconds label given as argument. Layout will be
	 * added to screen.
	 * @param dataLabel Second label that will be added to layout.
	 * Object's ownership is passed to this method.
	 * @param firstLabelText Text that will be set to the first label.
	 */
	void GyroscopeScreen::createHorizontalWidgetsRow(
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
	void GyroscopeScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mStartGyroscopeButton)
		{
			mApplicationController.startSensor(SENSOR_TYPE_GYROSCOPE);
			mStopGyroscopeButton->setEnabled(true);
			mStartGyroscopeButton->setEnabled(false);
		}
		else if (button == mStopGyroscopeButton)
		{
			this->stopSensor();
		}
	}
}

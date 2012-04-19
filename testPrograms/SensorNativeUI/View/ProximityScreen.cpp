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
 * @file ProximityScreen.cpp
 * @author Bogdan Iusco
 * @date 2 Mar 2012
 *
 * @brief Screen used to show data received from proximity sensor.
 */

#define SCREEN_TITLE "Proximity"

#define VALUE_LABEL_TEXT  "Proximity value:"

#define START_BUTTON_TEXT "Start proximity sensor"
#define STOP_BUTTON_TEXT "Stop proximity sensor"

#define SENSOR_NEAR_VALUE_TEXT "near"
#define SENSOR_FAR_VALUE_TEXT "far"

#include <NativeUI/HorizontalLayout.h>

#include "ProximityScreen.h"

namespace SensorNativeUI
{
	/**
	 * Constructor.
	 * @param applicationController Interface to controller.
	 */
	ProximityScreen::ProximityScreen(
			IApplicationController& applicationController) :
		NativeUI::Screen(),
		mApplicationController(applicationController),
		mMainLayout(NULL),
		mStartProximityButton(NULL),
		mStopProximityButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		this->createUI();
		mStartProximityButton->addButtonListener(this);
		mStopProximityButton->addButtonListener(this);

		mStopProximityButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	ProximityScreen::~ProximityScreen()
	{
		mStartProximityButton->removeButtonListener(this);
		mStopProximityButton->removeButtonListener(this);
	}

	/**
	 * Set the new proximity value.
	 * @param value Proximity data value.
	 * Must be one of the next values:
	 * - SENSOR_PROXIMITY_VALUE_FAR
	 * - SENSOR_PROXIMITY_VALUE_NEAR
	 */
	void ProximityScreen::setProximityData(const int value)
	{
		MAUtil::String stringValue = "";
		switch(value)
		{
		case SENSOR_PROXIMITY_VALUE_FAR:
			stringValue = SENSOR_FAR_VALUE_TEXT;
			break;
		case SENSOR_PROXIMITY_VALUE_NEAR:
			stringValue = SENSOR_NEAR_VALUE_TEXT;
			break;
		}

		mProximityValueLabel->setText(stringValue);
	}

	/**
	 * Stop the sensor.
	 * Called by application controller to stop the sensor when user
	 * changes the screen from tab screen.
	 * Screens should update the UI(e.g. enable the start sensor button and
	 * disable the stop sensor button).
	 */
	void ProximityScreen::stopSensor()
	{
		mStartProximityButton->setEnabled(true);
		mStopProximityButton->setEnabled(false);
		mApplicationController.stopSensor(SENSOR_TYPE_PROXIMITY);
	}

	/**
	 * Create widgets and add them to screen.
	 */
	void ProximityScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		this->setMainWidget(mMainLayout);

		// Create widgets for proximity data.
		mProximityValueLabel = new NativeUI::Label();
		this->createHorizontalWidgetsRow(mProximityValueLabel, VALUE_LABEL_TEXT);

		// Add a spacer
		NativeUI::Label* spacer = new NativeUI::Label();
		spacer->setHeight(this->getHeight() / 6);
		mMainLayout->addChild(spacer);

		// Create button for start reading proximity values.
		mStartProximityButton = new NativeUI::Button();
		mStartProximityButton->setText(START_BUTTON_TEXT);
		mStartProximityButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStartProximityButton);

		// Create button for stop reading proximity values.
		mStopProximityButton = new NativeUI::Button();
		mStopProximityButton->setText(STOP_BUTTON_TEXT);
		mStopProximityButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStopProximityButton);
	}

	/**
	 * Create a horizontal layout that will contain a first label with a
	 * given text and a seconds label given as argument. Layout will be
	 * added to screen.
	 * @param dataLabel Second label that will be added to layout.
	 * Object's ownership is passed to this method.
	 * @param firstLabelText Text that will be set to the first label.
	 */
	void ProximityScreen::createHorizontalWidgetsRow(
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
	void ProximityScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mStartProximityButton)
		{
			mApplicationController.startSensor(SENSOR_TYPE_PROXIMITY);
			mStopProximityButton->setEnabled(true);
			mStartProximityButton->setEnabled(false);
		}
		else if (button == mStopProximityButton)
		{
			this->stopSensor();
		}
	}
}

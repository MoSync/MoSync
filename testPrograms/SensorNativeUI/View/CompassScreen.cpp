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
 * @file CompassScreen.cpp
 * @author Bogdan Iusco
 * @date 2 Mar 2012
 *
 * @brief Screen used to show data received from compass sensor.
 */

#define SCREEN_TITLE "Compass"

#define VALUE_LABEL_TEXT  "Compass value:"

#define START_BUTTON_TEXT "Start compass"
#define STOP_BUTTON_TEXT "Stop compass"

#define BUF_DATA_SIZE 30

#include <NativeUI/HorizontalLayout.h>

#include "CompassScreen.h"

namespace SensorNativeUI
{
	/**
	 * Constructor.
	 * @param applicationController Interface to controller.
	 */
	CompassScreen::CompassScreen(
			IApplicationController& applicationController) :
		NativeUI::Screen(),
		mApplicationController(applicationController),
		mMainLayout(NULL),
		mStartCompassButton(NULL),
		mStopCompassButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		this->createUI();
		mStartCompassButton->addButtonListener(this);
		mStopCompassButton->addButtonListener(this);

		mStopCompassButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	CompassScreen::~CompassScreen()
	{
		mStartCompassButton->removeButtonListener(this);
		mStopCompassButton->removeButtonListener(this);
	}

	/**
	 * Set the new compass value.
	 * @param value Compass data value.
	 */
	void CompassScreen::setCompassData(const float value)
	{
		char buf[BUF_DATA_SIZE];
		sprintf(buf, "%f", value);
		mCompassValueLabel->setText(buf);
	}

	/**
	 * Stop the sensor.
	 * Called by application controller to stop the sensor when user
	 * changes the screen from tab screen.
	 * Screens should update the UI(e.g. enable the start sensor button and
	 * disable the stop sensor button).
	 */
	void CompassScreen::stopSensor()
	{
		mStartCompassButton->setEnabled(true);
		mStopCompassButton->setEnabled(false);
		mApplicationController.stopSensor(SENSOR_TYPE_COMPASS);
	}

	/**
	 * Create widgets and add them to screen.
	 */
	void CompassScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		this->setMainWidget(mMainLayout);

		// Create widgets for compass data.
		mCompassValueLabel = new NativeUI::Label();
		this->createHorizontalWidgetsRow(mCompassValueLabel, VALUE_LABEL_TEXT);

		// Add a spacer
		NativeUI::Label* spacer = new NativeUI::Label();
		spacer->setHeight(this->getHeight() / 6);
		mMainLayout->addChild(spacer);

		// Create button for start reading compass values.
		mStartCompassButton = new NativeUI::Button();
		mStartCompassButton->setText(START_BUTTON_TEXT);
		mStartCompassButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStartCompassButton);

		// Create button for stop reading compass values.
		mStopCompassButton = new NativeUI::Button();
		mStopCompassButton->setText(STOP_BUTTON_TEXT);
		mStopCompassButton->fillSpaceHorizontally();
		mMainLayout->addChild(mStopCompassButton);
	}

	/**
	 * Create a horizontal layout that will contain a first label with a
	 * given text and a seconds label given as argument. Layout will be
	 * added to screen.
	 * @param dataLabel Second label that will be added to layout.
	 * Object's ownership is passed to this method.
	 * @param firstLabelText Text that will be set to the first label.
	 */
	void CompassScreen::createHorizontalWidgetsRow(
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
	void CompassScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mStartCompassButton)
		{
			mApplicationController.startSensor(SENSOR_TYPE_COMPASS);
			mStopCompassButton->setEnabled(true);
			mStartCompassButton->setEnabled(false);
		}
		else if (button == mStopCompassButton)
		{
			this->stopSensor();
		}
	}
}

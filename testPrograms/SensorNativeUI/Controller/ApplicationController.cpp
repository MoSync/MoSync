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
 * @file ApplicationController.cpp
 * @author Bogdan Iusco
 * @date 1 Mar 2012
 *
 * @brief Application's controller.
 */

#include "ApplicationController.h"
#include "../View/AccelerometerScreen.h"
#include "../View/GyroscopeScreen.h"
#include "../View/MagneticFieldScreen.h"
#include "../View/CompassScreen.h"
#include "../View/ProximityScreen.h"
#include "../View/OrientationScreen.h"

namespace SensorNativeUI
{

	/**
	 * Constructor.
	 */
	ApplicationController::ApplicationController() :
		mTabScreen(NULL),
		mAccelerometerScreen(NULL),
		mGyroscopeScreen(NULL),
		mMagneticFieldScreen(NULL),
		mCompassScreen(NULL),
		mProximityScreen(NULL),
		mOrientationScreen(NULL),
		mCurrentScreen(NULL)
	{
		mTabScreen = new NativeUI::TabScreen();

		this->createScreens();
		this->addScreensToTabScreen();

		mTabScreen->addTabScreenListener(this);
		mTabScreen->show();

		mCurrentScreen = mAccelerometerScreen;

		MAUtil::Environment::getEnvironment().addSensorListener(this);
	}

	/**
	 * Destructor.
	 */
	ApplicationController::~ApplicationController()
	{
		MAUtil::Environment::getEnvironment().removeSensorListener(this);

		// Do NOT delete other screens because the tab screen will do that.
		delete mTabScreen;
	}

	/**
	 * Tell controller to start reading a given sensor data.
	 * @param sensorType One of the SENSOR_TYPE_ constants.
	 */
	void ApplicationController::startSensor(const int sensorType)
	{
		maSensorStart(sensorType, SENSOR_RATE_NORMAL);
	}

	/**
	 * Tell controller to stop reading a given sensor data.
	 * @param sensorType One of the SENSOR_TYPE_ constants.
	 */
	void ApplicationController::stopSensor(const int sensorType)
	{
		maSensorStop(sensorType);
	}

	/**
	 * Called when a sensor has new values.
	 * @param sensorData Sensor's values.
	 */
	void ApplicationController::sensorEvent(MASensor sensorData)
	{
		switch (sensorData.type)
		{
		case SENSOR_TYPE_ACCELEROMETER:
			mAccelerometerScreen->setAccelerationData(
				sensorData.values[0], sensorData.values[1], sensorData.values[2]);
			break;
		case SENSOR_TYPE_GYROSCOPE:
			mGyroscopeScreen->setGyroscopeData(
				sensorData.values[0], sensorData.values[1], sensorData.values[2]);
			break;
		case SENSOR_TYPE_MAGNETIC_FIELD:
			mMagneticFieldScreen->setMagneticFieldData(
				sensorData.values[0], sensorData.values[1], sensorData.values[2]);
			break;
		case SENSOR_TYPE_COMPASS:
			mCompassScreen->setCompassData(sensorData.values[0]);
			break;
		case SENSOR_TYPE_PROXIMITY:
			mProximityScreen->setProximityData( (int) sensorData.values[0] );
			break;
		case SENSOR_TYPE_ORIENTATION:
			mOrientationScreen->setOrientationData( (int) sensorData.values[0] );
			break;
		}
	}

	/**
	 * This method is called when a tab screen has changed to a new tab.
	 * @param tabScreen The tab screen object that generated the event.
	 * @param tabScreenIndex The index of the new tab.
	 */
	void ApplicationController::tabScreenTabChanged(
		NativeUI::TabScreen* tabScreen,
		const int tabScreenIndex)
	{
		if (mCurrentScreen)
		{
			mCurrentScreen->stopSensor();
		}

		switch (tabScreenIndex)
		{
		case 0:
			mCurrentScreen = mAccelerometerScreen;
			break;
		case 1:
			mCurrentScreen = mGyroscopeScreen;
			break;
		case 2:
			mCurrentScreen = mMagneticFieldScreen;
			break;
		case 3:
			mCurrentScreen = mCompassScreen;
			break;
		case 4:
			mCurrentScreen = mProximityScreen;
			break;
		case 5:
			mCurrentScreen = mOrientationScreen;
			break;
		}
	}

	/**
	 * Create all contained screens.
	 */
	void ApplicationController::createScreens()
	{
		mAccelerometerScreen = new AccelerometerScreen(*this);
		mGyroscopeScreen = new GyroscopeScreen(*this);
		mMagneticFieldScreen = new MagneticFieldScreen(*this);
		mCompassScreen = new CompassScreen(*this);
		mProximityScreen = new ProximityScreen(*this);
		mOrientationScreen = new OrientationScreen(*this);
	}

	/**
	 * Add all contained screens to tab screen.
	 * Note: make sure that the screen objects are created before calling
	 * this method.
	 */
	void ApplicationController::addScreensToTabScreen()
	{
		mTabScreen->addTab(mAccelerometerScreen);
		mTabScreen->addTab(mGyroscopeScreen);
		mTabScreen->addTab(mMagneticFieldScreen);
		mTabScreen->addTab(mCompassScreen);
		mTabScreen->addTab(mProximityScreen);
		mTabScreen->addTab(mOrientationScreen);
	}
}

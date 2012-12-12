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
 * @file MainScreen.cpp
 * @author Spiridon Alexandru
 */
#include "MainScreen.h"
#include <NativeUI/Location.h>
#include <NativeUI/MapRegion.h>
#include <NativeUI/MapPin.h>
#include <NativeUI/MapLocation.h>
#include <mastdlib.h>

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mGetVisibleAreaButton(NULL),
	mSetVisibleAreaButton(NULL),
	mAddPinToMap(NULL),
	mRemovePinFromMap(NULL),
	mSetZoomLevel(NULL),
	mSetCenterButton(NULL),
	mMap(NULL)
{
	createMainLayout();

	mMap->addMapListener(this);
	mGetVisibleAreaButton->addButtonListener(this);
	mSetVisibleAreaButton->addButtonListener(this);
	mAddPinToMap->addButtonListener(this);
	mRemovePinFromMap->addButtonListener(this);
	mSetZoomLevel->addButtonListener(this);
	mSetCenterButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mMapPins.clear();

	mMap->removeMapListener(this);
	mGetVisibleAreaButton->removeButtonListener(this);
	mSetVisibleAreaButton->removeButtonListener(this);
	mAddPinToMap->removeButtonListener(this);
	mRemovePinFromMap->removeButtonListener(this);
	mSetZoomLevel->removeButtonListener(this);
	mSetCenterButton->removeButtonListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x99CCFF);
	Screen::setMainWidget(mMainLayout);

	// create and add the title label to the main layout
	mTitleLabel = new Label();
	mTitleLabel->setText("Map widget");
	mTitleLabel->setFontSize(32.0);
	mTitleLabel->fillSpaceHorizontally();
	mTitleLabel->setTextHorizontalAlignment("center");
	mMainLayout->addChild(mTitleLabel);

	createMapPinLayout();
	createVisibleAreaLayout();
	createZoomLevelLayout();

	// create the set center button (it will center the map if the user clicks it)
	// and add it to the main layout
	mSetCenterButton = new Button();
	mSetCenterButton->fillSpaceHorizontally();
	mSetCenterButton->setText("Set center");
	mMainLayout->addChild(mSetCenterButton);

	// create the main map and add it to the main layout
	mMap = new Map("google mock credentials", "bing mock credentials");
	mMap->fillSpaceHorizontally();
	mMap->fillSpaceVertically();

	// set the map type to satellite (by default it's set to MAP_TYPE_ROAD)
	mMap->setMapType(MAP_TYPE_ROAD);
	printf("Map type: %d", mMap->getMapType());

	// set the map initial zoom level
	mMap->setZoomLevel(3);
	printf("Map zoom level: %d", mMap->getZoomLevel());

	mMainLayout->addChild(mMap);
}

/**
 * Creates and add the map pin add/remove layout to the screen.
 */
void MainScreen::createMapPinLayout()
{
	// create the pins layout and add the add and remove pin buttons
	// and add it to the main layout
	mPinsLayout = new HorizontalLayout();
	mPinsLayout->fillSpaceHorizontally();
	mPinsLayout->wrapContentVertically();

	mAddPinToMap = new Button();
	mAddPinToMap->fillSpaceHorizontally();
	mAddPinToMap->setText("Add pin");
	mPinsLayout->addChild(mAddPinToMap);

	mRemovePinFromMap = new Button();
	mRemovePinFromMap->fillSpaceHorizontally();
	mRemovePinFromMap->setText("Remove pin");
	mPinsLayout->addChild(mRemovePinFromMap);

	mMainLayout->addChild(mPinsLayout);
}

/**
 * Creates and adds the visible area get/set layout to the screen.
 */
void MainScreen::createVisibleAreaLayout()
{
	// create the visible area layout and add the get and set visible area buttons
	// and add it to the main layout
	mVisibleAreaLayout = new HorizontalLayout();
	mVisibleAreaLayout->fillSpaceHorizontally();
	mVisibleAreaLayout->wrapContentVertically();

	mGetVisibleAreaButton = new Button();
	mGetVisibleAreaButton->setText("Get visible area ");
	mGetVisibleAreaButton->fillSpaceHorizontally();
	mVisibleAreaLayout->addChild(mGetVisibleAreaButton);

	mSetVisibleAreaButton = new Button();
	mSetVisibleAreaButton->setText("Set visible area");
	mSetVisibleAreaButton->fillSpaceHorizontally();
	mVisibleAreaLayout->addChild(mSetVisibleAreaButton);

	mMainLayout->addChild(mVisibleAreaLayout);

	// add the label that will contain the visible area coordinates after
	// pressing the get visible area button
	mVisibleAreaCoordinatesLabel = new Label();
	mVisibleAreaCoordinatesLabel->fillSpaceHorizontally();
	mVisibleAreaCoordinatesLabel->setText("Visible area coordinates");
	mMainLayout->addChild(mVisibleAreaCoordinatesLabel);
}

/**
 * Creates and add the zoom level layout to the screen.
 */
void MainScreen::createZoomLevelLayout()
{
	// create the zoom level layout that will contain the set zoom level
	// button and the zoom level edit box and add it to the main layout
	mZoomLevelLayout = new HorizontalLayout();
	mZoomLevelLayout->fillSpaceHorizontally();
	mZoomLevelLayout->wrapContentVertically();

	mSetZoomLevel = new Button();
	mSetZoomLevel->setText("Set Z level");
	mSetZoomLevel->fillSpaceHorizontally();
	mZoomLevelLayout->addChild(mSetZoomLevel);

	mZoomLevelEditBox = new EditBox();
	mZoomLevelEditBox->fillSpaceHorizontally();
	mZoomLevelLayout->addChild(mZoomLevelEditBox);

	mMainLayout->addChild(mZoomLevelLayout);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if (button == mGetVisibleAreaButton)
	{
		MapRegion region;
		mMap->getVisibleArea(region);

		mVisibleAreaCoordinatesLabel->setText("UL la: " + MAUtil::doubleToString(region.getUpperLeftCorner().getLatitude()) +
						" UL lo: " + MAUtil::doubleToString(region.getUpperLeftCorner().getLongitude()) +
						" LR la: " + MAUtil::doubleToString(region.getLowerRightCorner().getLatitude()) +
						" LR lo: " + MAUtil::doubleToString(region.getLowerRightCorner().getLongitude()));
	}
	else if (button == mSetVisibleAreaButton)
	{
		mMap->setVisibleArea(32.1234, 22.123124, 11.231123, 12.32344);
	}
	else if (button == mSetZoomLevel)
	{
		mMap->setZoomLevel(MAUtil::stringToInteger(mZoomLevelEditBox->getText()));
		mZoomLevelEditBox->hideKeyboard();
	}
	else if (button == mAddPinToMap)
	{
		for (int i = 0; i < 20; i++)
		{
			// generate random values for the coordinates
			// get a random latitude and longitude
			double latitude = (double)(rand() % 80);
			double longitude = (double)(rand() & 170);
			int longSign = rand()%2;
			if (longSign == 0)
				longitude *= (-1);

			int latSign = rand()%2;
			if (latSign == 0)
				latitude *= (-1);

			MapPin *newPin = new MapPin(Location(latitude, longitude));
			newPin->setText("test title");
			mMapPins.add(newPin);
			mMap->addMapPin(newPin);
		}
	}
	else if (button == mRemovePinFromMap)
	{
		for (int i = 0; i < mMapPins.size(); i++)
		{
			mMap->removeMapPin(mMapPins[i]);
		}
	}
	else if (button == mSetCenterButton)
	{
		mMap->setCenter(MapLocation(32.43, 43.34, 11));
		mMap->centerMap();
	}
}

/**
 * This method is called when the zoom level of the map changes (at a double tap
 * on the google maps for example).
 * @param map The map object that generated the event.
 */
void MainScreen::mapZoomLevelChanged(Map* map)
{

}

/**
 * This method is called when the visible region on the map is changed (on a drag/scroll
 * for example).
 * @param map The map object that generated the event.
 */
void MainScreen::mapRegionChanged(Map* map)
{

}

/**
 * This method is called when the user clicks on a map pin.
 * @param mapPin The map pin object that generated the event.
 */
void MainScreen::mapPinClicked(MapPin* mapPin)
{

}

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

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mMap(NULL)
{
	createMainLayout();

	mMap->addMapListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mMap->removeMapListener(this);
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
	mTitleLabel->setText("Map widget test program");
	mTitleLabel->setFontSize(32.0);
	mTitleLabel->fillSpaceHorizontally();
	mTitleLabel->setTextHorizontalAlignment("center");
	mMainLayout->addChild(mTitleLabel);

	mMap = new Map();
	mMap->fillSpaceHorizontally();
	mMap->fillSpaceVertically();

	// set the map type to satellite (by default it's set to MAP_TYPE_ROAD)
	mMap->setMapType(MAP_TYPE_ROAD);
	printf("Map type: %d", mMap->getMapType());

	mMap->setZoomLevel(3);
	printf("Map zoom level: %d", mMap->getZoomLevel());

	mMainLayout->addChild(mMap);
}

/**
 * This method is called when the zoom level of the map changes (at a double tap
 * on the google maps for example).
 * @param map The map object that generated the event.
 */
void MainScreen::mapZoomLevelChanged(Map* map)
{
	printf("%s","Zoom level changed!");
}

/**
 * This method is called when the visible region on the map is changed (on a drag/scroll
 * for example).
 * @param map The map object that generated the event.
 */
void MainScreen::mapRegionChanged(Map* map)
{
	printf("%s","Region level changed!");
}

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
 * @file ScreenMain.cpp
 * @author Mikael Kindborg
 *
 * This is the main entry point of the user interface
 * of the application. Here the application screens
 * are created and connected to the main tab screen.
 */

#include "ScreenMain.h"
#include "ScreenColorList.h"
#include "ScreenWebView.h"
#include "ScreenImageSwiper.h"
#include "ScreenSpinningCube.h"
#include "ScreenSettings.h"

/**
 * Create the main screen.
 */
Screen* ScreenMain::create(WidgetManager* widgetManager)
{
	TabScreen* tabScreen = widgetManager->createTabScreen();

	Screen* colorScreen = ScreenColorList::create(widgetManager);
	Screen* webScreen = ScreenWebView::create(widgetManager);
	Screen* imageScreen = ScreenImageSwiper::create(widgetManager);
	Screen* cubeScreen = ScreenSpinningCube::create(widgetManager);
	Screen* settingsScreen = ScreenSettings::create(widgetManager);

	tabScreen->addTab(colorScreen);
	tabScreen->addTab(webScreen);
	tabScreen->addTab(imageScreen);
	tabScreen->addTab(cubeScreen);
	tabScreen->addTab(settingsScreen);

	return tabScreen;
}


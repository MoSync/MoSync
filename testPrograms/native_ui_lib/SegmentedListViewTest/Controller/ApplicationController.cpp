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
 * @date 22 Feb 2012
 *
 * @brief Application's controller.
 */

#include <MAUtil/String.h>

#include "ApplicationController.h"
#include "../View/IndexedListScreen.h"
#include "../View/SettingsScreen.h"
#include "../Model/DataSource.h"

using namespace MAUtil;


/**
 * Constructor.
 */
ApplicationController::ApplicationController():
	mTabScreen(NULL),
	mSettingsScreen(NULL),
	mIndexedListViewScreen(NULL),
	mDataSource(NULL)
{
	mDataSource = new DataSource();
	mTabScreen = new TabScreen();

	this->createScreens();

	mTabScreen->show();
}

/**
 * Destructor.
 */
ApplicationController::~ApplicationController()
{
	delete mTabScreen;
}

/**
 * Create screens and add them to tab screen.
 */
void ApplicationController::createScreens()
{
	mIndexedListViewScreen = new IndexedListScreen(*mDataSource);
	mSettingsScreen = new SettingsScreen();

	mTabScreen->addTab(mIndexedListViewScreen);
	mTabScreen->addTab(mSettingsScreen);
}

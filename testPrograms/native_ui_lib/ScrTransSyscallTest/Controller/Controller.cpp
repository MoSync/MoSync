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
 * @file Controller.cpp
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief App main controller.
 */

#include "Controller.h"
#include "../Screens/FirstScreen.h"
#include "../Screens/SecondScreen.h"

#include <NativeUI/Screen.h>

static int currentTransition = MAW_TRANSITION_TYPE_NONE;

namespace Transitions
{
	/**
	 * Constructor.
	 */
	Controller::Controller()
	{
		mFirstScreen = new FirstScreen(*this);
		mSecondScreen = new SecondScreen(*this);

		maWidgetScreenShow(mFirstScreen->getWidgetHandle());
		//mFirstScreen->show();
	}

	/**
	 * Destructor.
	 */
	Controller::~Controller()
	{
		delete mFirstScreen;
		delete mSecondScreen;
	}

	/**
	 * Tell the observer to show the second screen.
	 */
	void Controller::showSecondScreen(short transType /*TODO Replace with object*/, const char* text)
	{
		currentTransition = transType;
		mSecondScreen->resetTitleWithString(text);
		//mSecondScreen->show();
		//maWidgetScreenShow(mSecondScreen->getWidgetHandle());
		maWidgetScreenShowWithTransition(mSecondScreen->getWidgetHandle(), transType, 300);
	}

	/**
	 * Tell the observer to hide the second screen.
	 */
	void Controller::hideSecondScreen()
	{
		//maWidgetScreenShow(mFirstScreen->getWidgetHandle());
		maWidgetScreenShowWithTransition(mFirstScreen->getWidgetHandle(), currentTransition, 300);
		//mFirstScreen->show();
	}
} // end of Transitions

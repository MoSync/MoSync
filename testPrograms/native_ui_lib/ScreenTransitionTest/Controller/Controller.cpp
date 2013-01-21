/*
 Copyright (C) 2013 MoSync AB

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
#include <NativeUI/Screen.h>

#include "Controller.h"

#include "../Screens/ScreenUtils.h"
#include "../Screens/FirstScreen.h"
#include "../Screens/SecondScreen.h"

static MAWScreenTransitionType currentTransition = MAW_TRANSITION_TYPE_NONE;

namespace ScreenTransitionTest
{
	/**
	 * Constructor.
	 */
	Controller::Controller()
	{
		mFirstScreen = new FirstScreen(*this);
		mSecondScreen = new SecondScreen(*this);

		//maWidgetScreenShow(mFirstScreen->getWidgetHandle());
		mFirstScreen->show();
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
	void Controller::showSecondScreen(int transType, const char* text)
	{
		currentTransition = transType;
		mSecondScreen->resetTitleWithString(text);

		//int returnVal = maWidgetScreenShowWithTransition(mSecondScreen->getWidgetHandle(), transType, TRANSITION_TIME_MS);
		int returnVal = mSecondScreen->showWithTransition(transType, TRANSITION_TIME_MS);
		printf("showWithTransition result: %d", returnVal);
	}

	/**
	 * Tell the observer to hide the second screen.
	 */
	void Controller::hideSecondScreen()
	{
		//int returnVal = maWidgetScreenShowWithTransition(mFirstScreen->getWidgetHandle(), ScreenUtils::getSimetricTransition(currentTransition), TRANSITION_TIME_MS);
		int returnVal = mFirstScreen->showWithTransition(ScreenUtils::getSimetricTransition(currentTransition), TRANSITION_TIME_MS);
		printf("showWithTransition result: %d", returnVal);
	}
} // end of Transitions

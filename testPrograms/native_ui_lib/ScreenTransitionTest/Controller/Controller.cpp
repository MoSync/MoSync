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
#include "../Screens/TransitionsScreen.h"
#include "../Screens/LastScreen.h"
#include "../Screens/MainStackScreen.h"

static MAWScreenTransitionType currentTransition = MAW_TRANSITION_TYPE_NONE;

namespace ScreenTransitionTest
{
	/**
	 * Constructor.
	 */
	Controller::Controller()
	{
		mStackScreen = new MainStackScreen(*this);
		mSimpleScreen = new LastScreen(*this);

		mStackScreen->show();
	}

	/**
	 * Destructor.
	 */
	Controller::~Controller()
	{
		delete mStackScreen;
		delete mSimpleScreen;
	}

	/**
	 * Tell the observer to show the last screen.
	 */
	void Controller::showLastScreen(int transType)
	{
		currentTransition = transType;
		const char* nameOfSymmetricTransition = ScreenUtils::getTransitionName(ScreenUtils::getSimetricTransition(currentTransition));
		mSimpleScreen->resetTitleWithString(nameOfSymmetricTransition);

		//int returnVal = maWidgetScreenShowWithTransition(mSimpleScreen->getWidgetHandle(), transType, TRANSITION_TIME_MS);
		int returnVal = mSimpleScreen->showWithTransition(transType, TRANSITION_TIME_MS);
		printf("showWithTransition result: %d", returnVal);
	}

	/**
	 * Tell the observer to hide the second screen.
	 */
	void Controller::hideLastScreen()
	{
		//int returnVal = maWidgetScreenShowWithTransition(mStackScreen->getWidgetHandle(), ScreenUtils::getSimetricTransition(currentTransition), TRANSITION_TIME_MS);
		int returnVal = mStackScreen->showWithTransition(ScreenUtils::getSimetricTransition(currentTransition), TRANSITION_TIME_MS);
		printf("showWithTransition result: %d", returnVal);
	}
} // end of Transitions

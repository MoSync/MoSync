/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
	Controller::Controller():
		mIsLastScreenShown(false)
	{
		mStackScreen = new MainStackScreen(*this);
		mSimpleScreen = new LastScreen(*this);

		ScreenUtils::initScreenSizeConstants(
			mStackScreen->getWidth(),
			mStackScreen->getHeight());

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
		(MAW_RES_OK == returnVal) ? mIsLastScreenShown = true:false;
		printf("showWithTransition result: %d", returnVal);
	}

	/**
	 * Tell the observer to hide the second screen.
	 */
	void Controller::hideLastScreen()
	{
		//int returnVal = maWidgetScreenShowWithTransition(mStackScreen->getWidgetHandle(), ScreenUtils::getSimetricTransition(currentTransition), TRANSITION_TIME_MS);
		int returnVal = mStackScreen->showWithTransition(ScreenUtils::getSimetricTransition(currentTransition), TRANSITION_TIME_MS);
		(MAW_RES_OK == returnVal) ? mIsLastScreenShown = false:true;
		printf("showWithTransition result: %d", returnVal);
	}

	void Controller::handleAndroidBackButton()
	{
		if ( (1 == mStackScreen->getStackSize()) || mIsLastScreenShown )
		{
			maExit(0);
		}
	}
} // end of ScreenTransitionTest

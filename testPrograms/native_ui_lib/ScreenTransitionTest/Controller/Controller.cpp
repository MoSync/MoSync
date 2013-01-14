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
#define TRANSITION_TIME_MS 400

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
	void Controller::showSecondScreen(short transType, const char* text)
	{
		currentTransition = transType;
		mSecondScreen->resetTitleWithString(text);

		//int returnval maWidgetScreenShowWithTransition(mSecondScreen->getWidgetHandle(), transType, TRANSITION_TIME_MS);
		int returnVal = mSecondScreen->showWithTransition(transType, TRANSITION_TIME_MS);
		printf("showWithTransition result: %d", returnVal);
	}

	/**
	 * Tell the observer to hide the second screen.
	 */
	void Controller::hideSecondScreen()
	{
		//int returnval maWidgetScreenShowWithTransition(mFirstScreen->getWidgetHandle(), getSimetricScreenTransition(currentTransition), TRANSITION_TIME_MS);
		int returnVal = mFirstScreen->showWithTransition(getSimetricScreenTransition(currentTransition), TRANSITION_TIME_MS);
		printf("showWithTransition result: %d", returnVal);
	}

	MAWScreenTransitionType Controller::getSimetricScreenTransition(int screenTransitionType)
	{
		MAWScreenTransitionType returnTransition = MAW_TRANSITION_TYPE_NONE;
		switch(screenTransitionType)
		{
		case MAW_TRANSITION_TYPE_FLIP_FROM_LEFT:
			returnTransition = MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT;
			break;
		case MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT:
			returnTransition = MAW_TRANSITION_TYPE_FLIP_FROM_LEFT;
			break;
		case MAW_TRANSITION_TYPE_CURL_UP:
			returnTransition = MAW_TRANSITION_TYPE_CURL_DOWN;
			break;
		case MAW_TRANSITION_TYPE_CURL_DOWN:
			returnTransition = MAW_TRANSITION_TYPE_CURL_UP;
			break;
		case MAW_TRANSITION_TYPE_SLIDE_LEFT:
			returnTransition = MAW_TRANSITION_TYPE_SLIDE_RIGHT;
			break;
		case MAW_TRANSITION_TYPE_SLIDE_RIGHT:
			returnTransition = MAW_TRANSITION_TYPE_SLIDE_LEFT;
			break;
		case MAW_TRANSITION_TYPE_FADE_IN:
			returnTransition = MAW_TRANSITION_TYPE_FADE_OUT;
			break;
		case MAW_TRANSITION_TYPE_FADE_OUT:
			returnTransition = MAW_TRANSITION_TYPE_FADE_IN;
			break;
		case MAW_TRANSITION_TYPE_SWIVEL_IN:
			returnTransition = MAW_TRANSITION_TYPE_SWIVEL_OUT;
			break;
		case MAW_TRANSITION_TYPE_SWIVEL_OUT:
			returnTransition = MAW_TRANSITION_TYPE_SWIVEL_IN;
			break;
		case MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD:
			returnTransition = MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD;
			break;
		case MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD:
			returnTransition = MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD;
			break;
		default:
			break;
		}
		return returnTransition;
	}
} // end of Transitions

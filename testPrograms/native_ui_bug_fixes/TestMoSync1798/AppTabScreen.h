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
 * @file AppTabScreen.h
 * @author Bogdan Iusco.
 * @brief Application's tab screen class.
 */

#ifndef APPTABSCREEN_H_
#define APPTABSCREEN_H_

#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Application's tab screen class.
 */
class AppTabScreen:
	public TabScreen
{
public:

	/**
	 * Constructor.
	 */
	AppTabScreen();

	/**
	 * Destructor.
	 */
	~AppTabScreen();

private:
	/**
	 * Create and initialize first screen.
	 */
	void createFirstScreen();

	/**
	 * Create and initialize second screen.
	 */
	void createSecondScreen();

private:
	/**
	 * First screen in tab screen.
	 */
	Screen* mFirstScreen;

	/**
	 * Second screen in tab screen.
	 */
	Screen* mSecondScreen;
};

#endif /* APPTABSCREEN_H_ */

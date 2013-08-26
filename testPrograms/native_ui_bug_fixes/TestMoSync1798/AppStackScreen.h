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
 * @file StackScreen.h
 * @author Bogdan Iusco.
 * @brief Application's stack screen class.
 */

#ifndef APPSTACKSCREEN_H_
#define APPSTACKSCREEN_H_

#include <NativeUI/Widgets.h>

#include "HomeScreenListener.h"

using namespace NativeUI;

// Forward declaration
class HomeScreen;
class AppTabScreen;

/**
 * @brief Application's stack screen class.
 */
class AppStackScreen:
	public StackScreen,
	public HomeScreenListener
{
public:
	/**
	 * Constructor.
	 */
	AppStackScreen();

	/**
	 * Destructor.
	 */
	~AppStackScreen();

private:
	/**
	 * Show the tab screen.
	 */
	void showTabScreen();

private:
	/**
	 * First screen shown by stack screen.
	 */
	HomeScreen* mHomeScreen;

	/**
	 * Second screen shown by stack screen.
	 */
	AppTabScreen* mTabScreen;
};

#endif /* APPSTACKSCREEN_H_ */

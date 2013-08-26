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

/** @file main.cpp
 * @author Emma Tresanszki
 *
 * This application provides an example on how to use the Native UI.
 * It performs text searches on the Wikipedia, based on user input ( tags )
 * and selected categories. After searching for content that matches the given
 * tags, the user may select the desired article titles, and open  web views
 * to see the entire articles.
 * The application makes use of MoSync's Moblet framework to handle events.
 * On the first screen: HomeScreen, the user can provide some tags.
 * On the second screen: TitlesScreen, a list of available titles is displayed.
 * Each title can be checked/unchecked.
 * On the third screen: SummaryScreen, the snippets are displayed, for the
 * checked titles in the previous screen.
 * By clicking on each snippet, a Web view is shown in the WebScreen.
 * The application is exited by pressing the back button.
 */

// Library for the event manager, that manages the application main loop.
#include <MAUtil/Moblet.h>

// Include the header file for this application's moblet.
#include "WikiMoblet.h"

using namespace MAUtil;

// The entry point for the application - the place where processing starts.
extern "C" int MAMain()
{
	// Create an instance for WikiMoblet & run it to start the application.
	MAUtil::Moblet::run(WikiNativeUI::WikiMoblet::getInstance());

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
}

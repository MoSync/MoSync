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
 * @file MainScreen.h.
 * @author emma
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "FirstScreen.h"
#include "SecondScreen.h"
#include "ThirdScreen.h"

using namespace NativeUI;

class MainScreen:
	public TabScreen,
	public TabScreenListener
{

public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

    /**
     * This method is called when a tab screen has changed to a new tab.
     * @param tabScreen The tab screen object that generated the event.
     * @param tabScreenIndex The index of the new tab.
     */
    virtual void tabScreenTabChanged(
        TabScreen* tabScreen,
        const int tabScreenIndex);

	/**
	 * Handle keypresses.
	 */
	void handleKeyPress(int keyCode);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	FirstScreen* mFirstScreen;
	SecondScreen* mSecondScreen;
	ThirdScreen* mThirdScreen;
	int mCurrentTabIndex;
};

#endif /* MAINSCREEN_H_ */

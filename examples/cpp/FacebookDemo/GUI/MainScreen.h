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

/*
 * MainMenu.h
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_


#include <MAUtil/Moblet.h>
#include "ListScreen.h"

namespace FacebookDemoGUI
{

class MainScreen: public FacebookDemoScreen, public NativeUI::ButtonListener, public NativeUI::ListViewListener
{
public:
	MainScreen(MAUtil::Moblet *moblet);
	/**
	 * Override of KeyListener
	 * Called when a key is pressed.
	 *@param keyCode - the code of the key pressed
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	virtual void listViewItemClicked(
		NativeUI::ListView *listView,
		NativeUI::ListViewSection *listViewSection,
		NativeUI::ListViewItem *listViewItem);

	virtual void buttonClicked(Widget* button);

private:
	virtual int addChild(NativeUI::Widget* widget);

private:
	void initialize();

private:
	void closeApplication();

private:
	MAUtil::Moblet *mAppMoblet;
};

}

#endif /* MAINMENU_H_ */

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
 * MainMenu.cpp
 */

#include <maapi.h>
#include "MainScreen.h"
#include "Facebook/LOG.h"

#include "../PlatformInfo.h"

namespace FacebookDemoGUI
{

MainScreen::MainScreen(MAUtil::Moblet *moblet):FacebookDemoScreen(0)
{
	mAppMoblet = moblet;

	initialize();
}

void MainScreen::keyPressEvent(int keyCode, int nativeCode)
{
	if( MAK_BACK == keyCode)
	{
		closeApplication();
	}
}

void MainScreen::listViewItemClicked(
	NativeUI::ListView *listView,
	NativeUI::ListViewSection *listViewSection,
	NativeUI::ListViewItem *listViewItem)
{
	ListItem *item = (ListItem*)listViewItem;
	item->doClick();
}

void MainScreen::buttonClicked(Widget* button)
{
	closeApplication();
}

int MainScreen::addChild(NativeUI::Widget* widget)
{
	return NativeUI::Screen::addChild(widget);
}

void MainScreen::initialize()
{
	LOG("\n\t\tMainScreen::initialize()");

	mClearScreenAfterLosingFocus = false;

	NativeUI::ListView *list = new NativeUI::ListView();
	list->addListViewListener(this);
	list->setBackgroundColor(mScreenColor);

	if(FacebookDemoApplication::isAndroid())
	{
		NativeUI::VerticalLayout *layout = new NativeUI::VerticalLayout();
		layout->setBackgroundColor(mScreenColor);

		NativeUI::Button *button = new NativeUI::Button();
		button->addButtonListener(this);
		button->setText("exit");
		button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		button->fillSpaceHorizontally();

		layout->addChild(list);
		layout->addChild(button);

		setMainWidget(layout);

		mLayout = layout;
		mBackButton = button;
	}
	else
	{
		setMainWidget(list);
	}

	mList = list;

}

void MainScreen::closeApplication()
{
	if(mAppMoblet)
	{
		mAppMoblet->closeEvent();
	}
	else
	{
		maExit(0);
	}
}

}

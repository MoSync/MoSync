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

void MainScreen::listViewItemClicked(NativeUI::ListView* listView, NativeUI::ListViewItem* listViewItem)
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

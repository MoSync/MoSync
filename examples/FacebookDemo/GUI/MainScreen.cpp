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

namespace FacebookDemoGUI
{

MainScreen::MainScreen(MAUtil::Moblet *moblet)
{
	mAppMoblet = moblet;
	mBackButton->setText("exit");
	mBackButton->addButtonListener(this);
}

void MainScreen::keyPressEvent(int keyCode, int nativeCode)
{
	if( MAK_BACK == keyCode)
	{
		closeApplication();
	}
}

void MainScreen::buttonClicked(Widget* button)
{
	closeApplication();
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

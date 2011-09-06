/*
 * MainMenu.cpp
 *
 *  Created on: Sep 3, 2011
 *      Author: gabi
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
	closeApplication();
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

/*
 * OpenMenuCommand.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#include "OpenMenuCommand.h"
#include "Facebook/LOG.h"

namespace FacebookDemoApplication
{

OpenMenuCommand::OpenMenuCommand(FacebookDemoGUI::FacebookDemoScreen *previousScreen)
{
	mMenuScreen = new FacebookDemoGUI::ListScreen(previousScreen);
	mMenuScreen->clearScreenAfterLosingFocus(false);
}

void OpenMenuCommand::execute()
{
	mMenuScreen->show();
}

FacebookDemoGUI::ListScreen *OpenMenuCommand::getMenuScreen()
{
	return mMenuScreen;
}

void OpenMenuCommand::addMenuItem(FacebookDemoGUI::ListItem *item)
{
	mMenuScreen->add(item);
}

OpenMenuCommand::~OpenMenuCommand()
{
	delete mMenuScreen;
}

}//namespace FacebookDemoApplication

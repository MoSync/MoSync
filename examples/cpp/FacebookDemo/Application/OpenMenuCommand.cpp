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
 * OpenMenuCommand.cpp
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

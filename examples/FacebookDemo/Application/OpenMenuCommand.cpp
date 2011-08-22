/* Copyright (C) 2011 MoSync AB

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
 * OpenMenuCommand.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#include "OpenMenuCommand.h"
#include <Facebook/LOG.h>

namespace FacebookDemoApplication
{

OpenMenuCommand::OpenMenuCommand(FacebookDemoGUI::FacebookDemoScreen *previousScreen)
{
//	LOG("\n\t\tOpenMenuCommand constructor BEGIN. this=%d", this);
	mMenuScreen = new FacebookDemoGUI::ListScreen(previousScreen);
//	LOG("\t\tOpenMenuCommand constructor END. this=%d\n", this);
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
//	LOG("n\t\tOpenMenuCommand destructor BEGIN, this = %d");
	delete mMenuScreen;
//	LOG("\t\tOpenMenuCommand desctructor END, this=%d\n");
}

}//namespace FacebookDemoApplication

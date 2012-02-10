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
 * OpenMenuCommand.h
 */

#ifndef OPENMENUCOMMAND_H_
#define OPENMENUCOMMAND_H_

#include "ICommand.h"

#include "../GUI/FacebookDemoScreen.h"
#include "../GUI/ListScreen.h"
#include "../GUI/ListItem.h"

namespace FacebookDemoApplication
{

/*
 * class for opening a menu. Each menu item (button) will have a command on it.
 */
class OpenMenuCommand : public ICommand
{

public:
	OpenMenuCommand(FacebookDemoGUI::FacebookDemoScreen *previousScreen);
	void addMenuItem(FacebookDemoGUI::ListItem *item);
	virtual void execute();

	FacebookDemoGUI::ListScreen *getMenuScreen();

	~OpenMenuCommand();

private:
	FacebookDemoGUI::ListScreen *mMenuScreen;
};

}//namespace FacebookDemoApplication


#endif /* OPENMENUCOMMAND_H_ */

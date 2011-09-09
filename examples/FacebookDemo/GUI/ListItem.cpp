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
 * ListItem.cpp
 */

#include "ListItem.h"
#include "Facebook/LOG.h"

namespace FacebookDemoGUI
{

ListItem::ListItem(FacebookDemoApplication::ICommand *cmd, const MAUtil::String &text)
{
	mCommand = cmd;
	setText(text);
}

ListItem::ListItem():mCommand(0)
{

}


void ListItem::setCommand(FacebookDemoApplication::ICommand *cmd)
{
	mCommand = cmd;
}

ListItem::~ListItem()
{
	delete mCommand;
}

void ListItem::doClick()
{
	if(mCommand)
	{
		mCommand->execute();
	}
}

}//FacebookDemoGUI

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
 * ListItem.h
 */

#ifndef LISTITEM_H_
#define LISTITEM_H_

#include <MAUtil/Environment.h>
#include <MAUtil/String.h>

#include <NativeUI/ListViewItem.h>
#include "../Application/ICommand.h"

namespace FacebookDemoGUI
{

class ListItem: public NativeUI::ListViewItem
{
public:
	ListItem(FacebookDemoApplication::ICommand *cmd, const MAUtil::String &text);
	ListItem();

	void setCommand(FacebookDemoApplication::ICommand *cmd);
	void doClick();

	virtual ~ListItem();

public:
	FacebookDemoApplication::ICommand *mCommand;
};

}//namespace FacebookDemoGUI

#endif /* LISTITEM_H_ */

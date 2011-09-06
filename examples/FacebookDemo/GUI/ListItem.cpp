/*
 * MenuButton.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
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

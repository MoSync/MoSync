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
 * MenuButton.cpp
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#include "NativeUI/WidgetManager.h"
#include "ListItem.h"
#include <Facebook/LOG.h>

namespace FacebookDemoGUI
{

ListItem::ListItem(FacebookDemoApplication::ICommand *cmd, const MAUtil::String &text)
{
	mCommand = cmd;
	setText(text);
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
//	LOG("\t\tListItem constructor, this=%d", this);
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
//	LOG("\t\tListItem destructor BEGIN, this=%d", this);
	delete mCommand;
	MAUtil::Environment::getEnvironment().removeCustomEventListener(this);
//	LOG("\t\tListItem destructor END, this=%d", this);
}

void ListItem::doClick()
{
	if(mCommand)
	{
		mCommand->execute();
	}
}

void ListItem::customEvent(const MAEvent& event)
{
	if (EVENT_TYPE_WIDGET == event.type)
	{
		// Get the widget event data structure.
		MAWidgetEventData* widgetEvent = (MAWidgetEventData*) event.data;

		// Has the button been clicked?
		if (MAW_EVENT_CLICKED == widgetEvent->eventType )
		{
			if(this->getWidgetHandle() == widgetEvent->widgetHandle)
			{
				doClick();
			}
		}
	}
}

}//FacebookDemoGUI

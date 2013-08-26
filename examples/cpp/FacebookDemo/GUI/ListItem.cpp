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

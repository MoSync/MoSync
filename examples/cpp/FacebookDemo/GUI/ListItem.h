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

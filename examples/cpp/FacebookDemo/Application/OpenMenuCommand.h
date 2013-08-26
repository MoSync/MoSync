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

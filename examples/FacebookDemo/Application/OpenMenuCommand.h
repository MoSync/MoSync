/*
 * OpenMenuCommand.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
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

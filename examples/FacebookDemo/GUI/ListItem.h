/*
 * ListItem.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
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

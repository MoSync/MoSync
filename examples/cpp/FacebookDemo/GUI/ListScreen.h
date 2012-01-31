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
 * ListScreen.h
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include <MAUtil/Environment.h>

#include "FacebookDemoScreen.h"

#include "ListItem.h"
#include "FacebookDemoScreen.h"
#include <NativeUI/Widgets.h>

namespace FacebookDemoGUI
{

class ListScreen: public FacebookDemoScreen, public NativeUI::ButtonListener, public NativeUI::ListViewListener
{
public:
	ListScreen(FacebookDemoScreen *prev);
	ListScreen();

	virtual void show();

	void clear();

	virtual void listViewItemClicked(NativeUI::ListView* listView, NativeUI::ListViewItem* listViewItem);

	virtual void buttonClicked(Widget* button);

	virtual ~ListScreen();

private:
	void initialize();

protected:
	virtual void back();

};

}//namespace FacebookDemoGUI


#endif /* MENUSCREEN_H_ */

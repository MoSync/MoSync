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
 * MainMenu.h
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_


#include <MAUtil/Moblet.h>
#include "ListScreen.h"

namespace FacebookDemoGUI
{

class MainScreen: public FacebookDemoScreen, public NativeUI::ButtonListener, public NativeUI::ListViewListener
{
public:
	MainScreen(MAUtil::Moblet *moblet);
	/**
	 * Override of KeyListener
	 * Called when a key is pressed.
	 *@param keyCode - the code of the key pressed
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	virtual void listViewItemClicked(NativeUI::ListView* listView, NativeUI::ListViewItem* listViewItem);

	virtual void buttonClicked(Widget* button);

private:
	virtual int addChild(NativeUI::Widget* widget);

private:
	void initialize();

private:
	void closeApplication();

private:
	MAUtil::Moblet *mAppMoblet;
};

}

#endif /* MAINMENU_H_ */

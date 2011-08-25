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
 * ListScreen.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include <MAUtil/Environment.h>

#include "FacebookDemoScreen.h"
#include "NativeUI/Screen.h"
#include "NativeUI/ListView.h"
#include "NativeUI/WidgetEventListener.h"

#include "ListItem.h"
#include "FacebookDemoScreen.h"

namespace FacebookDemoGUI
{

class ListScreen: public FacebookDemoScreen
{
public:
	ListScreen(FacebookDemoScreen *prev);
	ListScreen();

	virtual void show();

	void add(ListItem *btn);
	void remove(ListItem *btn);

	void clear();
	bool isEmpty() const;

	void setBackgroundColor(int color);

	//0.0 - not visible
	//0.5 - half visible
	//1.0 - fully visible
	//1
	void setTransparency(float alpha);
	virtual ~ListScreen();
private:
	virtual void addChild(MoSync::UI::Widget *widget);

private:
	void initialize();

private:
	MoSync::UI::ListView *mList;
	int 				mBackgroundColor;
	float 				mTransparency;
};

}//namespace FacebookDemoGUI


#endif /* MENUSCREEN_H_ */

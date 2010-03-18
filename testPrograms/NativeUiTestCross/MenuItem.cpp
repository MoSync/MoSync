/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/*
 * MenuItem.cpp
 *
 *  Created on: Mar 18, 2010
 *      Author: romain
 */

#include "MenuItem.h"

/**
 * Constructor
 *
 * @param str	Text to be shown
 * @param frame		Pointer to the parent frame
 */
MenuItem::MenuItem(char *str, Widget *frame) {
	text=str;

	MAWidgetParameters itemParams;

	strncpy(itemParams.buf, text, strlen(text) + 1);
	itemParams.widgetID=myid;
	itemParams.pParent=frame->getInstance();
	maWinMobileAddRightMenuItem(&itemParams, NULL);

	maAndroidAddMenuItem(myid, text);
}

/**
 * Destructor
 */
MenuItem::~MenuItem() {

}

/**
 * Returns the widget's ID
 *
 * @return ID of the widget
 */
int MenuItem::getId() {
	Widget::getId();
}

/**
 * Processes events sent by the Manager.
 * Called by the Manager. You should not call it yourself.
 *
 * @param	MAEvent to be processed
 */
void MenuItem::processEvent(const MAEvent & e) {
	//if(e.hi_wparam==0) {
		actionListener->onClick(e.lo_wparam);
	//}
}

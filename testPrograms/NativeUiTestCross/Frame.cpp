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
 * Frame.cpp
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#include "Frame.h"

/**
 * Constructor
 */
Frame::Frame(int id) {
	myid=id;

	MAWidgetParameters wp;
	MAWidgetHandle handle;

	wp.pParent=NULL;
	wp.widgetID=myid;
	maWinMobileWindow(&wp, &handle);
	me=handle.pWidget;

	maAndroidStartActivity(myid);

	MAWidgetParameters mp;
	mp.pParent=me;
	maWinMobileMenuBar(&mp);
	
	maIPhoneView(&wp, &handle);
	me=handle.pWidget;

}

/**
 * Destructor
 */
Frame::~Frame() {

}

/**
 * Returns the widget's ID
 *
 * @return ID of the widget
 */
int Frame::getId() {
	Widget::getId();
}

/**
 * Returns the widget's instance INSIDE the runtime
 * (Used for Windows mobile).
 * You normally do not want to call it yourself !
 *
 * @return	Pointer to the widget instance inside the runtime:
 * 			This is dangerous !
 */
void *Frame::getInstance() {
	Widget::getInstance();
}

/**
 * Processes events sent by the Manager.
 * Called by the Manager. You should not call it yourself.
 *
 * @param	MAEvent to be processed
 */
void Frame::processEvent(const MAEvent &e) {
	if(e.hi_wparam==0) {
		actionListener->onCreate(e.lo_wparam);
	}
}

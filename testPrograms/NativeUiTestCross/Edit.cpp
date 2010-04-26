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
 * Edit.cpp
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#include "Edit.h"

/**
 * Constructor
 *
 * @param str	Text to be shown
 */
Edit::Edit(const char *str, int id) {
	text=str;
	myid=id;
}

/**
 * Actually builds the widget natively.
 * This method is to be overwritten in derived classes.
 * Called by the Layout. You should not call it yourself.
 *
 * @param x		Horizontal position
 * @param y		Vertical position
 * @param h		Height
 * @param l		Length
 * @param f		Pointer to the runtime instance of
 * 				the parent frame
 */
void Edit::build(int x, int y, int h, int l, void *f) {
	MAWidgetParameters editParams;
	editParams.pParent=f;
	editParams.posX=x;
	editParams.posY=y;
	editParams.sizeX=l;
	editParams.sizeY=h;
	editParams.widgetID=myid;
	strncpy(editParams.buf, text, strlen(text) + 1);
	MAWidgetHandle editHandle;
	maWinMobileEdit(&editParams, &editHandle);
	me=editHandle.pWidget;

	maAndroidAddEditText(myid, text);
	
	maIPhoneEdit(&editParams, &editHandle);
	me=editHandle.pWidget;
}


/**
 * Destructor
 */
Edit::~Edit() {

}

/**
 * Returns the widget's ID
 *
 * @return ID of the widget
 */
int Edit::getId() {
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
void *Edit::getInstance() {
	Widget::getInstance();
}

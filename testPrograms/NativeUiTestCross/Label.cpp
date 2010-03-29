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
 * Label.cpp
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#include "Label.h"

/**
 * Constructor
 *
 * @param str	Text to be shown
 */
Label::Label (const char *str, int id) {
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
void Label::build(int x, int y, int h, int l, void *f) {
	MAWidgetParameters labelParams;
	labelParams.pParent=f;
	labelParams.widgetID=myid;
	labelParams.posX=x;
	labelParams.posY=y;
	labelParams.sizeX=l;
	labelParams.sizeY=h;
	strncpy(labelParams.buf, text, strlen(text) + 1);
	MAWidgetHandle labelHandle;
	maWinMobileLabel(&labelParams, NULL);
	me=labelHandle.pWidget;

	maAndroidAddTextView(myid, text);
}

/**
 * Destructor
 */
Label::~Label() {

}

/**
 * Returns the widget's ID
 *
 * @return ID of the widget
 */
int Label::getId() {
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
void *Label::getInstance() {
	Widget::getInstance();
}

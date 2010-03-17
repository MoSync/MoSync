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
 * Layout.cpp
 *
 *  Created on: Mar 16, 2010
 *      Author: romain
 */

#include "Layout.h"
using namespace MAUtil;

/**
 * Constructor
 *
 * @param f		Pointer to the parent Frame.
 */
Layout::Layout(Frame *f) {
	parentFrame=f;
}

/**
 * Destructor
 */
Layout::~Layout() {

}

/**
 * Adds a widget to the layout.
 *
 * @param w		Pointer to the widget to be added.
 */
void Layout::addWidget(Widget *w) {
	set.insert(w);
}

/**
 * Calls every added widget's build method with
 * appropriate parameters.
 * Actually builds the layout natively.
 */
void Layout::build() {
	int x = 10;
	int y = 10;
	int h = 60;

	int l = ((short)(( maGetScrSize() ) >> 16)) - 20;

	for( Set<Widget*>::Iterator i = set.begin(); i != set.end(); i++) {
		(*i)->build(x, y, h, l, parentFrame->getInstance());
		y = y + h + 10;
	}

}

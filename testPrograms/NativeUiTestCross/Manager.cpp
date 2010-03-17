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
 * Manager.cpp
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#include "Manager.h"
using namespace MAUtil;

/**
 * Private constructor
 */
Manager::Manager() {

}

/**
 * Private destructor.
 */
Manager::~Manager() {

}

/**
 * Returns the unique instance of Manager.
 *
 * @return	Reference to the Manager.
 */
Manager& Manager::Instance() {
	static Manager m;
	return m;
}

/**
 * Adds widget to the widget map.
 *
 * @param w		Pointer to the widget to be added.
 */
void Manager::addWidget(Widget* w) {
	map.insert((w->getId()), w);
}

/**
 * Dispatches events coming from the runtime
 * to the appropriate widget.
 *
 * @param	Reference to the event to be dispatched.
 */
void Manager::dispatch(const MAEvent& e) {
	Map<int, Widget*>::Iterator i = map.find(e.lo_wparam);
	(*i).second->processEvent(e);
}

/**
 * Returns the next generated widget ID.
 * Called by Widget::Widget()
 * Not for the end user !
 */
int Manager::getNextId() {
	generatedId++;
	return generatedId;
}

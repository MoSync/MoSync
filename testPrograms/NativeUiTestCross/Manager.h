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
 * Manager.h
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#ifndef MANAGER_H
#define MANAGER_H

#include <ma.h>
#include <maassert.h>
#include <mastring.h>
#include <IX_NATIVE_UI.h>
#include <MAUtil\Map.h>
#include "Widget.h"

/**
 * This class is a widget manager.
 * Each widget created MUST be added to it.
 * This class is a singleton.
 */
class Manager
{
private:
	MAUtil::Map<int, Widget*> map;
	int generatedId;

public:
	/**
	 * Returns the unique instance of Manager.
	 *
	 * @return	Reference to the Manager.
	 */
	static Manager& Instance();

	/**
	 * Adds widget to the widget map.
	 *
	 * @param w		Pointer to the widget to be added.
	 */
	void addWidget(Widget *w);

	/**
	 * Dispatches events coming from the runtime
	 * to the appropriate widget.
	 *
	 * @param	Reference to the event to be dispatched.
	 */
	void dispatch(const MAEvent &);

	/**
	 * Finds a widget by its ID.
	 *
	 * @param id	ID of the widget to be found.
	 */
	Widget *findById(int id);

	/**
	 * Returns the next generated widget ID.
	 * Called by Widget::Widget()
	 * Not for the end user !
	 */
	int getNextId();

private:
	/**
	 * Private constructor
	 */
	Manager();

	/**
	 * Private destructor.
	 */
	~Manager();

	/**
	 * Prevents copy-construction.
	 */
	Manager(const Manager&);

	/**
	 * Prevents assignment.
	 */
	Manager& operator=(const Manager&);

};

#endif

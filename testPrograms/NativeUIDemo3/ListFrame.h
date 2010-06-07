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
 * ListFrame.h
 *
 *  Created on: May 4, 2010
 *      Author: Romain Chalant
 */

#ifndef LISTFRAME_H
#define LISTFRAME_H

#include <ma.h>
#include <conprint.h>
#include <IX_NATIVE_UI.h>
#include "Widget.h"

/**
 * This class is an implementation of
 * a list box window
 */
class ListFrame : public Widget {
public:
	/**
	 * Constructor
	 */
	ListFrame();

	/**
	 * Constructor by ID
	 */
	ListFrame(int id);

	/**
	 * Destructor
	 */
	~ListFrame();

	/**
	 * Returns the widget's ID
	 *
	 * @return ID of the widget
	 */
	int getId();

	/**
	 * Returns the widget's instance INSIDE the runtime
	 * (Used for Windows mobile).
	 * You normally do not want to call it yourself !
	 *
	 * @return	Pointer to the widget instance inside the runtime:
	 * 			This is dangerous !
	 */
	void *getInstance();

	/**
	 * Processes events sent by the Manager.
	 * Called by the Manager. You should not call it yourself.
	 *
	 * @param	MAEvent to be processed
	 */
	void processEvent(const MAEvent &);
	
	/**
	 * Adds an item to the list
	 *
	 * @param str	Text to be displayed
	 */
	void add(char *str);
	
	/**
	 * Shows the list box window.
	 *
	 * 
	 */
	void show();



};

#endif

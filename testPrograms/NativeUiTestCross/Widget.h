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
 * Widget.h
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <ma.h>
#include <IX_NATIVE_UI.h>
#include "ActionListener.h"

/**
 * This class is the base class for
 * all the widgets such as buttons, labels...
 * You should not need to instantiate it !
 */

class Widget {
public:
	/**
	 * Default constructor
	 */
	Widget();

	/**
	 * Constructor by ID
	 */
	Widget(int id);

	/**
	 * Destructor
	 */
	~Widget();

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
	virtual void processEvent(const MAEvent &);

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
	virtual void build(int x, int y, int h, int l, void *f);

	/**
	 * Adds an ActionListener class to the widget.
	 * Needed for event handling.
	 *
	 * @param a		Pointer to the ActionListener.
	 */
	void addActionListener(ActionListener *a);

	/**
	 * Operator needed by the Manager
	 */
	bool operator < ( const Widget & ) const;

	/**
	 * Operator needed by the Manager
	 */
	bool operator == ( const Widget & ) const;

protected:
	/**
	 * Protected fields common to all widgets.
	 */
	int myid;
	void *me;
	char *text;
	ActionListener *actionListener;
};

#endif

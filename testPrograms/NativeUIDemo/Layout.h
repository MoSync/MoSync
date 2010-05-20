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
 * Layout.h
 *
 *  Created on: Mar 16, 2010
 *      Author: romain
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_
#include "Widget.h"
#include "Frame.h"
#include <MAUtil/Set.h>

/**
 * This class is an implementation of
 * a widget layout and the base class of
 * all the layouts.
 * Each widget MUST belong to a layout.
 * Each layout must have a parent frame.
 */
class Layout {
public :
	/**
	 * Constructor
	 *
	 * @param f		Pointer to the parent Frame.
	 */
	Layout(Frame *f);

	/**
	 * Destructor
	 */
	~Layout();

	/**
	 * Adds a widget to the layout.
	 *
	 * @param w		Pointer to the widget to be added.
	 */
	void addWidget(Widget *w);

	/**
	 * Calls every added widget's build method with
	 * appropriate parameters.
	 * Actually builds the layout natively.
	 */
	void build();

protected:
	MAUtil::Set<Widget*> set;
	Frame *parentFrame;
};

#endif /* LAYOUT_H_ */

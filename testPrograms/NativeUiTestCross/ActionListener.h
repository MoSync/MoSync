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
 * ActionListener.h
 *
 *  Created on: Mar 15, 2010
 *      Author: Romain Chalant
 */

#ifndef ACTIONLISTENER_H_
#define ACTIONLISTENER_H_

/**
 * This class is an interface
 * for event handling
 */

class ActionListener {
public:
	/**
	 * The onCreate method shall be
	 * overwritten to contain the user's code
	 *
	 * @param id	ID of the widget that is created
	 */
	virtual void onCreate(int id) = 0 ;

	/**
	 * The onClick method shall be
	 * overwritten to contain the user's code
	 *
	 * @param id	ID of the widget that is clicked
	 */
	virtual void onClick(int id) = 0 ;

	/**
	 * The onItemSelected method shall be
	 * overwritten to contain the user's code
	 *
	 * @param index	Index of the item that is selected
	 */
	virtual void onItemSelected(int index) = 0 ;
};

#endif /* ACTIONLISTENER_H_ */

/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file HomeScreenListener.h
 * @author Bogdan Iusco.
 * @brief Listener for home screen class.
 * Notify listener to show the tab screen.
 */

#ifndef HOMESCREENLISTENER_H_
#define HOMESCREENLISTENER_H_

/**
 * @brief Listener for home screen class.
 * Notify listener to show the tab screen.
 */
class HomeScreenListener
{
public:
	/**
	 * Show the tab screen.
	 */
	virtual void showTabScreen() = 0;
};


#endif /* HOMESCREENLISTENER_H_ */

/*
 Copyright (C) 2012 MoSync AB

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
 * @file ApplicationController.h
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief Application's controller.
 */

#ifndef APPLICATIONCONTROLLER_H_
#define APPLICATIONCONTROLLER_H_

#include <NativeUI/TabScreen.h>

// Forward declaration
class IndexedListScreen;

using namespace NativeUI;

class ApplicationController
{
public:
	/**
	 * Constructor.
	 */
	ApplicationController();

	/**
	 * Destructor.
	 */
	~ApplicationController();

private:
	/**
	 * Create screens and add them to tab screen.
	 */
	void createScreens();

private:
	/**
	 * Application's tab screen.
	 */
	TabScreen* mTabScreen;

	/**
	 * 1st tab screen.
	 * Shows an alphabetical list.
	 */
	IndexedListScreen* mIndexedListViewScreen;
};


#endif /* APPLICATIONCONTROLLER_H_ */

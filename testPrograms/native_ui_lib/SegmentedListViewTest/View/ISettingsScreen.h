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
 * @file ISettingsScreen.h
 * @author Bogdan Iusco
 * @date 25 June 2012
 *
 * @brief Listener for settings screen.
 */

#ifndef ISETTINGSSCREEN_H_
#define ISETTINGSSCREEN_H_

#include "../wrapper/SegmentedListView.h"

using namespace NativeUI;

class ISettingsScreen
{
public:
	/**
	 * Allow editing value has changed.
	 * @param state True if allowed, false otherwise.
	 */
	virtual void allowEditing(bool state) = 0;

	/**
	 * Allow moving list items value has changed.
	 * @param state True if allowed, false otherwise.
	 */
	virtual void allowMoving(bool state) = 0;

	/**
	 * List mode has changed.
	 * @param listMode The new list mode.
	 */
	virtual void listModeChanged(SegmentedListViewMode listMode) = 0;
};


#endif /* ISETTINGSSCREEN_H_ */

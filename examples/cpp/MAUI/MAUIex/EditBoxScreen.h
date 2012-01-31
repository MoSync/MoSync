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

#ifndef _EDITBOXSCREEN_H_
#define _EDITBOXSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

using namespace MAUI;

/**
 *  The screen class used for demonstrating how to
 *  use \a EditBoxes and their different modes.
 */
class EditBoxScreen : public Screen, WidgetListener {
public:
	/**
	 * Constructor
	 * Sets up the UI hierarchy for this screen, filling
	 * it with a number of \a EditBoxes.
	 * @param previous a pointer to the screen to return to
	 */
	EditBoxScreen(Screen *previous);
	/**
	 * Destructor
	 */
	~EditBoxScreen();
	/**
	 * Recieves key presses and performs appropriate interaction
	 * with the UI.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);

	/**
	 * Implementation of a \a WidgetListener callback, which
	 * is notified whenever the selection state of a \a widget
	 * it's listeting to changes. I our case, we make sure that
	 * whenever a widget is selected, we make its first child
	 * selected instead.
	 */
	void selectionChanged(Widget *widget, bool selected);
	/**
	 * Overload of MAUI::Screen::show().
	 */
	void show();

private:
	void hide();

	Screen *previous;
	ListBox* listBox;
	Layout* mainLayout;
	Widget *softKeys;
};


#endif	//_EDITBOXSCREEN_H_

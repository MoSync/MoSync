/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

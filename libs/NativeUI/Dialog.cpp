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
 * @file Dialog.cpp
 * @author emma
 *
 * \brief Class that represents a modal dialog.
 * A dialog cannot have any parent, it acts only as a container of other widgets.
 * A dialog is a sort of modal view, that can look different depending on
 * the platform:
 *  - On Android it is a modal alert dialog.
 *  - On iPad it is a PopoverController, and on iPhone it is a view.
 *
 * When a Dialog widget is created it is empty, it has no content.
 * Use setMainWidget(widget) to set the main widget of the dialog.
 *
 * A Dialog gets visible only after calling show() method.
 * To show a Dialog call show(), to hide it call: hide().
 */

#include "Dialog.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	Dialog::Dialog() :
		Widget(MAW_MODAL_DIALOG)
	{
	}

	/**
	 * Destructor.
	 */
	Dialog::~Dialog()
	{
	}

	/**
	 * Set the title of the screen.
	 * The title is used by tab screen to display a text on the tab indicator.
	 * @param title The screen title.
	 */
	void Dialog::setTitle(const MAUtil::String& title)
	{
		setProperty(MAW_MODAL_DIALOG_TITLE, title.c_str());
	}

	/**
	 * Set the main widget of the dialog.
	 * Note: A dialog can only have one main widget.
	 * Use a layout as main widget and add child  widgets to the layout.
	 * @param widget The widget to be set as main widget.
	 * The ownership of the widget is passed to this function.
	 * When the dialog will be destroyed, the widget and it's child widgets
	 * will be deleted.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be added to the parent.
	 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 */
	int Dialog::setMainWidget(Widget* widget)
	{
	    int nrChildren = this->countChildWidgets();
	    for (int i = 0; i < nrChildren; i++)
	    {
	        this->removeChild(this->getChild(i));
	    }

	    return addChild(widget);
	}

	/**
	 * Shows the dialog. Only one dialog at a time is visible.
	 * It will become visible on top of the UI.
	 */
	void Dialog::show()
	{
		maWidgetModalDialogShow(getWidgetHandle());
	}

	/**
	 * Hides a dialog if it is visible.
	 */
	void Dialog::hide()
	{
		maWidgetModalDialogHide(getWidgetHandle());
	}

} // namespace NativeUI

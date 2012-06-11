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
 * @file MainScreen.h
 * @author emma
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public EditBoxListener
{

public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox){};

    /**
     * This method is called when an edit box gains focus.
     * The virtual keyboard is shown.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidBegin(EditBox* editBox);

    /**
     * This method is called when an edit box loses focus.
     * The virtual keyboard is hidden.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidEnd(EditBox* editBox);

    /**
     * This method is called when the edit box text was changed.
     * @param editBox The edit box object that generated the event.
     * @param text The new text.
     */
    virtual void editBoxTextChanged(
        EditBox* editBox,
        const MAUtil::String& text);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	Label* mLabel;
	ListView* mEventsList;
	EditBox* mEditBox;
};


#endif /* MAINSCREEN_H_ */

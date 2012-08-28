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
 * @file FirstScreen.h.
 * @author emma
 */

#ifndef FIRSTSCREEN_H_
#define FIRSTSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class FirstScreen:
	public Screen,
	public EditBoxListener
{

public:
	/**
	 * Constructor.
	 */
	FirstScreen();

	/**
	 * Destructor.
	 */
	~FirstScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();
	int getEditBoxIndex(EditBox& editBox);

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
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	Label* mLabel;
	ListView* mEventsList;
	EditBox* mEditBox1;
	EditBox* mEditBox2;
	EditBox* mEditBox3;
};

#endif /* FIRSTSCREEN_H_ */

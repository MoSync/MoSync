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
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <IX_PIM.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <NativeUI/OptionsDialog.h>
#include <NativeUI/OptionsDialogListener.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public ButtonListener,
	public OptionsDialogListener
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

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button);

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the destructive button from options
     * dialog was clicked.
     */
    virtual void optionsDialogDestructiveButtonClicked();

    /**
     * This method is called when the cancel button from options dialog was
     * clicked.
     */
    virtual void optionsDialogCancelButtonClicked();

    /**
     * This method is called when a button from options dialog was
     * clicked.
     * This method is not called if the destructive or cancel button were
     * clicked.
     * @param buttonIndex The index of the button that was clicked.
     * @param buttonTitle The title of the button that was clicked.
     */
    virtual void optionsDialogButtonClicked(
			const int buttonIndex,
			const MAUtil::WString& buttonTitle);

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Write n wchar-arrays to buffer.
	 * @param buffer The given buffer.
	 * The first value in buffer will be the number of wchar-arrays(n argument).
	 * @param src Contains the n wchar-arrays that will be written into buffer.
	 * @return The number of written bytes.
	 */
	int MainScreen::writeWCharArraysToBuf(
	    void* buffer,
	    const wchar_t** src,
	    const int n);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	Button* mButton;
};


#endif /* MAINSCREEN_H_ */

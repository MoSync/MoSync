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
 * @file ScreenWebView.h
 * @author Bogdan Iusco.
 *
 * Class that creates a screen that displays a web view.
 */

#ifndef SCREEN_WEB_VIEW_H_
#define SCREEN_WEB_VIEW_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Class that creates a screen that displays a web view.
 */
class ScreenWebView:
	public Screen,
	public ButtonListener,
	public EditBoxListener
{
public:
	/**
	 * Constructor.
	 */
	ScreenWebView();

	/**
	 * Destructor.
	 */
	virtual ~ScreenWebView();

    /**
     * This method is called if the touch-up event was inside the
     *  bounds of the button.
     *  @param button The button object that generated the event.
     */
     virtual void buttonClicked(Widget* button);

     /**
      * This method is called when the return button was pressed.
      * On iphone platform the virtual keyboard is not hidden after
      * receiving this event.
      * @param editBox The edit box object that generated the event.
      */
     virtual void editBoxReturn(EditBox* editBox);

private:

	/**
	 * Creates the address bar widgets and layouts.
	 */
	void createAddressBar();

	/**
	 * Creates an empty vertical layout.
	 * @param width Layout width.
	 * @param height Layout height.
	 * @return A pointer to a vertical layout widget.
	 * 		   The ownership of the result is passed to the caller!
	 */
	VerticalLayout* createSpacer(const int width,const int height);

	/**
	 * Open a web page.
	 * @param url The desired URL web page.
	 */
	virtual void openURL(const MAUtil::String& url);

private:

	/**
	 * Web view widget.
	 */
	WebView* mWebView;

	/**
	 * A simple navigation bar for iOS devices.
	 * It is used only for displaying a text(the title of the screen).
	 */
	NavigationBar* mNavBarWidget;

	/**
	 * Widget used for opening the given web address.
	 */
	Button* mOpenLinkButtonWidget;

	/**
	 * Used by the user for providing a web address.
	 */
	EditBox* mEditBoxWidget;

	/**
	 * Address bar layout.
	 * Will contain the edit box widget, open link button widget
	 * and the show/hide button widget.
	 */
	HorizontalLayout* mAddressBarLayout;

	/**
	 * A simple colored layout.
	 */
	HorizontalLayout* mLineLayout;

	/**
	 * Screen width.
	 */
	int mScreenWidth;

	/**
	 * Screen height.
	 */
	int mScreenHeight;
};

#endif // SCREEN_WEB_VIEW_H_

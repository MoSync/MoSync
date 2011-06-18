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
#include "UIWrapper/Widgets.h"

using namespace MoSync::UI;

/**
 * Class that creates a screen that displays a web view.
 */
class ScreenWebView:
    public WidgetEventListener // Used for listening widget events.
{
public:
	/**
	 * Create the web view screen.
	 */
	static Screen* create();

	/**
	 * Destroy the web view screen.
	 */
	static void destroy();

private:
	/**
	 * Constructor.
	 */
	ScreenWebView();

	/**
	 * Destructor.
	 */
	~ScreenWebView();

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
	 * This method is called when there is an event for a widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	virtual void handleWidgetEvent(
		Widget* widget,
		MAWidgetEventData* widgetEventData);

	/**
	 * Returns a pointer to the screen object.
	 * Ownership is not passed to the caller!
	 */
	Screen* getScreen() const;

	/**
	 * Open a web page.
	 * @param url The desired URL web page.
	 */
	virtual void openURL(const MAUtil::String& url);

private:
	/**
	 * Static ScreenWebView object(singleton design pattern).
	 */
	static ScreenWebView* sScreenWebView;

	/**
	 * A screen for this view.
	 */
	Screen* mScreen;

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

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

// Include the library for event handling listener.
#include "MAUtil/Environment.h"
// Include all the wrappers.
#include "UIWrapper/Widgets.h"

using namespace MoSync::UI;

/**
 * Navigation bar animation states.
 */
enum eNavigationBarAnimation
{
	NONE = 1,
	FADE_OUT_NAVIGATION_BAR,
	SHOW_EXPAND_WIDGET,
	HIDE_EXPAND_WIDGET,
	FADE_IN_NAVIGATION_BAR
};

/**
 * Class that creates a screen that displays a web view.
 */
class ScreenWebView: public WidgetEventListener, // used for listening widget events.
					 public MAUtil::TimerListener // used for timer notification.
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

	/**
	 * Sets the default URL address for the web widget.
	 * @param url The desired URL web page.
	 */
	static void setDefaultURL(const MAUtil::String url);

	/**
	 * This method is called when there is an event for a widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	virtual void handleWidgetEvent(
		Widget* widget,
		MAWidgetEventData* widgetEventData);

	/**
	 * Called on timer events.
	 * It is used for navigation bar animation.
	 */
	void runTimerEvent();

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
	 * Constructor.
	 */
	ScreenWebView();

	/**
	 * Destructor.
	 */
	~ScreenWebView();

	/**
	 * Creates the navigation bar.
	 */
	void createNavigationBar();

	/**
	 * Creates an empty vertical layout.
	 * @param width Layout width.
	 * @param height Layout height.
	 * @return A pointer to a vertical layout widget.
	 * 		   The ownership of the result is passed to the caller!
	 */
	VerticalLayout* createSpacer(const int width,const int height);

	/**
	 * Fades out the navigation bar.
	 */
	void fadeOutNavigationBar();

	/**
	 * Shows the expand image button.
	 */
	void showExpandWidget();

	/**
	 * Hides the expand image button.
	 */
	void hideExpandWidget();

	/**
	 * Fades in the navigation bar.
	 */
	void fadeInNavigationBar();

private:

	/**
	 * Static ScreenWebView object(singleton design pattern).
	 */
	static ScreenWebView* mScreenWebView;

	/**
	 * The URL opened by the web view when the screen is showed.
	 */
	static 	MAUtil::String mURL;

	/**
	 * A screen for this view.
	 */
	Screen* mScreen;

	/**
	 * Web view widget.
	 */
	WebView* mWebView;

	/**
	 * Widget used for opening the given web address.
	 */
	ImageButton* mOpenLinkButtonWidget;

	/**
	 * According with state of the navigation bar(collapsed or expanded),
	 * the widget will display a show or hide image.
	 * It is used for collapsing or expanding the navigation bar.
	 */
	ImageButton* mHideShowButtonWidget;

	/**
	 * Used by the user for providing a web address.
	 */
	EditBox* mEditBoxWidget;

	/**
	 * Navigation bar layout.
	 * Will contain the edit box widget, open link button widget
	 * and the show/hide button widget.
	 */
	HorizontalLayout* mNavigationBarLayout;

	/**
	 * A simple colored layout.
	 */
	HorizontalLayout* mLineLayout;

	/**
	 * Edit box layout.
	 * Will contain a spacer and the edit box widget.
	 */
	VerticalLayout* mEditBoxLayout;

	/**
	 * Open link button layout.
	 * Will contain a spacer and the open link button widget.
	 */
	VerticalLayout* mOpenLinkBtnLayout;

	/**
	 * Hide/show button layout.
	 * Will contain a spacer and the hide/show button widget.
	 */
	VerticalLayout* mHideShowBtnLayout;

	/**
	 * The current state of the navigation bar animation.
	 */
	eNavigationBarAnimation mNavigationBarAnimation;

	/**
	 * The current navigation bar height.
	 * Used by the animation.
	 */
	int mCurrentNavigationBarHeight;

	/**
	 * The current navigation bar alpha.
	 * Used by the animation.
	 */
	float mCurrentNavigationBarAlpha;

	/**
	 * Screen width.
	 */
	int mScreenWidth;

	/**
	 * Screen height.
	 */
	int mScreenHeight;

	/**
	 * The flag is set if the navigation bar is visible.
	 */
	BOOL mIsNavigationBarVisible;
};

#endif

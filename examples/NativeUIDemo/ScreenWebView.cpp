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
 * Class that creates a screen that displays a web view screen.
 */

// The default URL address.
#define DEFAULT_URL_ADDRESS "http://www.mosync.com"

// The name of the screen.
#define SCREEN_TITLE "Web View"

// The text that will be showed on the tab widget.
#define TAB_SCREEN_TITLE "Web"

// The height of the line layout.
#define LINE_LAYOUT_HEIGHT 2

// The background color of the line layout.
#define LINE_LAYOUT_BG_COLOR 0xC0C0C0

// False constant.
#define FALSE_CONST "false"

// Include the resources for images.
#include "MAHeaders.h"

#include "Util.h"

// Include the web view screen.
#include "ScreenWebView.h"

/**
 * Constructor.
 * @param Screen The widgets will be added on this screen.
 * Initialize the objects.
 */
ScreenWebView::ScreenWebView():
	Screen(),
	mWebView(NULL),
	mNavBarWidget(NULL),
	mOpenLinkButtonWidget(NULL),
	mEditBoxWidget(NULL),
	mAddressBarLayout(NULL),
	mLineLayout(NULL),
	mScreenWidth(0),
	mScreenHeight(0)
{
	setTitle(TAB_SCREEN_TITLE);

	// Get the screen size. This shows how to get a property
	// value for a property for which there is no predefined method.
	mScreenHeight = getHeight();
	mScreenWidth = getWidth();

	// Create and add the main layout to the screen.
	VerticalLayout* mainLayout = new VerticalLayout();
	setMainWidget(mainLayout);

	if (isAndroid())
	{
		// Set the screen icon for Android.
		setIcon(RES_TAB_ICON_WEB_VIEW_ANDROID);

		// Get the screen size.
		MAExtent screenSize = maGetScrSize();
		mScreenWidth = EXTENT_X(screenSize);
		mScreenHeight = EXTENT_Y(screenSize);
	}
	else
	{
		// Set the screen icon for iOS.
		setIcon(RES_TAB_ICON_WEB_VIEW);

		// Create and add the navigation bar to the main layout.
		mNavBarWidget = new NavigationBar();
		mNavBarWidget->setTitle(SCREEN_TITLE);
		mNavBarWidget->setBackButtonTitle("");
		mNavBarWidget->fillSpaceHorizontally();
		mainLayout->addChild(mNavBarWidget);
	}

	// Create and add the address bar to the main layout.
	createAddressBar();
	mainLayout->addChild(mAddressBarLayout);

	// Create and add the line layout to the main layout.
	mLineLayout = new HorizontalLayout();
	mLineLayout->setSize(
		MAW_CONSTANT_FILL_AVAILABLE_SPACE,
		LINE_LAYOUT_HEIGHT);
	mLineLayout->setBackgroundColor(LINE_LAYOUT_BG_COLOR);
	mainLayout->addChild(mLineLayout);

	// Create and add the web view widget to the main layout.
	mWebView = new WebView();
	mWebView->fillSpaceHorizontally();
	mWebView->fillSpaceVertically();
	mWebView->openURL(DEFAULT_URL_ADDRESS);
	mainLayout->addChild(mWebView);
}

/**
 * Destructor.
 */
ScreenWebView::~ScreenWebView()
{
	mOpenLinkButtonWidget->removeButtonListener(this);
	mEditBoxWidget->removeEditBoxListener(this);
}

/**
 * Creates the address bar.
 */
void ScreenWebView::createAddressBar()
{
	// Create the address layout.
	mAddressBarLayout = new HorizontalLayout();
	mAddressBarLayout->fillSpaceHorizontally();
	mAddressBarLayout->wrapContentVertically();

	// This shows how you set a property for which there
	// is no predefined method.
	mAddressBarLayout->setProperty(
		MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Create the edit box widget.
	mEditBoxWidget = new EditBox();
	mEditBoxWidget->setEditMode(EDIT_BOX_MODE_TEXT);
	mEditBoxWidget->setText(DEFAULT_URL_ADDRESS);
	mEditBoxWidget->fillSpaceHorizontally();
	mEditBoxWidget->wrapContentVertically();
	mEditBoxWidget->addEditBoxListener(this);

	// Create the open link button widget.
	mOpenLinkButtonWidget = new Button();
	mOpenLinkButtonWidget->setText("Open");
	mOpenLinkButtonWidget->wrapContentHorizontally();
	mOpenLinkButtonWidget->wrapContentVertically();
	mOpenLinkButtonWidget->addButtonListener(this);

	// Add widgets to the address bar layout.
	mAddressBarLayout->addChild(mEditBoxWidget);
	mAddressBarLayout->addChild(mOpenLinkButtonWidget);
}

/**
 * Creates an empty vertical layout.
 * @param width Layout width.
 * @param height Layout height.
 * @return A vertical layout widget.
 * 		   The ownership of the result is passed to the caller!
 */
VerticalLayout* ScreenWebView::createSpacer(const int width, const int height)
{
	VerticalLayout* layout = new VerticalLayout();
	layout->setSize(width, height);

	// This shows how you set a property for which there is no
	// predefined method.
	layout->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);
	layout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);

	return layout;
}

/**
 * This method is called if the touch-up event was inside the
 *  bounds of the button.
 *  @param button The button object that generated the event.
 */
void ScreenWebView::buttonClicked(Widget* button)
{
	if ( mOpenLinkButtonWidget == button )
	{
		//Hide the virtual keyboard. This is how you set a property
		//for which there is no predefined method.
		mEditBoxWidget->hideKeyboard();

		// Open the web page.
		mWebView->openURL( mEditBoxWidget->getText() );
	}
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void ScreenWebView::editBoxReturn(EditBox* editBox)
{
	if ( mEditBoxWidget == editBox )
	{
		//Hide the virtual keyboard. This is how you set a property
		//for which there is no predefined method.
		mEditBoxWidget->hideKeyboard();

		// Open the web page.
		mWebView->openURL( mEditBoxWidget->getText() );
	}
}

/**
 * This method is called when there is an event for a widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
// void ScreenWebView::handleWidgetEvent(
//	Widget* widget,
//	MAWidgetEventData* widgetEventData)
//{
//	// Check that the event was a click (touch) event.
//	if (MAW_EVENT_CLICKED == widgetEventData->eventType)
//	{
//		if (widget == mEditBoxWidget || widget == mOpenLinkButtonWidget)
//		{
//			// Hide the virtual keyboard. This is how you set a property
//			// for which there is no predefined method.
//			mEditBoxWidget->setProperty(
//				MAW_EDIT_BOX_SHOW_KEYBOARD,
//				FALSE_CONST);
//
//			// This is how can get a property value for which there
//			// is no predefined method.
//			MAUtil::String webAddress =
//				mEditBoxWidget->getPropertyString("text");
//
//			// Open the web page.
//			mWebView->openURL(webAddress);
//		}
//	}
//}

/**
 * Open a web page.
 * @param url The desired URL web page.
 */
void ScreenWebView::openURL(const MAUtil::String& url)
{
	mWebView->openURL(url);
	mEditBoxWidget->setText(url);
}

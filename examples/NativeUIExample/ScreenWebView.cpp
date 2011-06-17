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

// The width of the edit box in percentage (x% of the screen width).
#define EDIT_BOX_WIDTH 70.0

// The width of the open link button in percentage (x% of the screen width)
#define BUTTON_WIDTH 30.0

// The default height of the address bar.
#define DEFAULT_ADDRESS_BAR_HEIGHT 55

// Space between widgets.
#define SPACE 5

// The default URL address.
#define DEFAULT_URL_ADDRESS "http://www.google.com"

// The name of the screen.
#define SCREEN_TITLE "Web View"

// The text that will be showed on the tab widget.
#define TAB_SCREEN_TITLE "Web"

// The height of the line layout.
#define LINE_LAYOUT_HEIGHT 2

// The background color of the line layout.
#define LINE_LAYOUT_BG_COLOR 0xC0C0C0

// Constant for defining big screens.
#define LARGE_SCREEN_HEIGHT 800

// The increase height value for widgets(for large screens only).
#define LARGE_SCREEN_INCREASE_VALUE 30

// False constant.
#define FALSE_CONST "false"

// Include the resources for images.
#include "MAHeaders.h"

// Include the web view screen.
#include "ScreenWebView.h"

// Set the pointer to ScreenWebView object to null.
ScreenWebView* ScreenWebView::sScreenWebView = NULL;

/**
 * Create the web view screen.
 */
Screen* ScreenWebView::create()
{
	if (NULL == sScreenWebView) {
		sScreenWebView = new ScreenWebView();
	}

	return sScreenWebView->getScreen();
}

/**
 * Destroy the web view screen.
 */
void ScreenWebView::destroy()
{
	delete sScreenWebView;
	sScreenWebView = NULL;
}

/**
 * Constructor.
 * @param Screen The widgets will be added on this screen.
 * Initialize the objects.
 */
ScreenWebView::ScreenWebView():
		mScreen(NULL),
		mWebView(NULL),
		mNavBarWidget(NULL),
		mOpenLinkButtonWidget(NULL),
		mEditBoxWidget(NULL),
		mAddressBarLayout(NULL),
		mLineLayout(NULL),
		mEditBoxLayout(NULL),
		mOpenLinkBtnLayout(NULL),
		mScreenWidth(0),
		mScreenHeight(0),
		mAddressBarWidgetsHeight(0)
{
	mScreen = new Screen();
	mScreen->setTitle(TAB_SCREEN_TITLE);

	// Get the screen size.
	mScreenHeight = mScreen->getPropertyInt(MAW_WIDGET_HEIGHT);
	mScreenWidth = mScreen->getPropertyInt(MAW_WIDGET_WIDTH);

	// Create and add the main layout to the screen.
	VerticalLayout* mainLayout = new VerticalLayout();
	mScreen->setMainWidget(mainLayout);

	if (WidgetManager::isAndroid())
	{
		mScreen->setIcon(RES_TAB_ICON_WEB_VIEW_ANDROID);

		// Get the screen size.
		MAExtent screenSize = maGetScrSize();
		mScreenWidth = EXTENT_X(screenSize);
		mScreenHeight = EXTENT_Y(screenSize);
	}
	else
	{
		// Create and add the navigation bar to the main layout.
		mNavBarWidget = new NavigationBar();
		mNavBarWidget->setTitle(SCREEN_TITLE);
		mNavBarWidget->setBackButtonTitle("");
		mainLayout->addChild(mNavBarWidget);

		mScreen->setIcon(RES_TAB_ICON_WEB_VIEW);
	}

	// Create and add the address bar to the main layout.
	createAddressBar();
	mainLayout->addChild(mAddressBarLayout);

	// Create and add the line layout to the main layout.
	mLineLayout = new HorizontalLayout();
	mLineLayout->setSize(mScreenWidth, LINE_LAYOUT_HEIGHT);
	mLineLayout->setBackgroundColor(LINE_LAYOUT_BG_COLOR);
	mainLayout->addChild(mLineLayout);

	// Create and add the web view widget to the main layout.
	mWebView = new WebView();
	mWebView->openURL(DEFAULT_URL_ADDRESS);
	mainLayout->addChild(mWebView);
}

/**
 * Destructor.
 */
ScreenWebView::~ScreenWebView()
{
	mOpenLinkButtonWidget->setEventListener(NULL);
	mEditBoxWidget->setEventListener(NULL);

	delete mScreen;
	delete mWebView;
	delete mOpenLinkButtonWidget;
	delete mEditBoxWidget;
	delete mNavBarWidget;

	delete mAddressBarLayout;
	delete mLineLayout;
	delete mEditBoxLayout;
	delete mOpenLinkBtnLayout;
}

/**
 * Creates the address bar.
 */
void ScreenWebView::createAddressBar()
{
	float editBoxWidth = EDIT_BOX_WIDTH / 100.0 * mScreenWidth;
	float buttonWidth = BUTTON_WIDTH / 100.0 * mScreenWidth;
	mAddressBarHeight = DEFAULT_ADDRESS_BAR_HEIGHT;

	// For large screen increase the height of the widgets.
	if (LARGE_SCREEN_HEIGHT <= mScreenHeight)
	{
		mAddressBarHeight += LARGE_SCREEN_INCREASE_VALUE;
	}

	mAddressBarWidgetsHeight = mAddressBarHeight - (2 * SPACE);

	// Create the address layout.
	mAddressBarLayout = new HorizontalLayout();
	mAddressBarLayout->setSize(mScreenWidth, mAddressBarHeight);
	mAddressBarLayout->setProperty(
		MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Create the edit box widget.
	mEditBoxWidget = new EditBox();
	mEditBoxWidget->setTextInputMode();
	mEditBoxWidget->setProperty(MAW_EDIT_BOX_TEXT, DEFAULT_URL_ADDRESS);
	mEditBoxWidget->setProperty(
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	mEditBoxWidget->setEventListener(this);

	// Create the open link button widget.
	mOpenLinkButtonWidget = new ImageButton();
	mOpenLinkButtonWidget->setImage(RES_WEB_VIEW_OPEN_LINK_IMAGE);
	mOpenLinkButtonWidget->setSize(mAddressBarWidgetsHeight, mAddressBarWidgetsHeight);
	mOpenLinkButtonWidget->setEventListener(this);

	// Create the edit box layout.
	mEditBoxLayout = createSpacer((int)editBoxWidth, mAddressBarWidgetsHeight);
	mEditBoxLayout->addChild(mEditBoxWidget);

	// Create the open link button layout.
	mOpenLinkBtnLayout = createSpacer(
		(int)buttonWidth - (2 * SPACE),
		mAddressBarWidgetsHeight);
	mOpenLinkBtnLayout->setProperty(
			MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_RIGHT);
	mOpenLinkBtnLayout->addChild(mOpenLinkButtonWidget);

	// Add layouts to the address bar layout.
	mAddressBarLayout->addChild(mEditBoxLayout);
	mAddressBarLayout->addChild(mOpenLinkBtnLayout);
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
	layout->setProperty(
		MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_RIGHT);
	layout->setProperty(
		MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	return layout;
}

/**
 * This method is called when there is an event for a widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
 void ScreenWebView::handleWidgetEvent(
	Widget* widget,
	MAWidgetEventData* widgetEventData)
{
	// Check that the event was a click (touch) event.
	if (MAW_EVENT_CLICKED == widgetEventData->eventType)
	{
		if (widget == mEditBoxWidget || widget == mOpenLinkButtonWidget)
		{
			mEditBoxWidget->setProperty(MAW_EDIT_BOX_SHOW_KEYBOARD, FALSE_CONST);
			MAUtil::String webAddress = mEditBoxWidget->getPropertyString("text");
			mWebView->openURL(webAddress);
		}
	}
}

/**
 * Returns a pointer to the screen object.
 * Ownership is not passed to the caller!
 */
Screen* ScreenWebView::getScreen() const
{
	return mScreen;
}

/**
 * Open a web page.
 * @param url The desired URL web page.
 */
void ScreenWebView::openURL(const MAUtil::String& url)
{
	mWebView->openURL(url);
	mEditBoxWidget->setProperty(MAW_EDIT_BOX_TEXT, url);
}

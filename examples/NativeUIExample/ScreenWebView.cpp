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
#define EDIT_BOX_WIDTH 55.0

// The width of the open link button in percentage (x% of the screen width)
#define BUTTON_WIDTH 25.0

// The default height of the address bar(when it's expanded).
#define DEFAULT_MAX_ADDRESS_BAR_HEIGHT 55

// The default height of the address bar(when it's collapsed).
#define DEFAULT_MIN_ADDRESS_BAR_HEIGHT 20

// Increase/decrease height value of the address bar.
#define ANIMATION_HEIGHT_VALUE 8

// Increase/decrease alpha value of the address bar.
#define ANIMATION_ALPHA_VALUE 0.25

// Animation time.
#define ANIMATION_TIME_PERIOD 50

// The maximum alpha value of the address bar.
#define MAX_ALPHA_VALUE 1

// The minimum alpha value of the address bar.
#define MIN_ALPHA_VALUE 0

// Space between widgets.
#define SPACE 5

// The default URL address.
#define DEFAULT_URL_ADDRESS "http://www.google.com"

// The name of the screen.
#define SCREEN_TITLE "Web View"

// The text that will be showed on the tab widget.
#define TAB_SCREEN_TITLE "Web"

// The height of the title widget.
#define NAVIGATION_BAR_WIDGET_HEIGHT 30

// The height of the line layout.
#define LINE_LAYOUT_HEIGHT 2

// The background color of the line layout.
#define LINE_LAYOUT_BG_COLOR 0xC0C0C0

// Constant for defining big screens.
#define LARGE_SCREEN_HEIGHT 800

// The increase height value for widgets(for large screens only).
#define LARGE_SCREEN_INCREASE_VALUE 30

// True and false constants.
#define TRUE_CONST "true"
#define FALSE_CONST "false"

// Include the resources for images.
#include "MAHeaders.h"

// Include the web view screen.
#include "ScreenWebView.h"

// Set the pointer to ScreenWebView object to null.
ScreenWebView* ScreenWebView::sScreenWebView = NULL;

// Set the default URL address.
MAUtil::String ScreenWebView::sURL = DEFAULT_URL_ADDRESS;

/**
 * Create the web view screen.
 */
Screen* ScreenWebView::create()
{
	if(NULL == sScreenWebView) {
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
 * Sets the default URL address for the web widget.
 * @param url The desired URL web page.
 */
void ScreenWebView::setDefaultURL(const MAUtil::String url)
{
	sScreenWebView->openURL(url);
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
		mHideShowButtonWidget(NULL),
		mEditBoxWidget(NULL),
		mAddressBarLayout(NULL),
		mLineLayout(NULL),
		mEditBoxLayout(NULL),
		mOpenLinkBtnLayout(NULL),
		mHideShowBtnLayout(NULL),
		mAddressBarAnimation(NONE),
		mCurrentAddressBarHeight(0),
		mCurrentAddressBarAlpha(0),
		mScreenWidth(0),
		mScreenHeight(0),
		mAddressBarMaxHeight(0),
		mAddressBarMinHeight(0),
		mAddressBarWidgetsHeight(0),
		mIsAddressBarVisible(false)
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
		mNavBarWidget->setSize(mScreenWidth, NAVIGATION_BAR_WIDGET_HEIGHT);
		mNavBarWidget->setTitle(SCREEN_TITLE);
		mNavBarWidget->setBackButtonTitle("");
		mainLayout->addChild(mNavBarWidget);

		mScreen->setIcon(RES_TAB_ICON_WEB_VIEW);
	}

	// Create and add the address bar to the main layout.
	createAddressBar();
	mainLayout->addChild(mAddressBarLayout);
	mIsAddressBarVisible = true;

	// Create and add the line layout to the main layout.
	mLineLayout = new HorizontalLayout();
	mLineLayout->setSize(mScreenWidth, LINE_LAYOUT_HEIGHT);
	mLineLayout->setBackgroundColor(LINE_LAYOUT_BG_COLOR);
	mainLayout->addChild(mLineLayout);

	// Create and add the web view widget to the main layout.
	mWebView = new WebView();
	mWebView->openURL(sURL);
	mainLayout->addChild(mWebView);
}

/**
 * Destructor.
 */
ScreenWebView::~ScreenWebView()
{
	MAUtil::Environment::getEnvironment().removeTimer(this);
	mOpenLinkButtonWidget->setEventListener(NULL);
	mHideShowButtonWidget->setEventListener(NULL);
	mEditBoxWidget->setEventListener(NULL);

	delete mScreen;
	delete mWebView;
	delete mOpenLinkButtonWidget;
	delete mHideShowButtonWidget;
	delete mEditBoxWidget;
	delete mNavBarWidget;

	delete mAddressBarLayout;
	delete mLineLayout;
	delete mEditBoxLayout;
	delete mOpenLinkBtnLayout;
	delete mHideShowBtnLayout;
}

/**
 * Creates the address bar.
 */
void ScreenWebView::createAddressBar()
{
	float editBoxWidth = EDIT_BOX_WIDTH / 100.0 * mScreenWidth;
	float buttonWidth = BUTTON_WIDTH / 100.0 * mScreenWidth;
	int editBoxLayoutWidth = (int)editBoxWidth + SPACE;
	mAddressBarMaxHeight = DEFAULT_MAX_ADDRESS_BAR_HEIGHT;
	mAddressBarMinHeight = DEFAULT_MIN_ADDRESS_BAR_HEIGHT;

	// For large screen increase the height of the widgets.
	if(LARGE_SCREEN_HEIGHT <= mScreenHeight)
	{
		mAddressBarMaxHeight += LARGE_SCREEN_INCREASE_VALUE;
		mAddressBarMinHeight += LARGE_SCREEN_INCREASE_VALUE - (2 * SPACE);
	}

	mAddressBarWidgetsHeight = mAddressBarMaxHeight - (2 * SPACE);
	mCurrentAddressBarHeight = mAddressBarMaxHeight;
	mCurrentAddressBarAlpha = MAX_ALPHA_VALUE;

	// Create the address layout.
	mAddressBarLayout = new HorizontalLayout();
	mAddressBarLayout->setSize(mScreenWidth, mAddressBarMaxHeight);
	mAddressBarLayout->setProperty(
		MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Create the edit box widget.
	mEditBoxWidget = new EditBox();
	mEditBoxWidget->setTextInputMode();
	mEditBoxWidget->setProperty(MAW_EDIT_BOX_TEXT, sURL);
	mEditBoxWidget->setSize((int) editBoxWidth, mAddressBarWidgetsHeight);
	mEditBoxWidget->setEventListener(this);

	// Create the open link button widget.
	mOpenLinkButtonWidget = new ImageButton();
	mOpenLinkButtonWidget->setImage(RES_WEB_VIEW_OPEN_LINK_IMAGE);
	mOpenLinkButtonWidget->setSize(mAddressBarWidgetsHeight, mAddressBarWidgetsHeight);
	mOpenLinkButtonWidget->setEventListener(this);

	// Create the hide/show button widget.
	mHideShowButtonWidget = new ImageButton();
	mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_HIDE_IMAGE);
	mHideShowButtonWidget->setSize(mAddressBarMinHeight , mAddressBarMinHeight);
	mHideShowButtonWidget->setEventListener(this);

	// Create the edit box layout.
	mEditBoxLayout = createSpacer(editBoxLayoutWidth, mAddressBarWidgetsHeight);
	mEditBoxLayout->addChild(mEditBoxWidget);

	// Create the open link button layout.
	mOpenLinkBtnLayout = createSpacer(
		mAddressBarWidgetsHeight + (2 * SPACE),
		mAddressBarWidgetsHeight);
	mOpenLinkBtnLayout->addChild(mOpenLinkButtonWidget);

	// Create the hide/show button layout.
	int hideShowBtnLayoutWidth = mScreenWidth - editBoxLayoutWidth -
		mAddressBarWidgetsHeight - (5 * SPACE);
	mHideShowBtnLayout = createSpacer(hideShowBtnLayoutWidth, mAddressBarWidgetsHeight);
	mHideShowBtnLayout->setProperty(
		MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_TOP);
	mHideShowBtnLayout->addChild(mHideShowButtonWidget);

	// Add layouts to the address bar layout.
	mAddressBarLayout->addChild(mEditBoxLayout);
	mAddressBarLayout->addChild(mOpenLinkBtnLayout);
	mAddressBarLayout->addChild(mHideShowBtnLayout);
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
		if(widget == mEditBoxWidget || widget == mOpenLinkButtonWidget)
		{
			mEditBoxWidget->setProperty(MAW_EDIT_BOX_SHOW_KEYBOARD, FALSE_CONST);
			MAUtil::String webAddress = mEditBoxWidget->getPropertyString("text");
			mWebView->openURL(webAddress);
		}
		else if(widget == mHideShowButtonWidget)
		{
			// If the address bar is not animating handle this event.
			if(NONE == mAddressBarAnimation)
			{
				// Initialize animation values.
				mCurrentAddressBarAlpha = MAX_ALPHA_VALUE;
				if(mIsAddressBarVisible)
				{
					mCurrentAddressBarHeight = mAddressBarMaxHeight;
					mAddressBarAnimation = FADE_OUT_ADDRESS_BAR;
				} else
				{
					mCurrentAddressBarHeight = mAddressBarMinHeight;
					mAddressBarAnimation = HIDE_EXPAND_WIDGET;
				}

				// Start the timer. It should be stopped when the animation is finished.
				MAUtil::Environment::getEnvironment().addTimer(this, ANIMATION_TIME_PERIOD, 0);
			}
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
	mWebView->openURL(url.c_str());
	mEditBoxWidget->setProperty(MAW_EDIT_BOX_TEXT, sURL);
}

/**
 * Called on timer events.
 * It is used for address bar animation.
 */
 void ScreenWebView::runTimerEvent()
 {
	 // Check the state of the animation and call the method
	 // that handles this state.
	 switch(mAddressBarAnimation)
	 {
	 	 case FADE_OUT_ADDRESS_BAR:
	 		 fadeOutAddressBar();
	 		 break;
	 	 case SHOW_EXPAND_WIDGET:
	 		 showExpandWidget();
	 		 break;
	 	 case HIDE_EXPAND_WIDGET:
	 		 hideExpandWidget();
	 		 break;
	 	 case FADE_IN_ADDRESS_BAR:
	 		 fadeInAddressBar();
	 		 break;
	 	 case NONE:
	 	 default:
	 		 MAUtil::Environment::getEnvironment().removeTimer(this);
	}
}

/**
 * Fades out the address bar.
 * This method decreases the height and alpha value of the address bar with a
 * default value. The method should be called repeatedly until
 * mAddressBarAnimation will be set to SHOW_EXPAND_WIDGET (then the animation
 * can go to the next step - show the expand widget).
 */
void ScreenWebView::fadeOutAddressBar()
{
	 BOOL stopFading = true;

	 // Decrease the height of the address bar.
	 mCurrentAddressBarHeight -= ANIMATION_HEIGHT_VALUE;
	 if(mAddressBarMinHeight < mCurrentAddressBarHeight)
	 {
		 mAddressBarLayout->setSize(mScreenWidth, mCurrentAddressBarHeight);
		 stopFading = false;
	 }

	 // Set the alpha value for the address bar widgets.
	 mCurrentAddressBarAlpha -= ANIMATION_ALPHA_VALUE;
	 if(MIN_ALPHA_VALUE <= mCurrentAddressBarAlpha)
	 {
		 mEditBoxWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentAddressBarAlpha);
		 mOpenLinkButtonWidget->setProperty(
		 	MAW_WIDGET_ALPHA,
		 	mCurrentAddressBarAlpha);
		 mHideShowButtonWidget->setProperty(
			MAW_WIDGET_ALPHA,
			mCurrentAddressBarAlpha);
		 stopFading = false;
	 }

	 // If the height of the address bar has reached at the
	 // desired value(mAddressBarMinHeight) and the alpha
	 // value of the address bar widgets is MIN_ALPHA_VALUE
	 // then go to the next step of the animation.
	 if(stopFading) {
		 mAddressBarAnimation = SHOW_EXPAND_WIDGET;
		 mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_SHOW_IMAGE);
		 mHideShowBtnLayout->setProperty(
			MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		 setAddressBarWidgetsVisibility(false);
	 }
}

/**
 * Shows the expand image button.
 * This method increases the alpha value of the expand image button with a
 * default value. The method should be called repeatedly until
 * mAddressBarAnimation will be set to NONE (then the animation can go to
 * the next step - fade in the address bar).
 */
void ScreenWebView::showExpandWidget()
{
    mCurrentAddressBarAlpha += ANIMATION_ALPHA_VALUE;
	if(MAX_ALPHA_VALUE >= mCurrentAddressBarAlpha)
	{
		mHideShowButtonWidget->setProperty(
		 	MAW_WIDGET_ALPHA,
		 	mCurrentAddressBarAlpha);
	 } else
	 {
		// Stop the animation.
		mIsAddressBarVisible = false;
		mAddressBarAnimation = NONE;
		mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_SHOW_IMAGE);
	 }
}

/**
 * Hides the expand image button.
 * This method decreases the alpha value of the expand image button with a
 * default value. The method should be called repeatedly until
 * mAddressBarAnimation will be set to FADE_IN_ADDRESS_BAR (the animation is finished).
 */
void ScreenWebView::hideExpandWidget()
{
	 mCurrentAddressBarAlpha -= ANIMATION_ALPHA_VALUE;
	 if(MIN_ALPHA_VALUE <= mCurrentAddressBarAlpha)
	 {
		 mHideShowButtonWidget->setProperty(
			MAW_WIDGET_ALPHA,
			mCurrentAddressBarAlpha);
	 } else
	 {
		 // Go the next step of the animation.
		 mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_HIDE_IMAGE);
		 mAddressBarAnimation = FADE_IN_ADDRESS_BAR;
		 mHideShowBtnLayout->setProperty(
			MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_TOP);
	 }
}

/**
 * Fades in the navigation bar.
 * This method increases the height and alpha value of the address bar with a
 * default value. The method should be called repeatedly until
 * mAddressBarAnimation will be set to NONE (the animation is finished).
 */
void ScreenWebView::fadeInAddressBar()
{
	 BOOL stopFading = true;

	 // Increase the height of the navigation bar.
	 mCurrentAddressBarHeight += ANIMATION_HEIGHT_VALUE;
	 if(mAddressBarMaxHeight >= mCurrentAddressBarHeight)
	 {
		 mAddressBarLayout->setSize(mScreenWidth, mCurrentAddressBarHeight);
		 stopFading = false;
	 } else
	 {
		 mAddressBarLayout->setSize(mScreenWidth, mAddressBarMaxHeight);
	 }

	 // Set the alpha value for the address bar widgets.
	 mCurrentAddressBarAlpha += ANIMATION_ALPHA_VALUE;
	 if(MAX_ALPHA_VALUE >= mCurrentAddressBarAlpha)
	 {
		 mEditBoxWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentAddressBarAlpha);
		 mOpenLinkButtonWidget->setProperty(
		 	MAW_WIDGET_ALPHA,
		 	mCurrentAddressBarAlpha);
		 mHideShowButtonWidget->setProperty(
			MAW_WIDGET_ALPHA,
			mCurrentAddressBarAlpha);
		 stopFading = false;
	 }

	 // If the height of the address bar has reached at the
	 // desired value(mAddressBarMaxHeight) and the alpha
	 // value of the address bar widgets is MAX_ALPHA_VALUE
	 // then stop the animation.
	 if(stopFading) {
		 mAddressBarAnimation = NONE;
		 mIsAddressBarVisible = true;
		 setAddressBarWidgetsVisibility(true);
	 }
}

/**
 * Sets the visibility of the address bar widgets.
 * @param value True - shows the widgets, otherwise hides them.
 */
void ScreenWebView::setAddressBarWidgetsVisibility(const bool value)
{
	MAUtil::String propertyValue;

	if(true == value)
	{
		propertyValue = TRUE_CONST;
	} else
	{
		propertyValue = FALSE_CONST;
	}

	mEditBoxWidget->setProperty(MAW_WIDGET_VISIBLE, propertyValue);
	mOpenLinkButtonWidget->setProperty(
		MAW_WIDGET_VISIBLE,
		propertyValue);
}

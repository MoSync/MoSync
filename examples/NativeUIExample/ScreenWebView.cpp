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

// the width of the edit box in percentage (x% of the screen width).
#define EDIT_BOX_WIDTH 65.0

// the width of the open link button in percentage (x% of the screen width)
#define BUTTON_WIDTH 25.0

// the default height of the navigation bar(when it's expanded).
#define MAX_NAVIGATION_BAR_HEIGHT 55

// the height of the navigation bar(when it's collapsed).
#define MIN_NAVIGATION_BAR_HEIGHT 20

// the height of the widgets from navigation bar.
#define NAV_BAR_WIDGETS_HEIGHT 50

// increase/decrease height value of the navigation bar.
#define ANIMATION_HEIGHT_VALUE 8

// increase/decrease alpha value of the navigation bar.
#define ANIMATION_ALPHA_VALUE 0.25

// animation time
#define ANIMATION_TIME_PERIOD 50

// the maximum alpha value of the navigation bar.
#define MAX_ALPHA_VALUE 1

// the minimum alpha value of the navigation bar.
#define MIN_ALPHA_VALUE 0

// space between widgets
#define SPACE 5

// the default URL address
#define DEFAULT_URL_ADDRESS "http://www.google.com"

// the name of the screen.
#define SCREEN_TITLE "Web"

// the height of the line layout.
#define LINE_LAYOUT_HEIGHT 2

// the background color of the line layout.
#define LINE_LAYOUT_BG_COLOR 0xC0C0C0


#include "ScreenWebView.h"

// Include the resources for images.
#include "MAHeaders.h"

// set the pointer to ScreenWebView object to null.
ScreenWebView* ScreenWebView::mScreenWebView = NULL;

// set the default URL address.
MAUtil::String ScreenWebView::mURL = DEFAULT_URL_ADDRESS;

/**
 * Create the web view screen.
 */
Screen* ScreenWebView::create()
{
	if(NULL == mScreenWebView) {
		mScreenWebView = new ScreenWebView();
	}

	return mScreenWebView->getScreen();
}

/**
 * Destroy the web view screen.
 */
void ScreenWebView::destroy()
{
	delete mScreenWebView;
	mScreenWebView = NULL;
}

/**
 * Sets the default URL address for the web widget.
 * @param url The desired URL web page.
 */
void ScreenWebView::setDefaultURL(const MAUtil::String url)
{
	mScreenWebView->openURL(url);
}
/**
 * Constructor.
 * @param Screen The widgets will be added on this screen.
 * Initialize the objects.
 */
ScreenWebView::ScreenWebView():
		mScreen(NULL),
		mWebView(NULL),
		mOpenLinkButtonWidget(NULL),
		mHideShowButtonWidget(NULL),
		mEditBoxWidget(NULL),
		mNavigationBarLayout(NULL),
		mLineLayout(NULL),
		mEditBoxLayout(NULL),
		mOpenLinkBtnLayout(NULL),
		mHideShowBtnLayout(NULL),
		mNavigationBarAnimation(NONE),
		mCurrentNavigationBarHeight(0),
		mCurrentNavigationBarAlpha(0),
		mScreenWidth(0),
		mScreenHeight(0),
		mIsNavigationBarVisible(false)
{
	mScreen = new Screen();
	mScreen->setTitle(SCREEN_TITLE);

	// Get the screen size.
	mScreenHeight = mScreen->getPropertyInt(MAW_WIDGET_HEIGHT);
	mScreenWidth = mScreen->getPropertyInt(MAW_WIDGET_WIDTH);

	VerticalLayout* mainLayout = new VerticalLayout();

	// create the navigation bar.
	createNavigationBar();
	mainLayout->addChild(mNavigationBarLayout);
	mIsNavigationBarVisible = true;

	// create the line layout.
	mLineLayout = new HorizontalLayout();
	mLineLayout->setSize(mScreenWidth, LINE_LAYOUT_HEIGHT);
	mLineLayout->setBackgroundColor(LINE_LAYOUT_BG_COLOR);
	mainLayout->addChild(mLineLayout);

	// create the web view widget.
	mWebView = new WebView();
	mWebView->openURL(mURL.c_str());
	mainLayout->addChild(mWebView);

	mScreen->setMainWidget(mainLayout);
	if (WidgetManager::isAndroid())
	{
		mScreen->setIcon(RES_TAB_ICON_WEB_VIEW_ANDROID);
	}
	else
	{
		mScreen->setIcon(RES_TAB_ICON_WEB_VIEW);
	}
}

/**
 * Destructor.
 */
ScreenWebView::~ScreenWebView()
{
	MAUtil::Environment::getEnvironment().removeTimer(this);
	mOpenLinkButtonWidget->setEventListener(NULL);
	mHideShowButtonWidget->setEventListener(NULL);

	delete mScreen;
	delete mWebView;
	delete mOpenLinkButtonWidget;
	delete mHideShowButtonWidget;
	delete mEditBoxWidget;

	delete mNavigationBarLayout;
	delete mLineLayout;
	delete mEditBoxLayout;
	delete mOpenLinkBtnLayout;
	delete mHideShowBtnLayout;
}

/**
 * Creates the navigation bar.
 */
void ScreenWebView::createNavigationBar()
{
	float editBoxWidth = EDIT_BOX_WIDTH / 100.0 * mScreenWidth;
	float buttonWidth = BUTTON_WIDTH / 100.0 * mScreenWidth;
	int editBoxLayoutWidth = (int)editBoxWidth + SPACE;
	mCurrentNavigationBarHeight = NAV_BAR_WIDGETS_HEIGHT;
	mCurrentNavigationBarAlpha = MAX_ALPHA_VALUE;

	// create the navigation layout.
	mNavigationBarLayout = new HorizontalLayout();
	mNavigationBarLayout->setSize(mScreenWidth, MAX_NAVIGATION_BAR_HEIGHT);
	mNavigationBarLayout->setProperty(MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);

	// create the edit box widget.
	mEditBoxWidget = new EditBox();
	mEditBoxWidget->setTextInputMode();
	mEditBoxWidget->setProperty(MAW_EDIT_BOX_TEXT, mURL);
	mEditBoxWidget->setSize((int) editBoxWidth, NAV_BAR_WIDGETS_HEIGHT);

	// create the open link button widget.
	mOpenLinkButtonWidget = new ImageButton();
	mOpenLinkButtonWidget->setImage(RES_WEB_VIEW_OPEN_LINK_IMAGE);
	mOpenLinkButtonWidget->setSize(NAV_BAR_WIDGETS_HEIGHT, NAV_BAR_WIDGETS_HEIGHT);
	mOpenLinkButtonWidget->setEventListener(this);

	// create the hide/show button widget.
	mHideShowButtonWidget = new ImageButton();
	mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_HIDE_IMAGE);
	mHideShowButtonWidget->setSize(MIN_NAVIGATION_BAR_HEIGHT, MIN_NAVIGATION_BAR_HEIGHT);
	mHideShowButtonWidget->setEventListener(this);

	// create the edit box layout.
	mEditBoxLayout = createSpacer(editBoxLayoutWidth, NAV_BAR_WIDGETS_HEIGHT);
	mEditBoxLayout->addChild(mEditBoxWidget);

	// create the open link button layout
	mOpenLinkBtnLayout = createSpacer(NAV_BAR_WIDGETS_HEIGHT + (2 * SPACE), NAV_BAR_WIDGETS_HEIGHT);
	mOpenLinkBtnLayout->addChild(mOpenLinkButtonWidget);

	// create the hide/show btn layout.
	int hideShowBtnLayoutWidth = mScreenWidth - editBoxLayoutWidth - NAV_BAR_WIDGETS_HEIGHT - (3 * SPACE);
	mHideShowBtnLayout = createSpacer(hideShowBtnLayoutWidth, NAV_BAR_WIDGETS_HEIGHT);
	mHideShowBtnLayout->setProperty(MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_TOP);
	mHideShowBtnLayout->addChild(mHideShowButtonWidget);

	// add layouts to the navigation bar layout.
	mNavigationBarLayout->addChild(mEditBoxLayout);
	mNavigationBarLayout->addChild(mOpenLinkBtnLayout);
	mNavigationBarLayout->addChild(mHideShowBtnLayout);
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
	layout->setProperty(MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_RIGHT);
	layout->setProperty(MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	return layout;
}

/**
 * This method is called when there is an event for this widget.
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
		if(widget == mOpenLinkButtonWidget)
		{
			MAUtil::String webAddress = mEditBoxWidget->getPropertyString("text");
			mWebView->openURL(webAddress.c_str());
		} else if(widget == mHideShowButtonWidget)
		{
			// if the navigation bar is not animating handle this event.
			if(NONE == mNavigationBarAnimation)
			{
				// initialize animation values.
				mCurrentNavigationBarAlpha = MAX_ALPHA_VALUE;
				if(mIsNavigationBarVisible)
				{
					mCurrentNavigationBarHeight = MAX_NAVIGATION_BAR_HEIGHT;
					mNavigationBarAnimation = FADE_OUT_NAVIGATION_BAR;
				} else
				{
					mCurrentNavigationBarHeight = MIN_NAVIGATION_BAR_HEIGHT;
					mNavigationBarAnimation = HIDE_EXPAND_WIDGET;
				}

				// start the timer.
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
	mEditBoxWidget->setProperty(MAW_EDIT_BOX_TEXT, mURL);
}

/**
 * Called on timer events.
 * It is used for navigation bar animation.
 */
 void ScreenWebView::runTimerEvent()
 {
	 // check the state of the animation and call the method
	 // that handles this state.
	 switch(mNavigationBarAnimation)
	 {
	 	 case FADE_OUT_NAVIGATION_BAR:
	 		 fadeOutNavigationBar();
	 		 break;
	 	 case SHOW_EXPAND_WIDGET:
	 		 showExpandWidget();
	 		 break;
	 	 case HIDE_EXPAND_WIDGET:
	 		 hideExpandWidget();
	 		 break;
	 	 case FADE_IN_NAVIGATION_BAR:
	 		 fadeInNavigationBar();
	 		 break;
	 	 case NONE:
	 	 default:
	 		 MAUtil::Environment::getEnvironment().removeTimer(this);
	}
}

/**
 * Fades out the navigation bar.
 */
void ScreenWebView::fadeOutNavigationBar()
{
	 BOOL stopFading = true;

	 // decrease the height of the navigation bar.
	 mCurrentNavigationBarHeight -= ANIMATION_HEIGHT_VALUE;
	 if(MIN_NAVIGATION_BAR_HEIGHT < mCurrentNavigationBarHeight)
	 {
		 mNavigationBarLayout->setSize(mScreenWidth, mCurrentNavigationBarHeight);
		 stopFading = false;
	 }

	 // set the alpha value for the navigation bar widgets.
	 mCurrentNavigationBarAlpha -= ANIMATION_ALPHA_VALUE;
	 if(MIN_ALPHA_VALUE <= mCurrentNavigationBarAlpha)
	 {
		 mEditBoxWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
		 mOpenLinkButtonWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
		 mHideShowButtonWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
		 stopFading = false;
	 }

	 // if the height of the navigation bar has reached at the
	 // desired value(MIN_NAVIGATION_BAR_HEIGHT) and the alpha
	 // value of the navigation bar widgets is MIN_ALPHA_VALUE
	 // then go to the next step of the animation.
	 if(stopFading) {
		 mNavigationBarAnimation = SHOW_EXPAND_WIDGET;
		 mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_SHOW_IMAGE);
		 mHideShowBtnLayout->setProperty(MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	 }
}

/**
 * Shows the expand image button.
 */
void ScreenWebView::showExpandWidget()
{
	// set the alpha value for the show/hide button widget.
	 mCurrentNavigationBarAlpha += ANIMATION_ALPHA_VALUE;
	 if(MAX_ALPHA_VALUE >= mCurrentNavigationBarAlpha)
	 {
		 mHideShowButtonWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
	 } else
	 {
		 // stop the animation.
		 mIsNavigationBarVisible = false;
		 mNavigationBarAnimation = NONE;
	 }
}

/**
 * Hides the expand image button.
 */
void ScreenWebView::hideExpandWidget()
{
	 mCurrentNavigationBarAlpha -= ANIMATION_ALPHA_VALUE;
	 if(MIN_ALPHA_VALUE <= mCurrentNavigationBarAlpha)
	 {
		 mHideShowButtonWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
	 } else
	 {
		 // go the next step of the animation.
		 mHideShowButtonWidget->setImage(RES_WEB_VIEW_IMAGE_HIDE_IMAGE);
		 mNavigationBarAnimation = FADE_IN_NAVIGATION_BAR;
		 mHideShowBtnLayout->setProperty(MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_TOP);
	 }
}

/**
 * Fades in the navigation bar.
 */
void ScreenWebView::fadeInNavigationBar()
{
	 BOOL stopFading = true;

	 // increase the height of the navigation bar.
	 mCurrentNavigationBarHeight += ANIMATION_HEIGHT_VALUE;
	 if(MAX_NAVIGATION_BAR_HEIGHT >= mCurrentNavigationBarHeight)
	 {
		 mNavigationBarLayout->setSize(mScreenWidth, mCurrentNavigationBarHeight);
		 stopFading = false;
	 } else
	 {
		 mNavigationBarLayout->setSize(mScreenWidth, MAX_NAVIGATION_BAR_HEIGHT);
	 }

	 // set the alpha value for the navigation bar widgets.
	 mCurrentNavigationBarAlpha += ANIMATION_ALPHA_VALUE;
	 if(MAX_ALPHA_VALUE >= mCurrentNavigationBarAlpha)
	 {
		 mEditBoxWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
		 mOpenLinkButtonWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
		 mHideShowButtonWidget->setProperty(MAW_WIDGET_ALPHA, mCurrentNavigationBarAlpha);
		 stopFading = false;
	 }

	 // if the height of the navigation bar has reached at the
	 // desired value(MAX_NAVIGATION_BAR_HEIGHT) and the alpha
	 // value of the navigation bar widgets is MAX_ALPHA_VALUE
	 // then stop the animation.
	 if(stopFading) {
		 mNavigationBarAnimation = NONE;
		 mIsNavigationBarVisible = true;
	 }
}

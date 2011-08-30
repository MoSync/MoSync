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
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mWebView(NULL),
	mHookReceived(NULL),
	mLoadingState(NULL),
	mEnableVerticalScroll(NULL),
	mEnableHorizontalScroll(NULL),
	mOpenUrl(NULL),
	mSetHardHook(NULL),
	mSetSoftHook(NULL)
{
	createMainLayout();

	mWebView->addWebViewListener(this);
	mEnableHorizontalScroll->addButtonListener(this);
	mEnableVerticalScroll->addButtonListener(this);
	mOpenUrl->addButtonListener(this);
	mSetSoftHook->addButtonListener(this);
	mSetHardHook->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mWebView->removeWebViewListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mLoadingState = new Label();
	mLoadingState->setText("Loading state:");
	mMainLayout->addChild(mLoadingState);

	mHookReceived = new Label();
	mHookReceived->setText("Hook listener");
	mMainLayout->addChild(mHookReceived);

	mWebView = new WebView();
	mWebView->fillSpaceHorizontally();
	mWebView->fillSpaceVertically();

	HorizontalLayout* scrollLayout= new HorizontalLayout();
	mEnableVerticalScroll = new Button();
	if ( mWebView->isVerticalScrollBarEnabled() )
	{
		mEnableVerticalScroll->setText("Disable VerticalScroll");
	}
	else
	{
		mEnableVerticalScroll->setText("Enable VerticalScroll");
	}

	mEnableHorizontalScroll = new Button();
	if ( mWebView->isHorizontalScrollbarEnabled() )
	{
		mEnableHorizontalScroll->setText("Disable HorizontalScroll");
	}
	else
	{
		mEnableHorizontalScroll->setText("Enable HorizontalScroll");
	}

	scrollLayout->addChild(mEnableVerticalScroll);
	scrollLayout->addChild(mEnableHorizontalScroll);
	scrollLayout->wrapContentVertically();
	scrollLayout->fillSpaceHorizontally();
	mMainLayout->addChild(scrollLayout);

//	mWebView->setHtml("<h1>Hello World</h1>");
	mWebView->openURL("http://mosync.com");
	mMainLayout->addChild(mWebView);

	mOpenUrl = new Button();
	mOpenUrl->setText("Open http://www.google.ro");
	mMainLayout->addChild(mOpenUrl);

	mSetSoftHook = new Button();
	mSetSoftHook->setText("Set soft hook for all urls");
	mMainLayout->addChild(mSetSoftHook);

	mSetHardHook = new Button();
	mSetHardHook->setText("Set hard hook for all urls");
	mMainLayout->addChild(mSetHardHook);
}

/**
 * This method is called when MAW_EVENT_WEB_VIEW_HOOK_INVOKED is received.
 * @param hookType The type of hook that has been invoked.
 * One of:
 *  -#MAW_CONSTANT_SOFT
 *  -#MAW_CONSTANT_HARD
 *
 *  @param urlData The handle to url data. This is string data,
 * there is NO null terminating character. The encoding
 * of the data is determined by the application.
 * The data MUST be deallocated with maDestroyObject after
 * it hs been used. Each event allocates a new data object
 * for the url data.
 */
void MainScreen::webViewHookInvoked(
	WebView* webView,
	int hookType,
	MAHandle urlData)
{
	MAUtil::String type =(hookType == MAW_CONSTANT_SOFT ? "Soft" : "Hard");
	mHookReceived->setText("Hook invoked event received with Hook type: "+ type );

	maDestroyObject(urlData);
}

/**
 * This method is called when the web view content is loading.
 * @param webView The web view object that generated the event.
 * Any of the following constants:
 * - #MAW_CONSTANT_STARTED
 * - #MAW_CONSTANT_DONE
 * - #MAW_CONSTANT_STOPPED
 * - #MAW_CONSTANT_ERROR
 */
void MainScreen::webViewContentLoading(
	WebView* webView,
	const int webViewState)
{
	switch(webViewState)
	{
	case MAW_CONSTANT_STARTED:
		mLoadingState->setText(" Loading started....");
		break;
	case MAW_CONSTANT_STOPPED:
		mLoadingState->setText(" Loading stopped.");
		break;
	case MAW_CONSTANT_DONE:
		mLoadingState->setText("Loading done.");
		break;
	case MAW_CONSTANT_ERROR:
		mLoadingState->setText("Loading error!");
		break;
	}
}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * Only for iphone platform.
 * @param button The button object that generated the event.
 */
 void MainScreen::buttonPressed(Widget* button)
{

}

/**
 * This method is called when there is an touch-up event for
 * a button.
 * Only for iphone platform.
 * @param button The button object that generated the event.
 */
 void MainScreen::buttonReleased(Widget* button)
{
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
 void MainScreen::buttonClicked(Widget* button)
{
	 if( mSetSoftHook == button)
	 {
		 mWebView->setSoftHook(".*");
	 }
	 else if (mSetHardHook == button)
	 {
		 mWebView->setHardHook(".*");
	 }
	 else if (mOpenUrl == button)
	 {
		 mWebView->openURL("http://www.google.ro");
	 }
	 else if (mEnableVerticalScroll == button)
	 {
		if ( mWebView->isVerticalScrollBarEnabled() )
		{
			mWebView->disableVerticalScrollbar();
			mEnableVerticalScroll->setText("Enable VerticalScroll");
		}
		else
		{
			mWebView->enableVerticalScrollBar();
			mEnableVerticalScroll->setText("Disable VerticalScroll");
		}
	 }
	 else if(mEnableHorizontalScroll == button)
	 {
			if ( mWebView->isHorizontalScrollbarEnabled() )
			{
				mWebView->disableHorizontalScrollbar();
				mEnableHorizontalScroll->setText("Enable HorizontalScroll");
			}
			else
			{
				mWebView->enableHorizontalScrollBar();
				mEnableHorizontalScroll->setText("Disable HorizontalScroll");
			}
	 }
}

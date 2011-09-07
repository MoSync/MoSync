/*
 * FacebookLoginScreen.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: gabi
 */


#include "FacebookLoginScreen.h"
#include <IX_WIDGET.h>

namespace FacebookDemoGUI{

FacebookLoginScreen::FacebookLoginScreen()
{
	using namespace NativeUI;

	mWebView = new FacebookDemoGUI::FacebookWebView();

	mWebView->fillSpaceHorizontally();
	mWebView->fillSpaceVertically();

	mScreen.setMainWidget(mWebView);

	mWebView->setSoftHook(".*");
}

void FacebookLoginScreen::setSize(int width, int height)
{
	mScreen.setSize(width, height);
}

void FacebookLoginScreen::setUrl(const MAUtil::String &url)
{
	//mWebView->setProperty("url", url.c_str());
	//MAW_WEB_VIEW_URL
	//mWebView->setProperty(MAW_WEB_VIEW_URL, url);
	mWebView->openURL(url);
}

MAUtil::String FacebookLoginScreen::getRedirectUrl() const
{
	//mWebView->getPropertyString("newurl");
	return mWebView->getNewURL();
}

void FacebookLoginScreen::setListener(NativeUI::WebViewListener *listener)
{
	mWebView->addWebViewListener(listener);
}

void FacebookLoginScreen::show()
{

	mScreen.show();
}

FacebookLoginScreen::~FacebookLoginScreen()
{

}

}//namespace FacebookDemoGUI

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

/*
 * FacebookLoginScreen.cpp
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

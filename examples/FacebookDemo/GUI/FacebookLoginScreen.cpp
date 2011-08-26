/* Copyright (C) 2011 MoSync AB

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
 *
 *  Created on: Jun 23, 2011
 *      Author: gabi
 */


#include "FacebookLoginScreen.h"
#include <IX_WIDGET.h>

namespace FacebookDemoGUI{

FacebookLoginScreen::FacebookLoginScreen()
{
	using namespace MoSync::UI;

	mWebView = new WebView();

	mWebView->fillSpaceHorizontally();
	mWebView->fillSpaceVertically();
	mWebView->setProperty("title", "Web");

	mScreen.setMainWidget(mWebView);
}

void FacebookLoginScreen::setSize(int width, int height)
{
	mScreen.setSize(width, height);
}

void FacebookLoginScreen::setUrl(const MAUtil::String &url)
{
	mWebView->setProperty("url", url.c_str());
	mWebView->openURL(url);
}

MAUtil::String FacebookLoginScreen::getRedirectUrl() const
{
	return mWebView->getPropertyString("newurl");
}

void FacebookLoginScreen::show()
{

	mScreen.show();
}

FacebookLoginScreen::~FacebookLoginScreen()
{

}

}//namespace FacebookDemoGUI

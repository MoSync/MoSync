/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
	mWebView->openURL(url);
}

MAUtil::String FacebookLoginScreen::getRedirectUrl() const
{
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

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
 * @author TODO: Add your name.
 *
 * Class that creates a screen that displays a web view.
 */

#include "MAHeaders.h"
#include "ScreenWebView.h"

/**
 * Create the web view screen.
 */
Screen* ScreenWebView::create()
{
	Screen* screen = new Screen();
	screen->setTitle("Web");
	screen->setIcon(RES_TAB_ICON_WEB_VIEW);
	WebView* webView = new WebView();
	webView->openURL("http://www.google.com");
	screen->setMainWidget(webView);
	return screen;
}

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
 * FacebookLoginScreen.h
 */

#ifndef FACEBOOKLOGINSCREEN_H_
#define FACEBOOKLOGINSCREEN_H_


#include <MAUtil/String.h>
#include "FacebookWebView.h"
#include <NativeUI/Screen.h>

namespace FacebookDemoGUI{

class FacebookLoginScreen
{
public:
	FacebookLoginScreen();
	void setSize(int width, int height);

	void setUrl(const MAUtil::String &url);
	MAUtil::String getRedirectUrl() const;

	void setListener(NativeUI::WebViewListener *listener);

	void show();
	~FacebookLoginScreen();

private:
	NativeUI::Screen					mScreen;
	FacebookDemoGUI::FacebookWebView 	*mWebView;
};

}//namespace FacebookDemoGUI

#endif /* FACEBOOKLOGINSCREEN_H_ */

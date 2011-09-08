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
 * FacebookWebView.h
 */

#ifndef FACEBOOKWEBVIEW_H_
#define FACEBOOKWEBVIEW_H_

#include <MAUtil/String.h>
#include <NativeUI/WebView.h>

namespace FacebookDemoGUI
{

class FacebookWebView: public NativeUI::WebView
{
public:
	FacebookWebView();
	void setStringPropertySize(int bufferSize);

	 /**
	 * Widget override.
	 * Get a widget property as a string, setting also the result code.
	 */
	virtual MAUtil::String getPropertyString( const MAUtil::String& property) const;

private:
		int BUFFER_SIZE;
};


}//namespace FacebookDemoGUI


#endif /* FACEBOOKWEBVIEW_H_ */

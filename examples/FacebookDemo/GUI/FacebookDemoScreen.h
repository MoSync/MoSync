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
 * FacebookDemoScreen.h
 */

#ifndef FACEBOOKDEMOSCREEN_H_
#define FACEBOOKDEMOSCREEN_H_

#include <MAUtil/Environment.h>
#include "NativeUI/Screen.h"

namespace FacebookDemoGUI
{

class FacebookDemoScreen: public NativeUI::Screen, public MAUtil::KeyListener
{
public:
	FacebookDemoScreen(FacebookDemoScreen *prevScreen);
	virtual void show();
	void receiveKeyEvents(bool receive);

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	FacebookDemoScreen *getPreviousScreen();

	virtual ~FacebookDemoScreen();

protected:
	virtual void back();

protected:
	FacebookDemoScreen *mPreviousScreen;
};


}//namespace FacebookDemoGUI


#endif /* FACEBOOKDEMOSCREEN_H_ */

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
 * TextScreen.h
 *
 *  Created on: Aug 6, 2011
 *      Author: gabi
 */

#ifndef TEXTSCREEN_H_
#define TEXTSCREEN_H_

#include <MAUtil/String.h>

#include "../GUI/NativeUI/Screen.h"
#include "../GUI/NativeUI/Label.h"

#include "FacebookDemoScreen.h"

namespace FacebookDemoGUI
{

class TextScreen: public FacebookDemoScreen
{
public:
	TextScreen(FacebookDemoScreen *prev);
	TextScreen();

	void setText(const MAUtil::String &text);
	void clear();

private:
	virtual void addChild(MoSync::UI::Widget *widget);

private:
	void initialize(MoSync::UI::Label *label);

private:
	MoSync::UI::Label 		*mLabel;
};

}//namespace FacebookDemoGUI

#endif /* TEXTSCREEN_H_ */

/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/**
* \file WidgetSkin.h
* \brief Class for defining the visual appearance of a widget
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_NATIVE_EDIT_BOX_H_
#define _SE_MSAB_MAUI_NATIVE_EDIT_BOX_H_

#include "Label.h"
#include <MAUtil/Environment.h>

namespace MAUI {

class NativeEditBox : public Label, public TextBoxListener {
public:
	NativeEditBox(int x, int y, int w, int h, Widget* parent=NULL, int maxSize=128, int options=MA_TB_TYPE_ANY, const String& initialText="", const WString& titleString=L"");
	~NativeEditBox();

	void setOptions(int options);
	void setMaxSize(int size);

	void setTitleString(const WString& mTitleString);
	const WString& getTitleString() const;

	bool pointerPressed(MAPoint2d p, int id);
	bool pointerMoved(MAPoint2d p, int id);
	bool pointerReleased(MAPoint2d p, int id);

	// if caption is larger than mMaxSize mMaxSize will be changed to the length of the caption.
	virtual void setCaption(const String& caption);

protected:
	void textBoxClosed(int res, int length);

	WString mTitleString;
	wchar_t* mString;
	int mMaxSize;
	int mOptions;

	int mStartX, mStartY;

};

} // namespace MAUI

#endif // _SE_MSAB_MAUI_NATIVE_EDIT_BOX_H_

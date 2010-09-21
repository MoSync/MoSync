#include "NativeEditBox.h"
#include <mastdlib.h>
#include <mavsprintf.h>
#include <mawvsprintf.h>

namespace MAUI {

void NativeEditBox::textBoxClosed(int res, int length) {
	if(res == MA_TB_RES_OK) {
		MAUI_LOG("length: %d", length);
		String str;
		str.resize(length);
		sprintf(str.pointer(), "%S", mString);
		setCaption(str);
		MAUI_LOG("%S", mString);
		requestRepaint();
	}
	Environment::getEnvironment().removeTextBoxListener(this);
}

NativeEditBox::NativeEditBox(int x, int y, int w, int h, Widget* parent, int maxSize, int options, const String& initialText, const WString& titleString) :
	Label(x, y, w, h, parent),
	mString(NULL),
	mTitleString(titleString),
	mOptions(options) {
	setMaxSize(maxSize);
	setCaption(initialText);
}

NativeEditBox::~NativeEditBox() {
	if(mString)
		delete mString;
}

void NativeEditBox::setOptions(int options) {
	mOptions = options;
}

void NativeEditBox::setMaxSize(int size) {
	if(mString) delete mString;
	mString = new wchar_t[mMaxSize];
	mMaxSize = size;
}

bool NativeEditBox::pointerPressed(MAPoint2d p, int id) {
	mStartX = p.x;
	mStartY = p.y;
	return true;
}

bool NativeEditBox::pointerMoved(MAPoint2d p, int id) {
	p.x-=mStartX;
	p.y-=mStartY;
	if((abs(p.x)<7) && (abs(p.y)<7)) return true;
	else return false;
}

bool NativeEditBox::pointerReleased(MAPoint2d p, int id) {
	maTextBox(mTitleString.c_str(), mString, mString, mMaxSize, mOptions);
	Environment::getEnvironment().addTextBoxListener(this);
	return false;
}

void NativeEditBox::setTitleString(const WString& titleString) {
	mTitleString = titleString;
}
const WString& NativeEditBox::getTitleString() const {
	return mTitleString;
}


void NativeEditBox::setCaption(const String& caption) {
	if(caption.length() > mMaxSize) {
		setMaxSize(caption.length()+1);
	}
	wsprintf(mString, L"%s", caption.c_str());
	Label::setCaption(caption);
}

} // namespace MAUI

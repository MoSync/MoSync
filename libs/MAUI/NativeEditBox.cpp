#include "NativeEditBox.h"
#ifdef USE_NEWLIB
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>
#include <stdio.h>
#else
#include <mastdlib.h>
#include <mavsprintf.h>
#include <mawvsprintf.h>
#include <maassert.h>
#endif

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
		/*
		if(mListener != NULL) {
			mListener->nativeEditFinished();
		}
		*/		
		ListenerSet_fire(NativeEditBoxListener, mEditBoxListeners, nativeEditFinished(this, mCaption));
		
	}
	//mListener = NULL;
	Environment::getEnvironment().removeTextBoxListener(this);
}

NativeEditBox::NativeEditBox(int x, int y, int width, int height, int maxSize, int options,
	const String& initialText, const WString& titleString) :
Label(x, y, width, height, initialText),
mTitleString(titleString),
mString(NULL),
mOptions(options),
mEditBoxListeners(false)
{
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
	if(size == 0) maPanic(1, "NativeEditBox: Invalid max size!!");
	wchar_t *oldString = mString;
	mString = new wchar_t[size];

	if(oldString)  
	{
		int minSize = (mMaxSize<size)?mMaxSize:size;
		memcpy(mString, oldString, minSize);
		mString[size-1] = 0;
		delete oldString;
	}
	
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
	activate();
	return false;
}

void NativeEditBox::activate() {
	
#ifdef USE_NEWLIB
	swprintf(mString, mMaxSize, L"%s", mCaption.c_str());
#else
	wsprintf(mString, L"%s", mCaption.c_str());
#endif
	int res = maTextBox((const wchar*)mTitleString.c_str(), (wchar*)mString,
		(wchar*)mString, mMaxSize, mOptions);
	if(res < 0) {
		PANIC_MESSAGE("maTextBox failed");
	}
	Environment::getEnvironment().addTextBoxListener(this);
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
	Label::setCaption(caption);
}


void NativeEditBox::addNativeEditBoxListener(NativeEditBoxListener* wl) {
	mEditBoxListeners.add(wl);
}

void NativeEditBox::removeNativeEditBoxListener(NativeEditBoxListener* wl) {
	mEditBoxListeners.remove(wl);
}

} // namespace MAUI

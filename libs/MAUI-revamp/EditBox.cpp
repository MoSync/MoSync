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
* \file EditBox.cpp
* \brief Edit box widget, multi and single line.
* \author Patrick Broman and Niklas Nummelin
*/

#include <ma.h>
#include "EditBox.h"
#include "Font.h"
#include "Engine.h"
//#include <conprint.h>
#include <mastdlib.h>
#include <mavsprintf.h>

#include <MAUtil/CharInput.h>
#include <MAUtil/Graphics.h>
#include <conprint.h>

namespace MAUI {
	void EditBoxListener::textChanged(EditBox *eb, const String& text) {
	}

	void EditBoxListener::cursorMoved(
		EditBox *eb, 
		int newCursor, 
		int lastCursor) {
	}

	void EditBoxListener::characterDeleted(EditBox *eb, char c) {
	}

	void EditBoxListener::characterChanged(EditBox *eb, char c) {
	}

	void EditBoxListener::characterAdded(EditBox *eb, char c) {
	}

	EditBox::EditBox(
		int x, 
		int y, 
		int width, 
		int height, 
		const String &text,
		bool manageNavigation, 
		bool mMultiLine, 
		int maxLength, 
		InputMode inputMode) :
			Label(x, y, width, height, text),
			mCursorIndex(0),
			mManageNavigation(manageNavigation),
			mActive(false),
			mCursorColor(0xffffff),
			mCharacterInputActive(false),
			mCurrentLine(0),
			mMaxLength(maxLength),
			mInputMode(inputMode),
			mEditBoxListeners(),
			mCaseToggle(true),
			mPasswordMode(false)
	{
		setMultiLine(mMultiLine);
		setCaption(text);
		this->mCaption.reserve(mMaxLength);
		setInputMode(mInputMode);
	}

	void EditBox::setSelected(bool selected) {
		if(!mActive&&selected==true) {
			activate();
		}
		else if(mActive&&selected==false) {
			deactivate();
		}
		Widget::setFocused(selected);
		
	}


	void EditBox::activate() {
		if(mActive) return;
		if(mManageNavigation) {
			Environment::getEnvironment().addKeyListener(this);
		}
		CharInput::getCharInput().addCharInputListener(this);
		mActive = true;
		setInputMode(mInputMode);
		setSelected(true);
		
	}

	void EditBox::deactivate() {
		if(!mActive) return;
		if(mManageNavigation) {
			if(Environment::getEnvironment().isKeyListener(this))	
				Environment::getEnvironment().removeKeyListener(this);
		}

		mCharacterInputActive = false;
		CharInput::getCharInput().forceDeployment();

		if(CharInput::getCharInput().isCharInputListener(this))
			CharInput::getCharInput().removeCharInputListener(this);
		mActive = false;
		setSelected(false);
	}

	bool EditBox::isActive() const {
		return mActive;
	}

	void EditBox::clearText() {
		setCaption("");
		mPassword.clear();
	}

	void EditBox::setCaption(const String& caption) {
		setText(caption);
	}
	
	void EditBox::setText(const String& text) {
		// TODO: Document why this is commented out or remove.
		//this->text = text;
		//if(mCursorIndex>text.size()) mCursorIndex = text.size();
		//this->mCaption = mCaption;
		Label::setCaption(text);
		if (mCursorIndex>mCaption.length()) {
			mCursorIndex = mCaption.length();
		}
		requestRepaint();
	}

	const String& EditBox::getText() const {
		return mPasswordMode ? mPassword : mCaption;
	}

	// TODO: Document why this is commented out or remove.
	/*
	void EditBox::setFont(Font *mFont) {
		this->mFont = mFont;
	}

	Font* EditBox::getFont() const {
		return mFont;
	}
	*/

	void EditBox::setCursorColor(int color) {
		mCursorColor = color;
	}

	int EditBox::getCursorColor() const {
		return mCursorColor;
	}

	void EditBox::setInputMode(EditBox::InputMode inputMode) {
		if(inputMode == IM_STANDARD) {
			CharInput::getCharInput().setMode(CharInput::LOWERCASE);
		} 
		else if(inputMode == IM_NUMBERS) {
			CharInput::getCharInput().setMode(CharInput::NUMBERS);
		}
		else if(inputMode == IM_QWERTY) {
			CharInput::getCharInput().setQwerty(true);
		}
		this->mInputMode = inputMode;
	}

	EditBox::InputMode EditBox::getInputMode() const {
		return mInputMode;
	}

	void EditBox::setManageNavigation(bool b) {
		mManageNavigation = b;
		if(mManageNavigation) {
			Environment::getEnvironment().addKeyListener(this);
		}
	}

	// TODO: Document why this is commented out or remove.
	/*
	void EditBox::setMultiLine(bool b) {
		mMultiLine = b;
	}

	bool EditBox::isMultiLine() {
		return mMultiLine;
	}
	*/

	bool EditBox::isManagingNavigation() {
		return mManageNavigation;
	}

	void EditBox::setMaxLength(int maxLength) {
		this->mMaxLength = maxLength;
	}

	int EditBox::getMaxLength() {
		return mMaxLength;
	}

	void EditBox::drawWidget() {
		if(!mFont) {
			return;
		}
		// TODO: Document why this is commented out or remove.
		//calcStrSize();
		//int x = mPaddedBounds.x;
		//int y = mPaddedBounds.y;
		int x = 0;
		int y = 0;

		Rect bound = mPaddedBounds;
		bound.x = 0;
		bound.y = 0;
		getTextStart(&bound.x, &bound.y);
		if(mMultiLine==false) {
			bound.height = 0xffff;
			bound.width = 0xffff;
		} 
		else {
			bound.x = 0;
		}

		// TODO: Document why this is commented out or remove.
//		Rect r = mFont->calculateRectOfIndex(mCursorIndex, text.c_str(), bound);
		Rect r = mFont->calculateRectOfIndex(mCursorIndex, mCaption.c_str(), bound);

		if(mMultiLine) {
			// 2 equals the cursor width ;)
			if(bound.y+r.y+r.height+2>(mPaddedBounds.height)) {
				y-=(bound.y+r.y+r.height+2)-(mPaddedBounds.height);
				// TODO: Document why this is commented out or remove.
				//Gfx_translate(x, y);
			}
			// TODO: Document why this is commented out or remove.
//			mFont->drawBoundedString(text.c_str(), x, y, bound);
//			Gfx_translate(x, y);
			mFont->drawBoundedString(mCaption.c_str(), bound.x+x, bound.y+y, bound);
			
		} else {
			// 2 equals the cursor width ;)
			if(bound.x+r.x+r.width+2>mPaddedBounds.width) {
				x-=(bound.x+r.x+r.width+2)-(mPaddedBounds.width);
				// TODO: Document why this is commented out or remove.
				//Gfx_translate(x, y);
			}
			// TODO: Document why this is commented out or remove.
//			mFont->drawString(text.c_str(), x, y);
			
			// TODO: Document why this is commented out or remove.
			//Gfx_translate(x, y);
			mFont->drawString(mCaption.c_str(), bound.x+x, bound.y+y);
		}

		// TODO: Document why this is commented out or remove.
		//Label::drawWidget();

		maSetColor(mCursorColor);
		if(mFocused) {
			// draw cursor
			int sx, sy, ex, ey;
			sx = r.x + x + bound.x;
			sy = r.y + y + bound.y;
			ex = sx;
			ey = sy+r.height;
			// TODO: Document why this is commented out or remove.
			//maLine(ex, sy, ex, ey);
			Gfx_line(sx, sy, ex, ey);
		}
	}

	void EditBox::keyPressEvent(int keyCode, int nativeCode) {
		switch(keyCode) {
			case MAK_LEFT:
				if(moveCursorHorizontal(-1)==false) return;
				break;
			case MAK_RIGHT:
				if(moveCursorHorizontal(1)==false) return;
				break;
			case MAK_CLEAR:
				// TODO: Document why this is commented out or remove.
				/*
				if(mCharacterInputActive) {
					CharInput::getCharInput().forceDeployment();
					moveCursorHorizontal(-1, false);
				}*/
				mCharacterInputActive = false;
				CharInput::getCharInput().forceDeployment();
				deletePreviousCharacter();
				return;
			case MAK_HASH:
				if(mCaseToggle && mInputMode == IM_STANDARD) {
					CharInput::getCharInput().setMode(
						(CharInput::CharMode)(
							((int)CharInput::getCharInput().getMode() + 1) 
							& 0x1));
				}
				break;
			default:
				return;
		}
		CharInput::getCharInput().forceDeployment();
		requestRepaint();
	}

	void EditBox::keyReleaseEvent(int keyCode, int nativeCode) {
	}

	int mCurrentIndex;

	void EditBox::characterChanged(char c) {
		// TODO: Document why this is commented out or remove.
//		if(text.size()==mMaxLength) return;

		if(mCharacterInputActive==false && mCaption.length()<mMaxLength) {
			// TODO: Document what should be fixed.
			// FIXME
			char tmp[2];
			sprintf(tmp, "%c", c);
			mCurrentIndex = mCursorIndex;
			// TODO: Document why this is commented out or remove.
			//text.insert(mCurrentIndex, tmp);
			mCaption.insert(mCurrentIndex, tmp);
			if(mPasswordMode) {
				mPassword.insert(mCurrentIndex, tmp);
			}
		} 
		else {
			if(mCharacterInputActive==false) {
				mCurrentIndex = mCursorIndex;
				if(mCurrentIndex>mCaption.length()-1) {
					mCurrentIndex = mCaption.length()-1;
				}
			}

			// TODO: Document why this is commented out or remove.
//			text[mCurrentIndex] = c;
			mCaption[mCurrentIndex] = c;
		}
		mCharacterInputActive = true;
		requestRepaint();
	
		fireCharacterChanged(c);
	
		// TODO: Document why this is commented out or remove.
		//calcStrSize();
		requestUpdate();
	}
	
	void EditBox::characterDeployed(char c) {
		// TODO: Document why this is commented out or remove.
		//if(text.length()>=mMaxLength+1) return;
		//if(mCaption.length()>=mMaxLength+1) return;

		mCharacterInputActive = false;
		if(mCaption.length()>=mMaxLength+1) return;
		
		if(mCursorIndex == mCurrentIndex) {
			// TODO: Document why this is commented out or remove.
			//mCursorIndex++;
			moveCursorHorizontal(1, false);
		}

		if(mPasswordMode) {
			mCaption[mCurrentIndex] = '*';
			mPassword[mCurrentIndex] = c;
		} else {
			mCaption[mCurrentIndex] = c;
		}
		fireTextChanged();
		fireCharacterAdded(c);
		requestRepaint();
	}

	bool EditBox::moveCursorHorizontal(int steps, bool fireListeners) {
		// TODO: Document why this is commented out or remove.
//		if(mCursorIndex + steps < 0 || mCursorIndex + steps > text.length()) return false;
		if (mCursorIndex + steps < 0 
			|| mCursorIndex + steps > mCaption.length()) 
		{
			if (fireListeners) {
				fireCursorMoved(mCursorIndex);
			}
			return false;
		}
	
		int lastCursor = mCursorIndex;
		mCursorIndex+=steps;
		if (fireListeners) {
			fireCursorMoved(lastCursor);
		}

		return true;
	}

	bool EditBox::deleteCharacter(int c) {
		// TODO: Document why this is commented out or remove.
		//text.remove(mCursorIndex-1, 1);
/*
		if(c<0 || c>=text.length()) return false;
		char deletedCharacter = text[mCursorIndex];
		text.remove(c, 1);
		*/
		if(c<0 || c>=mCaption.length()) {
			return false;
		}
		char deletedCharacter = mCaption[c];

		if(mPasswordMode) {
			mPassword.remove(c, 1);
			mCaption.remove(c, 1);
		} 
		else {
			mCaption.remove(c, 1);
		}

		// TODO: Document why this is commented out or remove.
/*
		for(int i = mCursorIndex; i < text.size(); i++) {
			text[i-1] = text[i];
		}
		text.resize(text.size()-1);
		mCursorIndex--;
*/
		fireCharacterDeleted(deletedCharacter);
		fireTextChanged();
		requestRepaint();
		
		// TODO: Document why this is commented out or remove.
		//calcStrSize();
		requestUpdate();

		return true;
	}

	bool EditBox::deleteCurrentCharacter() {
		return deleteCharacter(mCursorIndex);
	}

	bool EditBox::deletePreviousCharacter() {
		// TODO: Document why this is commented out or remove.
		/*	
		if(mCursorIndex-1<0) return false;
		char c = text[mCursorIndex];
		for(int i = mCursorIndex; i < text.size(); i++) {
			text[i-1] = text[i];
		}
		text.resize(text.size()-1);
		mCursorIndex--;
		fireTextChanged();
		fireCharacterDeleted(c);
		requestRepaint();
		return true;
		*/
		
		int expectedCursorIndex = mCursorIndex-1;
		bool r = deleteCharacter(expectedCursorIndex);
		if(!r) {
			return false;
		}
		mCursorIndex = expectedCursorIndex;
		// TODO: Document why this is commented out or remove.
		//if(mCursorIndex>text.length()) mCursorIndex = text.length();
		if(mCursorIndex>mCaption.length()) {
			mCursorIndex = mCaption.length();
		}

		return r;
	}

	void EditBox::setEnabled(bool enabled) {
		Widget::setEnabled(enabled);
		if(!enabled) {
			deactivate();
		}
	}

	void EditBox::addEditBoxListener(EditBoxListener *ebl) {
		mEditBoxListeners.add(ebl);
	}

	void EditBox::fireTextChanged() {
		for(int i = 0; i < mEditBoxListeners.size(); i++) {
			// TODO: Document why this is commented out or remove.
			//mEditBoxListeners[i]->textChanged(this, text);
			mEditBoxListeners[i]->textChanged(this, mCaption);
		}
	}

	void EditBox::fireCursorMoved(int lastCursor) {
		for(int i = 0; i < mEditBoxListeners.size(); i++) {
			mEditBoxListeners[i]->cursorMoved(
				this, 
				this->mCursorIndex, lastCursor);
		}
	}

	void EditBox::fireCharacterDeleted(char c) {
		for(int i = 0; i < mEditBoxListeners.size(); i++) {
			mEditBoxListeners[i]->characterDeleted(this, c);
		}
	}

	void EditBox::fireCharacterChanged(char c) {
		for(int i = 0; i < mEditBoxListeners.size(); i++) {
			mEditBoxListeners[i]->characterChanged(this, c);
		}
	}

	void EditBox::fireCharacterAdded(char c) {
		for(int i = 0; i < mEditBoxListeners.size(); i++) {
			mEditBoxListeners[i]->characterAdded(this, c);
		}
	}

	void EditBox::setCaseToggle(bool caseToggle) {
		this->mCaseToggle = caseToggle;
	}

	void EditBox::setPasswordMode(bool enabled) {
		if(enabled && !this->mPasswordMode) {
			mPassword = mCaption;
			for(int i=0; i<mCaption.length(); i++) {
				mCaption[i] = '*';
			}
			requestRepaint();
		}
		if(!enabled && this->mPasswordMode) {
			mCaption = mPassword;
			requestRepaint();
		}
		this->mPasswordMode = enabled;
	}
}

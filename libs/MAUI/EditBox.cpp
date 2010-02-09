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

	void EditBoxListener::cursorMoved(EditBox *eb, int newCursor, int lastCursor) {
	}

	void EditBoxListener::characterDeleted(EditBox *eb, char c) {
	}

	void EditBoxListener::characterChanged(EditBox *eb, char c) {
	}

	void EditBoxListener::characterAdded(EditBox *eb, char c) {
	}

	EditBox::EditBox(int x, int y, int width, int height, Widget* parent, const String &text,
		int backColor, Font* font, bool manageNavigation, bool multiLine, int maxLength, InputMode inputMode) :
		//Widget(x, y, width, height, parent),
		Label(x, y, width, height, parent, text, backColor, font),
	//	font(font),
		cursorIndex(0),
		manageNavigation(manageNavigation),
		active(false),
		cursorColor(0xffffff),
		characterInputActive(false),
	//	multiLine(multiLine),
		currentLine(0),
		maxLength(maxLength),
		inputMode(inputMode),
		editBoxListeners(),
		caseToggle(true),
		passwordMode(false)
	{
		setBackgroundColor(backColor);
		setDrawBackground(true);	

		setMultiLine(multiLine);

		setCaption(text);

		if(!font) {
			this->font = Engine::getSingleton().getDefaultFont();
		} else {
			this->font = font;
		}

		//this->text.reserve(maxLength);
		this->caption.reserve(maxLength);

		setInputMode(inputMode);
	}

	void EditBox::setSelected(bool selected) {
		if(!active&&selected==true) {
			activate();
		}
		else if(active&&selected==false) {
			deactivate();
		}
		Widget::setSelected(selected);
	}


	void EditBox::activate() {
		if(active) return;
		if(manageNavigation) {
			Environment::getEnvironment().addKeyListener(this);
		}
		CharInput::getCharInput().addCharInputListener(this);
		active = true;
		setInputMode(inputMode);
		setSelected(true);
	}

	void EditBox::deactivate() {
		if(!active) return;
		if(manageNavigation) {
			if(Environment::getEnvironment().isKeyListener(this))	
				Environment::getEnvironment().removeKeyListener(this);
		}

		characterInputActive = false;
		CharInput::getCharInput().forceDeployment();

		if(CharInput::getCharInput().isCharInputListener(this))
			CharInput::getCharInput().removeCharInputListener(this);
		active = false;
		setSelected(false);
	}

	bool EditBox::isActive() const {
		return active;
	}

	void EditBox::clearText() {
		setCaption("");
		password.clear();
	}

	void EditBox::setCaption(const String& caption) {
		setText(caption);
	}
	
	void EditBox::setText(const String& text) {
		//this->text = text;
		//if(cursorIndex>text.size()) cursorIndex = text.size();
		//this->caption = caption;
		Label::setCaption(text);
		if(cursorIndex>caption.length()) cursorIndex = caption.length();
		requestRepaint();
	}

	const String& EditBox:: getText() const {
		return caption;
	}

	/*
	void EditBox::setFont(Font *font) {
		this->font = font;
	}

	Font* EditBox::getFont() const {
		return font;
	}
	*/

	void EditBox::setCursorColor(int color) {
		cursorColor = color;
	}

	int EditBox::getCursorColor() const {
		return cursorColor;
	}

	void EditBox::setInputMode(EditBox::InputMode inputMode) {
		if(inputMode == IM_STANDARD) {
			CharInput::getCharInput().setMode(CharInput::LOWERCASE);
		} else if(inputMode == IM_NUMBERS) {
			CharInput::getCharInput().setMode(CharInput::NUMBERS);
		}
		this->inputMode = inputMode;
	}

	EditBox::InputMode EditBox::getInputMode() const {
		return inputMode;
	}

	void EditBox::setManageNavigation(bool b) {
		manageNavigation = b;
		if(manageNavigation) 
			Environment::getEnvironment().addKeyListener(this);
	}

	/*
	void EditBox::setMultiLine(bool b) {
		multiLine = b;
	}

	bool EditBox::isMultiLine() {
		return multiLine;
	}
	*/

	bool EditBox::isManagingNavigation() {
		return manageNavigation;
	}

	void EditBox::setMaxLength(int maxLength) {
		this->maxLength = maxLength;
	}

	int EditBox::getMaxLength() {
		return maxLength;
	}

	void EditBox::drawWidget() {
		if(!font) return;
		//calcStrSize();
		//int x = paddedBounds.x;
		//int y = paddedBounds.y;
		int x = 0;
		int y = 0;

		Rect bound = paddedBounds;
		bound.x = 0;
		bound.y = 0;
		getTextStart(&bound.x, &bound.y);
		if(multiLine==false) {
			bound.height = 0xffff;
			bound.width = 0xffff;
		} else {
			bound.x = 0;
		}

//		Rect r = font->calculateRectOfIndex(cursorIndex, text.c_str(), bound);
		Rect r = font->calculateRectOfIndex(cursorIndex, caption.c_str(), bound);

		if(multiLine) {
			// 2 equals the cursor width ;)
			if(bound.y+r.y+r.height+2>(paddedBounds.height)) {
				y-=(bound.y+r.y+r.height+2)-(paddedBounds.height);
				//Gfx_translate(x, y);
			}
//			font->drawBoundedString(text.c_str(), x, y, bound);
//			Gfx_translate(x, y);
			font->drawBoundedString(caption.c_str(), bound.x+x, bound.y+y, bound);
			
		} else {
			// 2 equals the cursor width ;)
			if(bound.x+r.x+r.width+2>paddedBounds.width) {
				x-=(bound.x+r.x+r.width+2)-(paddedBounds.width);
				//Gfx_translate(x, y);
			}
//			font->drawString(text.c_str(), x, y);
			
			//Gfx_translate(x, y);
			font->drawString(caption.c_str(), bound.x+x, bound.y+y);
		}

		//Label::drawWidget();

		maSetColor(cursorColor);
		if(selected) { 
			// draw cursor
			int sx, sy, ex, ey;
			sx = r.x + x + bound.x;
			sy = r.y + y + bound.y;
			ex = sx;
			ey = sy+r.height;
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
				/*
				if(characterInputActive) {
					CharInput::getCharInput().forceDeployment();
					moveCursorHorizontal(-1, false);
				}*/
				characterInputActive = false;
				CharInput::getCharInput().forceDeployment();
				deletePreviousCharacter();
				return;
			case MAK_HASH:
				if(caseToggle && inputMode == IM_STANDARD)
					CharInput::getCharInput().setMode((CharInput::CharMode)(((int)CharInput::getCharInput().getMode()+1)&0x1));
				break;
			default:
				return;
		}
		CharInput::getCharInput().forceDeployment();
		requestRepaint();
	}

	void EditBox::keyReleaseEvent(int keyCode, int nativeCode) {
	}

	int currentIndex;

	void EditBox::characterChanged(char c) {
//		if(text.size()==maxLength) return;

		if(characterInputActive==false && caption.length()<maxLength) {
			// FIXME
			char tmp[2];
			sprintf(tmp, "%c", c);
			currentIndex = cursorIndex;
			//text.insert(currentIndex, tmp);
			caption.insert(currentIndex, tmp);
			if(passwordMode)
				password.insert(currentIndex, tmp);
		} else {
			if(characterInputActive==false) {
				currentIndex = cursorIndex;
				if(currentIndex>caption.length()-1) currentIndex = caption.length()-1;
			}

//			text[currentIndex] = c;
			caption[currentIndex] = c;
		}
		characterInputActive = true;
		requestRepaint();
	
		fireCharacterChanged(c);
	
		//calcStrSize();
		mustCalcStrSize = true;
	}
	
	void EditBox::characterDeployed(char c) {
		//if(text.length()>=maxLength+1) return;
		//if(caption.length()>=maxLength+1) return;

		characterInputActive = false;
		if(caption.length()>=maxLength+1) return;
		
		if(cursorIndex == currentIndex) {
			//cursorIndex++;
			moveCursorHorizontal(1, false);
		}

		if(passwordMode) {
			caption[currentIndex] = '*';
			password[currentIndex] = c;
		} else {
			caption[currentIndex] = c;
		}
		fireTextChanged();
		fireCharacterAdded(c);
		requestRepaint();
	}

	bool EditBox::moveCursorHorizontal(int steps, bool fireListeners) {
//		if(cursorIndex + steps < 0 || cursorIndex + steps > text.length()) return false;
		if(cursorIndex + steps < 0 || cursorIndex + steps > caption.length()) {
			if(fireListeners)
				fireCursorMoved(cursorIndex);
			return false;
		}
	
		int lastCursor = cursorIndex;
		cursorIndex+=steps;
		if(fireListeners)
			fireCursorMoved(lastCursor);

		return true;
	}

	bool EditBox::deleteCharacter(int c) {
		//text.remove(cursorIndex-1, 1);
/*
		if(c<0 || c>=text.length()) return false;
		char deletedCharacter = text[cursorIndex];
		text.remove(c, 1);
		*/
		if(c<0 || c>=caption.length()) return false;
		char deletedCharacter = caption[c];
		caption.remove(c, 1);

/*
		for(int i = cursorIndex; i < text.size(); i++) {
			text[i-1] = text[i];
		}
		text.resize(text.size()-1);
		cursorIndex--;
*/
		fireCharacterDeleted(deletedCharacter);
		fireTextChanged();
		requestRepaint();
				
		//calcStrSize();
		mustCalcStrSize = true;

		return true;
	}

	bool EditBox::deleteCurrentCharacter() {
		return deleteCharacter(cursorIndex);
	}

	bool EditBox::deletePreviousCharacter() {
		/*	
		if(cursorIndex-1<0) return false;
		char c = text[cursorIndex];
		for(int i = cursorIndex; i < text.size(); i++) {
			text[i-1] = text[i];
		}
		text.resize(text.size()-1);
		cursorIndex--;
		fireTextChanged();
		fireCharacterDeleted(c);
		requestRepaint();
		return true;
		*/
		
		int expectedCursorIndex = cursorIndex-1;
		bool r = deleteCharacter(expectedCursorIndex);
		if(!r) return false;
		cursorIndex = expectedCursorIndex;
		//if(cursorIndex>text.length()) cursorIndex = text.length();
		if(cursorIndex>caption.length()) cursorIndex = caption.length();

		return r;
	}

	void EditBox::setEnabled(bool enabled) {
		Widget::setEnabled(enabled);
		if(!enabled)
			deactivate();
	}

	void EditBox::addEditBoxListener(EditBoxListener *ebl) {
		editBoxListeners.add(ebl);
	}

	void EditBox::fireTextChanged() {
		for(int i = 0; i < editBoxListeners.size(); i++) {
			//editBoxListeners[i]->textChanged(this, text);
			editBoxListeners[i]->textChanged(this, caption);
		}
	}

	void EditBox::fireCursorMoved(int lastCursor) {
		for(int i = 0; i < editBoxListeners.size(); i++) {
			editBoxListeners[i]->cursorMoved(this, this->cursorIndex, lastCursor);
		}
	}

	void EditBox::fireCharacterDeleted(char c) {
		for(int i = 0; i < editBoxListeners.size(); i++) {
			editBoxListeners[i]->characterDeleted(this, c);
		}
	}

	void EditBox::fireCharacterChanged(char c) {
		for(int i = 0; i < editBoxListeners.size(); i++) {
			editBoxListeners[i]->characterChanged(this, c);
		}
	}

	void EditBox::fireCharacterAdded(char c) {
		for(int i = 0; i < editBoxListeners.size(); i++) {
			editBoxListeners[i]->characterAdded(this, c);
		}
	}

	void EditBox::setCaseToggle(bool caseToggle) {
		this->caseToggle = caseToggle;
	}

	void EditBox::setPasswordMode(bool enabled) {
		if(enabled && !this->passwordMode) {
			password = caption;
			for(int i=0; i<caption.length(); i++) {
				caption[i] = '*';
			}
			requestRepaint();
		}
		if(!enabled && this->passwordMode) {
			caption = password;
			requestRepaint();
		}
		this->passwordMode = enabled;
	}

	void EditBox::setQwerty(bool enabled) {
		CharInput::getCharInput().setQwerty(enabled);
	}
	bool EditBox::getQwerty() const {
		return CharInput::getCharInput().getQwerty();
	}

}

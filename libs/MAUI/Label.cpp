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
#include "Label.h"
#include "Font.h"
#include "Engine.h"
#include <mastdlib.h>
#include <conprint.h>

#include <mavsprintf.h>

#define LLOG(...) //printf

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a):(b))
#endif

namespace MAUI {
	static void cutText(String& output, Font* font, const String& text, Rect mBounds)
	{
		output.clear();
		const Charset& cs = font->getCharset();
		const unsigned char *str = (const unsigned char*)text.c_str();
		int w = mBounds.width;
		int x = mBounds.x;
		int i = 0;
		while(*str) {
			const CharDescriptor& cd = cs.chars[*str];
			if(x+cd.xAdvance>w) {
				int numDots = 3;
				if(i<3) numDots = i;
				output.resize(i-numDots);
				memcpy(&output[0], text.c_str(), i-numDots);
				output.append("...", numDots);
				return;
			} else {
				i++;
			}
			x+=cd.xAdvance;
			str++;
		}
		output = text;
	}

	#if 0
	static int brightenColor(int c, int ammount) {
		int retc;
		unsigned char* col = (unsigned char *)&c;
		unsigned char* retCol = (unsigned char *)&retc;

		int r = MIN(((int)col[0])+ammount, 255);
		int g = MIN(((int)col[1])+ammount, 255);
		int b = MIN(((int)col[2])+ammount, 255);

		retCol[0] = (char)r;
		retCol[1] = (char)g;
		retCol[2] = (char)b;
		retCol[3] = col[3];
		return retc;
	}
	#endif

	Label::Label(int x, int y, int width, int height, Widget* parent) :
		Widget(x, y, width, height, parent), 
		mustCalcStrSize(true),
		caption(""),
		font(NULL),
		autoSizeX(false),
		autoSizeY(false),
		multiLine(false),
		horizontalAlignment(HA_LEFT),
		verticalAlignment(VA_TOP)
	{
		if(!font)
		{
			this->font = Engine::getSingleton().getDefaultFont();
		} else {
			this->font = font;
		}
		//calcStrSize();
		setDrawBackground();
		setBackgroundColor(0xffff00ff);
	}

	Label::Label(int x, int y, int width, int height, Widget* parent, const String &caption,
		int backColor, Font* font) :
		Widget(x, y, width, height, parent),
		mustCalcStrSize(true),
		caption(""),
		font(NULL),
		autoSizeX(false),
		autoSizeY(false),
		multiLine(false),
		horizontalAlignment(HA_LEFT),
		verticalAlignment(VA_TOP)
	{
		if(!font)
		{
			this->font = Engine::getSingleton().getDefaultFont();
		} else {
			this->font = font;
		}

		setCaption(caption);
		requestRepaint();
		this->setBackgroundColor(backColor);
		//calcStrSize();
	}

	void Label::calcStrSize() {
		mustCalcStrSize = false;
		Rect tempRect = Rect(0, 0, mPaddedBounds.width, mPaddedBounds.height);
		if(!font) {
			strSize = EXTENT(0,0);
		} else {
			if(autoSizeX)
				strSize = font->getStringDimensions(caption.c_str());
			else {
				if(multiLine) {	
					strSize = font->getBoundedStringDimensions(caption.c_str(), tempRect);
				} else {
					cutText(cuttedCaption, font, caption, tempRect);
					strSize = font->getStringDimensions(cuttedCaption.c_str());
				}
			}

		}
		strWidth  = EXTENT_X(strSize);
		strHeight = EXTENT_Y(strSize);

		if(autoSizeX) resize(strWidth + mPaddingLeft + mPaddingRight, mBounds.height);
		if(autoSizeY) resize(mBounds.width,  strHeight + mPaddingTop + mPaddingBottom);
	}

	void Label::setMultiLine(bool b) {
		//calcStrSize();
		mustCalcStrSize = true;
		multiLine = b;
	}

	bool Label::isMultiLine() {
		return multiLine;
	}

	void Label::setPaddingLeft(int l) {
		Widget::setPaddingLeft(l);
		//calcStrSize();
		mustCalcStrSize = true;
	}

	void Label::setPaddingTop(int t) {
		Widget::setPaddingTop(t);
		//calcStrSize();
		mustCalcStrSize = true;
	}

	void Label::setPaddingRight(int r) {
		Widget::setPaddingRight(r);
		//calcStrSize();
		mustCalcStrSize = true;
	}

	void Label::setPaddingBottom(int b) {
		Widget::setPaddingBottom(b);
		//calcStrSize();
		mustCalcStrSize = true;
	}

	void Label::resize(int width, int height) {
		Widget::setWidth(width);
		Widget::setHeight(height);
	}

	void Label::getTextStart(int *x, int *y) {
		//calcStrSize();
		if(!autoSizeX) {
			switch(horizontalAlignment) {
				case HA_LEFT: *x =   0; break;
				case HA_CENTER: *x = (mPaddedBounds.width)/2 - (strWidth/2); break;
				case HA_RIGHT: *x =  (mPaddedBounds.width) - (strWidth); break;
			}
		}

		if(!autoSizeY) {
			switch(verticalAlignment) {
				case VA_TOP: *y = 0; break;
				case VA_CENTER: *y = (mPaddedBounds.height)/2 - (strHeight/2); break;
				case VA_BOTTOM: *y = (mPaddedBounds.height) - (strHeight); break;
			}
		}
	}

	void Label::update() {
		Widget::update();
		if(mustCalcStrSize) {
			calcStrSize();
		}
	}

	void Label::drawWidget() {
		const char* wStr = caption.c_str();

		//int textX=mPaddedBounds.x, textY=mPaddedBounds.y;
		int textX=0, textY=0;
		//calcStrSize();

		getTextStart(&textX, &textY);

		Rect tempRect = Rect(0, 0, mPaddedBounds.width, mPaddedBounds.height);
		if(font) {
			if(multiLine)
				font->drawBoundedString(wStr, textX, textY, tempRect);
			else  {

				if(autoSizeX)
					font->drawString(wStr, textX, textY);
				else
					font->drawString(cuttedCaption.c_str(), textX, textY);
			}
		}
	}

	void Label::setHorizontalAlignment(Label::HorizontalAlignment alignment) {
		this->horizontalAlignment = alignment;
		requestRepaint();
		//calcStrSize();
		mustCalcStrSize = true;
	}

	Label::HorizontalAlignment Label::getHorizontalAlignment() const {
		return horizontalAlignment;
	}

	void Label::setVerticalAlignment(Label::VerticalAlignment alignment) {
		this->verticalAlignment = alignment;
		requestRepaint();
		//calcStrSize();
		mustCalcStrSize = true;
	}

	Label::VerticalAlignment Label::getVerticalAlignment() const {
		return verticalAlignment;
	}

	void Label::setAutoSizeX(bool f) {
		this->autoSizeX = f;
		//calcStrSize();
		mustCalcStrSize = true;
		requestRepaint();
	}

	void Label::setAutoSizeY(bool f) {
		this->autoSizeY = f;
		//calcStrSize();
		mustCalcStrSize = true;
		requestRepaint();
	}

	bool Label::getAutoSizeX() const {
		return autoSizeX;
	}

	bool Label::getAutoSizeY() const {
		return autoSizeY;
	}

	void Label::setCaption(const String& caption) {
		this->caption = caption;
		requestRepaint();
		//calcStrSize();
		mustCalcStrSize = true;
	}

	const String& Label::getCaption() const {
		return caption;
	}

	void Label::setFont(Font* font) {
		if(font)
			this->font = font;
		else
			this->font = Engine::getSingleton().getDefaultFont();
		requestRepaint();
		//calcStrSize();
		mustCalcStrSize = true;
	}

	Font* Label::getFont() const {
		return font;
	}


	void Label::setParameter(const String& name, const String& value) {
		if(name == "caption") this->setCaption(value);
		else if(name == "autoSizeX") this->setAutoSizeX(value=="true"?true:false);
		else if(name == "autoSizeY") this->setAutoSizeY(value=="true"?true:false);
		else if(name == "horizontalAlignment") {
			if(value == "left") setHorizontalAlignment(HA_LEFT);
			else if(value == "right") setHorizontalAlignment(HA_RIGHT);
			else if(value == "center") setHorizontalAlignment(HA_CENTER);
			else maPanic(0, "MAUI::Label wrong parameter");
		} 
		else if(name == "verticalAlignment") {
			if(value == "top") setVerticalAlignment(VA_TOP);
			else if(value == "bottom") setVerticalAlignment(VA_BOTTOM);
			else if(value == "center") setVerticalAlignment(VA_CENTER);
			else maPanic(0, "MAUI::Label wrong parameter");
		}
		else Widget::setParameter(name, value);
	}
}

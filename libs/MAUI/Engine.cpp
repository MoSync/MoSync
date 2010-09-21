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
#include "Engine.h"
#include <maheap.h>
#include <mastring.h>
#include <mastdlib.h>
#include <conprint.h>

#include <MAUtil/Graphics.h>

using namespace MAUtil;

//#define PUSH_EMPTY_CLIPRECT pushClipRect(0,0,0,0)

#define EXTENT(x, y) ((MAExtent)((((int)(x)) << 16) | ((y) & 0xFFFF)))

namespace MAUI {
	Engine* Engine::mSingletonPtr = 0;
	
	Engine::Engine()
	{
		mMain = NULL;
		mDefaultFont = NULL;
		mDefaultSkin = NULL;
		mOverlay = NULL;
		mSingletonPtr = this;
		mDisplayConsole = false;
		//clipStackPtr = -1;
		Environment::getEnvironment().addFocusListener(this);
	}
	
	void Engine::setMain(Widget* mMain) {
		mMain->setPosition(mMain->getPosition().x, mMain->getPosition().y);
		this->mMain = mMain;
	}

	Engine::~Engine()
	{
		if(mMain)
			delete mMain;
		mSingletonPtr = 0;
	}

	void Engine::setDefaultSkin(WidgetSkin* systemSkin) {
		this->mDefaultSkin = systemSkin;
	}

	void Engine::setDefaultFont(Font* defaultFont) {
		this->mDefaultFont = defaultFont;
	}

	WidgetSkin* Engine::getDefaultSkin() {
		return mDefaultSkin;
	}

	Font* Engine::getDefaultFont()
	{
		return mDefaultFont;
	}

/*	void Engine::clearClipRect()
	{
		clipStackPtr = -1;
	}

	void Engine::pushClipRect(int left, int top, int width, int height)
	{
		if(clipStackPtr >= MAX_WIDGET_DEPTH-1) {
			PANIC_MESSAGE("Clip stack broken");
			//printf("BIG FUCKUP!!!\n");
			return;
		}

		maSetClipRect(left, top, width, height);
		
		clipStackPtr++;
		clipStack[clipStackPtr].left = left; 
		clipStack[clipStackPtr].top = top; 
		clipStack[clipStackPtr].width = width; 
		clipStack[clipStackPtr].height = height; 		
	}

	bool Engine::pushClipRectIntersect(int left, int top, int width, int height)
	{	
		int pLeft = clipStack[clipStackPtr].left;
		int pTop = clipStack[clipStackPtr].top;
		int pWidth = clipStack[clipStackPtr].width;
		int pHeight = clipStack[clipStackPtr].height;

		if((!pWidth) || (!pHeight)) {
			PUSH_EMPTY_CLIPRECT;
			return false;
		}

		if(left + width < pLeft)
		{
			PUSH_EMPTY_CLIPRECT;
			return false;
		}

		if(top + height < pTop)
		{
			PUSH_EMPTY_CLIPRECT;
			return false;
		}

		if(left > pLeft + pWidth)
		{
			PUSH_EMPTY_CLIPRECT;
			return false;
		}

		if(top > pTop + pHeight)
		{
			PUSH_EMPTY_CLIPRECT;
			return false;
		}
		
		if( left < pLeft)
		{
			width -= pLeft - left;
			left =  pLeft;
		}

		if( top < pTop)
		{
			height -= pTop - top;
			top =  pTop;
		}

		if( left + width > pLeft + pWidth)
		{
			width -= (left + width) - (pLeft + pWidth);
		}

		if( top + height > pTop + pHeight)
		{
			height -= (top + height) - (pTop + pHeight);
		}

		pushClipRect(left,top,width,height);

		if(!width || !height) return false;
		else return true;
	}

	void Engine::popClipRect()
	{
		clipStackPtr--;
		if(clipStackPtr <= -1) clipStackPtr = -1;
		else maSetClipRect(	
			clipStack[clipStackPtr].left, 
			clipStack[clipStackPtr].top, 
			clipStack[clipStackPtr].width, 
			clipStack[clipStackPtr].height);
	}
*/	

	void Engine::focusLost() {
	}

	void Engine::focusGained() {
		if(mMain) mMain->requestRepaint();
		if(mOverlay) mOverlay->requestRepaint();
	}

	void Engine::requestUIUpdate() {
		Environment::getEnvironment().addIdleListener(this);
	}

	void Engine::setDisplayConsole(bool dc) {
		mDisplayConsole = dc;
	}


	void Engine::repaint() {
		//lprintfln("repaint @ (%i ms)", maGetMilliSecondCount());
		if(!mMain) return;
		//printf("doing repaint!");
		
		//clearClipRect();
		Gfx_clearClipRect();
		Gfx_clearMatrix();
		
		int scrW = EXTENT_X(maGetScrSize());
		int scrH = EXTENT_Y(maGetScrSize());

		// TODO: remove this!!!
		maSetColor(0);
		Gfx_fillRect(0, 0, scrW, scrH);

		//printf("screenSize: (%d, %d)\n", scrW, scrH);
		Gfx_pushClipRect(0, 0, scrW, scrH);
		mMain->update();
		mMain->draw();
		Gfx_popClipRect();

		if(mOverlay) {
			Gfx_clearClipRect();
			Gfx_clearMatrix();
			Gfx_pushClipRect(0, 0, scrW, scrH);

			mOverlay->requestRepaint();
			Gfx_translate(mOverlayPosition.x, mOverlayPosition.y);
			mOverlay->update();
			mOverlay->draw();
		}

		if(mDisplayConsole) {
			DisplayConsole();
		}
		else
			maUpdateScreen();
	}
	
	void Engine::idle() {
		//printf("Engine::idle()\n");
		repaint();
		Environment::getEnvironment().removeIdleListener(this);
	}

	Engine&	Engine::getSingleton() {
		if(!mSingletonPtr)
			mSingletonPtr = new Engine();
		return *mSingletonPtr;
	}



	/* is an mOverlay shown? */
	bool Engine::isOverlayShown() {
		return mOverlay!=NULL;
	}

	/* shows the mOverlay widget (passed as an argument). Put the top left
	corner at position x and y. */
	void Engine::showOverlay(int x, int y, Widget *overlay) {
		mOverlayPosition.x = x;
		mOverlayPosition.y = y;
		this->mOverlay = overlay;
		mOverlay->requestRepaint();
		mMain->requestRepaint();
	}
		
	/* hide the currently shown mOverlay. */
	void Engine::hideOverlay() {
		mOverlay = NULL;
		mMain->requestRepaint();
	}

	void Engine::setDefaultStyle(const String& widgetType, Style* style) {
		mDefaultStyles.insert(widgetType, style);
	}

	const Style* Engine::getDefaultStyle(const String& widgetType) const {
		Map<String, Style*>::ConstIterator iter = mDefaultStyles.find(widgetType);
		if(iter != mDefaultStyles.end()) return iter->second;
		else return NULL;
	}

}

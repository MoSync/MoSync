/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <ma.h>
#include "Engine.h"
#include <maheap.h>
#include <mastring.h>
#include <mastdlib.h>

#include <MAUtil/Graphics.h>

//#define PUSH_EMPTY_CLIPRECT pushClipRect(0,0,0,0)

#define EXTENT(x, y) ((MAExtent)((((int)(x)) << 16) | ((y) & 0xFFFF)))

namespace MAUI {
	Engine* Engine::singletonPtr = 0;
	
	Engine::Engine()
	{
		main = NULL;
		defaultFont = NULL;
		defaultSkin = NULL;
		overlay = NULL;
		singletonPtr = this;
		//clipStackPtr = -1;
		Environment::getEnvironment().addFocusListener(this);
	}
	
	void Engine::setMain(Widget* main) {
		main->setPosition(main->getPosition().x, main->getPosition().y);
		this->main = main;
	}

	Engine::~Engine()
	{
		if(main)
			delete main;
		singletonPtr = 0;
	}

	void Engine::setDefaultSkin(WidgetSkin* systemSkin) {
		this->defaultSkin = systemSkin;
	}

	void Engine::setDefaultFont(Font* defaultFont) {
		this->defaultFont = defaultFont;
	}

	WidgetSkin* Engine::getDefaultSkin() {
		return defaultSkin;
	}

	Font* Engine::getDefaultFont()
	{
		return defaultFont;
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
		requestUIUpdate();
	}

	void Engine::requestUIUpdate() {
		Environment::getEnvironment().addIdleListener(this);
	}
	
	void Engine::repaint() {
		//lprintfln("repaint @ (%i ms)", maGetMilliSecondCount());
		if(!main) return;
		//printf("doing repaint!");
		
		Gfx_beginRendering();
		
		//clearClipRect();
		Gfx_clearClipRect();
		Gfx_clearMatrix();

		
		int scrW = EXTENT_X(maGetScrSize());
		int scrH = EXTENT_Y(maGetScrSize());
		//printf("screenSize: (%d, %d)\n", scrW, scrH);
		Gfx_pushClipRect(0, 0, scrW, scrH);
		main->update();
		main->draw();
		Gfx_popClipRect();

		if(overlay) {
			Gfx_clearClipRect();
			Gfx_clearMatrix();
			Gfx_pushClipRect(0, 0, scrW, scrH);

			overlay->requestRepaint();
			Gfx_translate(overlayPosition.x, overlayPosition.y);
			overlay->update();
			overlay->draw();
		}

		//maUpdateScreen();
		Gfx_updateScreen();
	}
	
	void Engine::idle() {
		//printf("Engine::idle()\n");
		repaint();
		Environment::getEnvironment().removeIdleListener(this);
	}

	Engine&	Engine::getSingleton() {
		if(!singletonPtr)
			singletonPtr = new Engine();
		return *singletonPtr;
	}



	/* is an overlay shown? */
	bool Engine::isOverlayShown() {
		return overlay!=NULL;
	}

	/* shows the overlay widget (passed as an argument). Put the top left
	corner at position x and y. */
	void Engine::showOverlay(int x, int y, Widget *overlay) {
		overlayPosition.x = x; 
		overlayPosition.y = y;
		this->overlay = overlay;
		overlay->requestRepaint();
		main->requestRepaint();
	}
		
	/* hide the currently shown overlay. */
	void Engine::hideOverlay() {
		overlay = NULL;
		main->requestRepaint();
	}

}

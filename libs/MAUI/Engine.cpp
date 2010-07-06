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

#include <MAUI/Keyboard/VirtualKeyboard.h>
#include <MAUI/Keyboard/VirtualKeyboardFactory.h>

#include <MAUI/Layout.h>
#include <MAUtil/Graphics.h>

//#define PUSH_EMPTY_CLIPRECT pushClipRect(0,0,0,0)

#define EXTENT(x, y) ((MAExtent)((((int)(x)) << 16) | ((y) & 0xFFFF)))

extern char VIRTUAL_KEYBOARD_DEFAULT_XML[];

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
		m_keyboardVisible = false;
		Environment::getEnvironment().addFocusListener(this);

		// Setup virtual keyboard
		m_keyboard = VirtualKeyboardFactory::createFromXMLString(VIRTUAL_KEYBOARD_DEFAULT_XML);
		MAExtent screenSize = maGetScrSize();
		m_keyboard->setPosition(0, EXTENT_Y(screenSize) - m_keyboard->getHeight());
	}
	
	void Engine::setMain(Widget* main) {
		/* The keyboard must be hidden before the main widget is updated. */
		hideKeyboard();

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
		if(!main) {
			return;
		}
		
		//clearClipRect();
		Gfx_clearClipRect();
		Gfx_clearMatrix();
		
		int scrW = EXTENT_X(maGetScrSize());
		int scrH = EXTENT_Y(maGetScrSize());

		int mainWidgetHeight = main->getHeight();
		int mainWidgetWidth = main->getWidth();

		//printf("screenSize: (%d, %d)\n", scrW, scrH);
		/* Draw main widget */
		Gfx_pushClipRect(0, 0, mainWidgetWidth, mainWidgetHeight);
		main->update();
		main->draw();
		Gfx_popClipRect();

		/* Draw overlay */
		if(overlay) {
			Gfx_clearClipRect();
			Gfx_clearMatrix();
			Gfx_pushClipRect(0, 0, scrW, scrH);

			overlay->requestRepaint();
			Gfx_translate(overlayPosition.x, overlayPosition.y);
			overlay->update();
			overlay->draw();
		}

		/* If the keyboard is visible it should be drawn,
		   is is assumed that its position has been set up 
		   correctly. */
		if(keyboardVisible()) {
			Gfx_clearClipRect();
			Gfx_clearMatrix();
			Gfx_pushClipRect(0, scrH - m_keyboard->getHeight(), m_keyboard->getWidth(), scrH);

			m_keyboard->update();
			m_keyboard->draw();

			Gfx_popClipRect();
		}

		maUpdateScreen();
	}
	
	void Engine::idle() {
		//printf("Engine::idle()\n");
		repaint();
		Environment::getEnvironment().removeIdleListener(this);
	}

	Engine&	Engine::getSingleton() {
		if(!singletonPtr) {
			singletonPtr = new Engine();
		}
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

	void Engine::showKeyboard()
	{
		if(!m_keyboardVisible)
		{
			m_keyboardVisible = true;
			m_mainHeight = main->getHeight();

			/* Change height of main widget if it is too big */
			int screenHeight = EXTENT_Y(maGetScrSize());
			if(screenHeight - main->getHeight() <  m_keyboard->getHeight()) {
				main->setHeight(screenHeight - m_keyboard->getHeight());
			}

			/* Keyboard produces qwerty output */
			CharInput::getCharInput().setQwerty(true);

			/* Let the keyboard listen to pointer events as long as it is active */
			Environment::getEnvironment().addPointerListener(m_keyboard);

			/* Reset the keyboard to the default state and display it */
			m_keyboard->reset();

			Environment::getEnvironment().fireVKShownEvent();

			main->requestRepaint();
			requestUIUpdate();
		}
	}

	void Engine::hideKeyboard()
	{
		if(m_keyboardVisible)
		{
			m_keyboardVisible = false;
			main->setHeight(m_mainHeight);

			/* The keyboard should not respond to pointer events when hidden */
			Environment::getEnvironment().removePointerListener(m_keyboard);

			/* Clear screen */
			MAExtent screenSize = maGetScrSize();
			maSetColor(0);
			Gfx_fillRect(0, EXTENT_Y(screenSize) - m_keyboard->getHeight( ), 
						 EXTENT_X(screenSize), EXTENT_Y(screenSize));

			Environment::getEnvironment().fireVKHiddenEvent();

			main->requestRepaint();
			requestUIUpdate();
		}
	}

	bool Engine::keyboardVisible()
	{
		return m_keyboardVisible;
	}

	bool Engine::onKeyboard(int x, int y)
	{
		if(!keyboardVisible())
		{
			return false;
		}
		
		MAExtent screenSize = maGetScrSize();
		if(y > EXTENT_Y(screenSize) - m_keyboard->getHeight()) {
			return true;
		}
		else {
			return false;
		}
	}
}

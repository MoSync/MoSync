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

#include "DefaultSkin.h"

#include "Button.h"
#include "Slider.h"

using namespace MAUtil;

#define EXTENT(x, y) ((MAExtent)((((int)(x)) << 16) | ((y) & 0xFFFF)))

namespace MAUI {
	Engine* Engine::mSingletonPtr = 0;

	MAHandle imageFromMem(void *data, int size) {
		MAHandle res = maCreatePlaceholder();
		MAHandle img = maCreatePlaceholder();
		maCreateData(res, size);
		maWriteData(res, data, 0, size);
		maCreateImageFromData(img, res, 0, size);
		maDestroyObject(res);
		return img;
	}

	MAHandle fontFromMem(void *data, int size) {
		MAHandle res = maCreatePlaceholder();
		maCreateData(res, size);
		maWriteData(res, data, 0, size);
		return res;
	}

#define RESIMG(r) imageFromMem((r), sizeof(r))
#define RESFNT(r) fontFromMem((r), sizeof(r))
	
	Engine::Engine()
	{
		mMain = NULL;
		mOverlay = NULL;
		mSingletonPtr = this;
		mDisplayConsole = false;
		Environment::getEnvironment().addFocusListener(this);


		// setup default styles

		MAHandle s1  = RESIMG(button_pressed_selected);
		MAHandle s2  = RESIMG(button_notpressed_selected);
		MAHandle s3  = RESIMG(button_notpressed_unselected);

		SkinProperty* focusedPressed = new SkinProperty(s1, 19, 32, 19, 32, true);
		SkinProperty* focusedReleased = new SkinProperty(s2, 19, 32, 19, 32, true);
		SkinProperty* unfocusedReleased = new SkinProperty(s3, 19, 32, 19, 32, true);
		FontProperty* font = new FontProperty(RESFNT(arial));
		ButtonStyle* buttonStyle = new ButtonStyle(focusedPressed, focusedReleased, unfocusedReleased, font);
		mSingletonPtr->setDefaultStyle("Button", buttonStyle);

		s1  = RESIMG(slider_left_unselected);
		s2  = RESIMG(slider_right_unselected);
		s3  = RESIMG(slider_handle);
		SkinProperty* sliderAmountSkin = new SkinProperty(s1, 10, 12, 0, 18, true);
		SkinProperty* sliderBackgroundSkin = new SkinProperty(s2, 10, 12, 0, 18, true);
		ImageProperty* sliderGripImage = new ImageProperty(s3);
		SliderStyle* sliderStyle = new SliderStyle(sliderAmountSkin, sliderBackgroundSkin, sliderGripImage);
		mSingletonPtr->setDefaultStyle("Slider", sliderStyle);

		Style* widgetStyle = new Style();
		widgetStyle->set("backgroundSkinFocused", NULL);
		widgetStyle->set("backgroundSkinUnfocused", NULL);
		mSingletonPtr->setDefaultStyle("Widget", widgetStyle);

		Style* labelStyle = new LabelStyle(font);
		mSingletonPtr->setDefaultStyle("Label", labelStyle);
	}
	
	void Engine::setMain(Widget* main) {
		if(main)
			main->setPosition(main->getPosition().x, main->getPosition().y);
		this->mMain = main;
	}

	Engine::~Engine()
	{
		if(mMain)
			delete mMain;
		mSingletonPtr = 0;
	}

	void Engine::focusLost() {
	}

	void Engine::focusGained() {
		if(mMain) mMain->requestRepaint();
		if(mOverlay) mOverlay->requestRepaint();
	}

	void Engine::requestUIUpdate() {
		//maReportCallStack();
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
		//maSetColor(0);
		//Gfx_fillRect(0, 0, scrW, scrH);

		//printf("screenSize: (%d, %d)\n", scrW, scrH);
		Gfx_pushClipRect(0, 0, scrW, scrH);
		mMain->update();
		mMain->draw();
		Gfx_popClipRect();

		if(mOverlay) {
			Gfx_clearClipRect();
			Gfx_clearMatrix();
			Gfx_pushClipRect(0, 0, scrW, scrH);

			mOverlay->requestRepaint(); // won't add the idle listener again just setDirty(true).
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
		if(!mSingletonPtr) {
			mSingletonPtr = new Engine();
		}
		return *mSingletonPtr;
	}

	Widget* Engine::currentOverlay(Point& p) {
		p = mOverlayPosition;
		return mOverlay;
	}

	/* shows the mOverlay widget (passed as an argument). Put the top left
	corner at position x and y. */
	void Engine::showOverlay(int x, int y, Widget *overlay, OverlayListener* listener) {
		mOverlayListener = listener;
		mOverlayPosition.x = x;
		mOverlayPosition.y = y;
		overlay->setPosition(x, y);
		this->mOverlay = overlay;
		mOverlay->requestRepaint();
		mMain->requestRepaint();
	}

	OverlayListener::OutsideResponse Engine::fireOverlayEvent(int x, int y) {
		if(mOverlayListener)
			return mOverlayListener->pointerPressedOutsideOverlay(x, y);
		else
			return OverlayListener::eProceed;
	}

	/* hide the currently shown mOverlay. */
	void Engine::hideOverlay() {
		mOverlay = NULL;
		mMain->requestRepaint();
	}

	void Engine::setDefaultStyle(const String& widgetType, Style* style) {
		mDefaultStyles.insert(widgetType, style);
	}

	Style* Engine::getDefaultStyle(const String& widgetType) {
		Map<String, Style*>::ConstIterator iter = mDefaultStyles.find(widgetType);
		if(iter != mDefaultStyles.end())
			return iter->second;
		iter = mDefaultStyles.find("Widget");
		if(iter == mDefaultStyles.end())
			maPanic(1, "No style set (not even a default style for Widget is available!");
		return iter->second;
	}

}

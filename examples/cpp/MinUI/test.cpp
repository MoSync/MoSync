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

//******************************************************************************
// MinUI test program
//******************************************************************************

#include <conprint.h>
#include "MinUI.h"
#include <MAUtil/Moblet.h>

#include "MAHeaders.h"

using namespace MinUI;

#define GREEN 0x40FF40
#define BRIGHT_RED 0xFF8080
#define SKY_BLUE 0x8080FF

//******************************************************************************
// Simple Layout-Label View
//******************************************************************************
#if 0
class MyView : public View {
public:
	MyView() : mLayout(NULL, 2) {
		View::mRootWidget = &mLayout;
		mLayout.add(new Label(&mLayout, "Hello", GREEN), 0);
		mLayout.add(new Label(&mLayout, "World", GREEN), 1);
		mLayout.add(new Label(&mLayout, "2", GREEN), 1);
		mLayout.add(new Label(&mLayout, "0", GREEN), 0);
		mLayout.add(new Label(&mLayout, "3", GREEN), 1);
		mLayout.add(new Label(&mLayout, "4", GREEN), 1);
		Extent s = maGetScrSize();
		MAPoint2d scrSize = { EXTENT_X(s), EXTENT_Y(s) };
		mLayout.setSize(scrSize);
	}
	virtual ~MyView() {}

	//View
	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0 || keyCode == MAK_SOFTRIGHT)
			maExit(0);
	}
	void keyReleaseEvent(int keyCode, int nativeCode) {
	}
private:
	GridLayout mLayout;
};
#endif
//******************************************************************************
// View with everything
//******************************************************************************

class CompleteView : public View, public HyperlinkListener {
public:
	CompleteView() : mMainScreen(1), mSecondaryScreen( 1 )  {
		View::mRootWidget = &mMainScreen;
		mMainScreen.setBackground(true);

		// Init main screen
		GridLayout* layout = new GridLayout(2);

		layout->add(new Image(R_IMAGE_1), 0);
		layout->add(new Image(R_IMAGE_2), 0);
		layout->add(new Label("Hello World!", GREEN), 1);	//should wind up on the top left?
		layout->add(new StringLabel("String", GREEN), 1);
		layout->add(new Textbox(10, 20, GREEN, GREEN), 1);

		mMainScreen.add(layout, 0, -1, true);
		Textbox* t = new Textbox(10, 200, GREEN, WHITE/*BRIGHT_RED*/);
		mMainScreen.add(t, 0, -1, true);
		t->activate();
		mMainScreen.add(new MultilineLabel("Line 1\nLine 10\nYet another line\n ", GREEN), 0);
		mMainScreen.add(new Label("Next label", WHITE), 0);
		mMainScreen.add(new HyperlinkLabel(this, "To close the program, \1click here\1.\n Now for a \1line-\nbroken\1 link.", GREEN, SKY_BLUE), 0);

		// Init secondary screen
		mSecondaryScreen.setBackground( true );
		mSecondaryScreen.add(new HyperlinkLabel(this, "\1Exit\1", GREEN, SKY_BLUE ), 0);

	}
	virtual ~CompleteView() {}

	//View
	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0 || keyCode == MAK_SOFTRIGHT)
			maExit(0);
		if(keyCode == MAK_FIRE)
			mMainScreen.draw();
	}
	void keyReleaseEvent(int keyCode, int nativeCode) {
	}

	//HyperlinkListener
	void click(int linkIndex) {
		switch ( linkIndex )
		{
			case 0:
				maExit(0);

			case 1:
				setView( &mSecondaryScreen );
				break;
		}
	}

private:
	void setView ( Widget * wid ) {
		mRootWidget = wid;
		show( );
	}

	GridLayout mMainScreen;
	GridLayout mSecondaryScreen;
};

//******************************************************************************
// Moblet
//******************************************************************************

class MyMoblet : public MAUtil::Moblet {
public:
	MyMoblet() {
		mView.show();
	}
private:
	CompleteView mView;
};

extern "C" int MAMain() {
	MAUtil::Moblet::run(new MyMoblet());
	return 0;
};

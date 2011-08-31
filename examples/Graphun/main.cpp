/*
 Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

#include <MAUtil/Moblet.h>
#include <stdio.h>

#include "UIFramework/Screen.h"
#include "UIFramework/ListSelectionScreen.h"
#include "grid.h"

#include "MAHeaders.h"

using namespace MAUtil;
using namespace MoSync;
using namespace UI;

namespace Graphun {
static const char
		* sHelpText =
				"Graphun is a tool to visualize 3D graphs. The z-coordinates of the points on the plane are "
					"calculated using the custom expression (editable using the input text field). The z-values are "
					"limited to values between -1 and +1. In your expression you can use different variables and functions "
					"to model the shape of the plane and see how it is affected.\n\n"
					"These variables and constants are available:\n\n"
					"'x' Is the x-coordinate for the point being evaluated (a value between -0.5 and 0.5)\n"
					"'y' Is the y-coordinate for the point being evaluated (a value between -0.5 and 0.5)\n"
					"'time' A frame independant timer that moves from time to time+1 in one second (millisecond resolution). This comes in handy if you want to do animations\n"
					"'pi' Equals to pi (or 3.14159....)\n\n"
					"These functions are available (x can be any sub-expression):\n\n"
					"'tan(x)' Calculates the tangent of x\n"
					"'sin(x)' Calculates the sine of x\n"
					"'cos(x)' Calculates the cosine of x\n"
					"'sqrt(x)' Calculates the square-root of x\n"
					"'abs(x)' Calculates the absolute value of x\n\n"
					"Press any of the following buttons to load the example expressions:\n\n";

static const char
		* sAboutText =
				"Graphun (version 1.0)\nMoSync AB 2011\n\nMade using MoSync SDK\nSee www.mosync.com for more information.\n\n"
					"Icons by Glyphish (www.glyphish.com)\n";

/**
 * A generic text screen. It has a vertical layout where the first item is a label with multiple lines of text.
 * If a handle to an image is sent as the last argument to the constructor it is used to create an image widget
 * at the top.
 */
class TextScreen: public Screen {
public:
	TextScreen(const String& title, const String& text, MAHandle topImage = 0) {
		const int padding = 20;

		setProperty("title", title);
		mLayout = new Widget("VerticalLayout");
		mLayout->setProperty("backgroundColor", "000000");

		if (topImage) {
			Widget* layout = new Widget("VerticalLayout");
			layout->setProperty("backgroundColor", "00000000");
			layout->setProperty("width", -1);
			layout->setProperty("height", -2);
			layout->setProperty("childHorizontalAlignment", "center");

			Widget* image = new Widget("Image");
			image->setProperty("image", topImage);
			image->setProperty("width", "279");
			image->setProperty("height", "99");

			layout->addChild(image);
			mLayout->addChild(layout);
		}

		mLabel = new Widget("Label");
		mLabel->setProperty("text", text.c_str());
		mLabel->setProperty("fontColor", "ffffff");
		mLabel->setProperty("textVerticalAlignment", "top");
		mLabel->setProperty("textHorizontalAlignment", "left");
		mLabel->setProperty("width", -1);
		mLabel->setProperty("height", -2);
		mLabel->setProperty("maxNumberOfLines", "0");

		mLayout->setProperty("width", -1);
		mLayout->setProperty("height", -1);
		mLayout->setProperty("leftMargin", padding);
		mLayout->setProperty("rightMargin", padding);
		mLayout->setProperty("topMargin", padding);
		mLayout->setProperty("bottomMargin", padding);
		mLayout->setProperty("spacing", "5");
		mLayout->setProperty("isScrollable", "true");

		mLayout->addChild(mLabel);
		setMainWidget(mLayout);
	}

	virtual ~TextScreen() {
	}

protected:
	Widget* mLabel;
	Widget* mLayout;

};

/**
 * The settings screen.
 */
class SettingsScreen: public Screen {
public:
	SettingsScreen();
	virtual ~SettingsScreen() {
	}

	void checkboxStateChanged(UIItem* item, bool checked);
	void listboxItemSelected(UIItem* item, int index);
protected:
	Widget* mListView;

	Widget* mShadingCheckBox;
	Widget* mSetResolutionItem;

};

/**
 * The main screen with the graph view.
 */
class MainScreen: public Screen, public IdleListener, public PointerListener {
protected:
	Widget* mEditBox;
	Widget* mGLView;
	Widget* mSettingsButton;
	Grid *grid;
	MAPoint2d lastPoint;

	bool mRotating;
	double mRotationStart;
	double mRotationVelocity;
	double mStartTime;
	String mExpression;

	bool mDrawing;
public:
	Grid* getGrid();
	MainScreen();
	void setExpression(const String& exp);
	virtual void buttonClicked(UIItem* item);
	virtual void editBoxReturned(UIItem* item);
	virtual void glViewReady(UIItem* item);
	void idle();
	void willAppear();
	void hide();
	void pointerPressEvent(MAPoint2d p);
	void pointerReleaseEvent(MAPoint2d p);
	void pointerMoveEvent(MAPoint2d p);

	void setDrawing(bool b) {
		mDrawing = b;
	}

};

MainScreen *sMainScreen = NULL;
SettingsScreen *sSettingsScreen = NULL;
TextScreen* sHelpScreen = NULL;
TextScreen* sAboutScreen = NULL;

StackScreen *sStackScreen = NULL;
ListSelectionScreen* sResolutionSelector = NULL;

/**
 * The help screen. It inherits from the text screen.
 */
class HelpScreen: public TextScreen {
public:
	HelpScreen() :
		TextScreen("Help", sHelpText) {
		Vector<String> expressions;
		expressions.add("x*y");
		expressions.add("cos(x*pi*2.0+time)*0.5");
		expressions.add("sin(sqrt(x*x+y*y)*10+time)*0.5");

		for (int i = 0; i < expressions.size(); i++) {
			Widget* button = new Widget("Button");
			button->setProperty("text", expressions[i].c_str());
			button->setProperty("width", -1);
			button->setProperty("height", -2);
			mLayout->addChild(button);
			mExpressions[button] = expressions[i];
		}
	}
	virtual ~HelpScreen() {
	}

	virtual void buttonClicked(UIItem* item) {
		const String& exp = mExpressions[item];
		sMainScreen->setExpression(exp);
		sStackScreen->pop();
		//sStackScreen->pop();
	}

protected:
	Map<UIItem*, String> mExpressions;

};

Grid* MainScreen::getGrid() {
	return grid;
}

MainScreen::MainScreen() {
	mDrawing = false;

	setProperty("title", "Graphun");

	Widget* layout = new Widget("VerticalLayout");
	layout->setProperty("width", -1);
	layout->setProperty("height", -1);
	setMainWidget(layout);

	layout->setProperty("backgroundColor", "000000");
	layout->setProperty("childHorizontalAlignment", "center");

	Widget* horizontalLayout = new Widget("HorizontalLayout");
	horizontalLayout->setProperty("childVerticalAlignment", "center");
	horizontalLayout->setProperty("width", "-1");
	horizontalLayout->setProperty("height", -2); // should be -2?

	Widget* label = new Widget("Label");
	label->setProperty("text", " z =");
	label->setProperty("fontColor", "ffffff");
	label->setProperty("fontSize", 16);
	label->setProperty("width", -2);
	label->setProperty("height", -2);
	label->setProperty("backgroundColor", "000000");
	horizontalLayout->addChild(label);

	mExpression = "sin(x*10.0*y+time)*0.5";
	mEditBox = new Widget("EditBox");
	mEditBox->setProperty("width", -1);
	mEditBox->setProperty("height", -2);
	mEditBox->setProperty("placeholder", "Expression (x,y,sin,cos,sqrt,time)");
	mEditBox->setProperty("text", mExpression.c_str());
	mEditBox->setProperty("autocorrectionEnabled", "false");
	horizontalLayout->addChild(mEditBox);

	layout->addChild(horizontalLayout);

	mGLView = new Widget("GLView");
	layout->addChild(mGLView);

	mGLView->setProperty("width", -1);
	mGLView->setProperty("height", -1);

	Widget* toolBar = new Widget("HorizontalLayout");
	toolBar->setProperty("childVerticalAlignment", "center");
	toolBar->setProperty("width", "-1");
	toolBar->setProperty("height", EXTENT_Y(maGetScrSize()) / 10);
	layout->addChild(toolBar);

	toolBar->setProperty("leftMargin", "8");
	toolBar->setProperty("rightMargin", "8");
	toolBar->setProperty("topMargin", "8");
	toolBar->setProperty("bottomMargin", "8");

	mSettingsButton = new Widget("ImageButton");
	mSettingsButton->setProperty("image", R_SETTINGS_ICON);
	mSettingsButton->setProperty("width", "-2");
	mSettingsButton->setProperty("height", "-2");
	toolBar->addChild(mSettingsButton);

	grid = new Grid(0, 0, 0, 0);
}

void MainScreen::setExpression(const String& exp) {
	if (grid->setExpression(exp)) {
		mExpression = exp;
		mEditBox->setProperty("text", mExpression.c_str());
	}
}

void MainScreen::willAppear() {
	Screen::willAppear();
	Environment::getEnvironment().addIdleListener(this);
	Environment::getEnvironment().addPointerListener(this);
}

void MainScreen::hide() {
	Screen::hide();
	Environment::getEnvironment().removeIdleListener(this);
	Environment::getEnvironment().removePointerListener(this);
}

void MainScreen::idle() {
	if (!mDrawing) {
		return;
	}

	if (mRotating) {
		double rotationTime = 1.0;
		double time = (double) maGetMilliSecondCount() * 0.001 - mStartTime;

		if (time < rotationTime) {
			double t = time / rotationTime;
			double f = -1 / (t * 4.0 + 1) + 1;
			double rotation = mRotationStart + 0.2 * mRotationVelocity * f;
			grid->setRotation(rotation);
		} else {
			mRotating = false;
		}
	}

	grid->render();
	mGLView->setProperty("invalidate", "");
}

void MainScreen::pointerPressEvent(MAPoint2d p) {
	lastPoint = p;
	mRotating = false;
	mStartTime = (double) maGetMilliSecondCount() * 0.001;
	mRotationVelocity = 0.0;
	mRotationStart = grid->getRotation();
}

void MainScreen::pointerMoveEvent(MAPoint2d p) {
	int delta = p.x - lastPoint.x;
	lastPoint = p;

	double rotation = grid->getRotation();
	rotation += (double) delta * 3.14159 / EXTENT_X(maGetScrSize());
	grid->setRotation(rotation);
	mRotationVelocity += rotation - mRotationStart;
	mRotationStart = rotation;
}

void MainScreen::pointerReleaseEvent(MAPoint2d p) {
	pointerMoveEvent(p);

	double mstime = (double) maGetMilliSecondCount() * 0.001;
	double time = mstime - mStartTime;
	if (time == 0.0)
		return;
	mRotationVelocity /= time;
	mStartTime = mstime;
	mRotating = true;
}

void MainScreen::buttonClicked(UIItem* item) {
	if (item == mSettingsButton) {
		sStackScreen->pushScreen(sSettingsScreen);
	}
}

void MainScreen::editBoxReturned(UIItem* item) {
	if (item == mEditBox) {
		mExpression = mEditBox->getPropertyString("text");
		if (!grid->setExpression(mExpression)) {
			mEditBox->setProperty("placeholder", "invalid expression");
			mEditBox->setProperty("text", "");
		}
		mEditBox->setProperty("showKeyboard", "false");
	}
}

void MainScreen::glViewReady(UIItem* item) {
	if (item == mGLView) {
		mGLView->setProperty("bind", "");
		int glWidth = mGLView->getPropertyInt("width");
		int glHeight = mGLView->getPropertyInt("height");
		grid->initContext(glWidth, glHeight);
		grid->setExpression(mExpression);
		mDrawing = true;
	}
}

SettingsScreen::SettingsScreen() {
	setProperty("title", "Settings");

	mListView = new Widget("ListView");
	mListView->setProperty("width", "-1");
	mListView->setProperty("height", "-1");

	Widget* shadingItem = new Widget("ListViewItem");
	shadingItem->setProperty("text", "Shading enabled");

	Widget *horizontalLayout = new Widget("HorizontalLayout");
	horizontalLayout->setProperty("width", "-1");
	horizontalLayout->setProperty("height", "-1");
	horizontalLayout->setProperty("childVerticalAlignment", "center");

	Widget *padding = new Widget("RelativeLayout");
	padding->setProperty("width", "-1");
	padding->setProperty("height", "-1");
	horizontalLayout->addChild(padding);

	mShadingCheckBox = new Widget("CheckBox");
	mShadingCheckBox->setProperty("width", "-2");
	mShadingCheckBox->setProperty("height", "-2");
	mShadingCheckBox->setProperty("checked", "true");
	horizontalLayout->addChild(mShadingCheckBox);

	shadingItem->addChild(horizontalLayout);

	mListView->addChild(shadingItem);

	mSetResolutionItem = new Widget("ListViewItem");
	mSetResolutionItem->setProperty("text", "Grid resolution");
	mSetResolutionItem->setProperty("accessoryType", "hasChildren");
	mListView->addChild(mSetResolutionItem);

	Widget* helpItem = new Widget("ListViewItem");
	helpItem->setProperty("text", "Help");
	helpItem->setProperty("accessoryType", "hasChildren");
	mListView->addChild(helpItem);

	Widget* aboutItem = new Widget("ListViewItem");
	aboutItem->setProperty("text", "About");
	aboutItem->setProperty("accessoryType", "hasChildren");
	mListView->addChild(aboutItem);

	setMainWidget(mListView);
}

void SettingsScreen::checkboxStateChanged(UIItem* item, bool checked) {
	if (item == mShadingCheckBox) {
		sMainScreen->getGrid()->setShadingEnabled(checked);
	}

}

void SettingsScreen::listboxItemSelected(UIItem* item, int index) {
	if (item == mListView && index == 1) {
		Graphun::sStackScreen->pushScreen(Graphun::sResolutionSelector);
	} else if (item == mListView && index == 2) {
		Graphun::sStackScreen->pushScreen(Graphun::sHelpScreen);
	} else if (item == mListView && index == 3) {
		Graphun::sStackScreen->pushScreen(Graphun::sAboutScreen);
	}

}

class ResolutionSelector: public ListSelectionScreen {
public:

	ResolutionSelector() :
		ListSelectionScreen() {
		setProperty("title", "Resolution");
		Vector<String> resolutions;
		resolutions.add("low (8x8)");
		resolutions.add("medium (16x16)");
		resolutions.add("high (32x32)");
		addStrings(resolutions);
	}

	void listboxItemSelected(UIItem* item, int index) {

		if (item == mListView) {
			if (index == 0)
				sMainScreen->getGrid()->initGrid(8, 8);
			else if (index == 1)
				sMainScreen->getGrid()->initGrid(16, 16);
			else if (index == 2)
				sMainScreen->getGrid()->initGrid(32, 32);

			Graphun::sStackScreen->pop();
		}
	}
};
}

class GraphunApp: public Moblet {
public:
	GraphunApp() {
		Graphun::sStackScreen = new StackScreen();
		Graphun::sMainScreen = new Graphun::MainScreen();
		Graphun::sSettingsScreen = new Graphun::SettingsScreen();
		Graphun::sResolutionSelector = new Graphun::ResolutionSelector();

		Graphun::sHelpScreen = new Graphun::HelpScreen();
		Graphun::sAboutScreen = new Graphun::TextScreen("About",
				Graphun::sAboutText, R_GRAPHUN_LOGO);

		Graphun::sStackScreen->pushScreen(Graphun::sMainScreen);
		Graphun::sStackScreen->show();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if (keyCode == MAK_BACK) {
			if (1 == Graphun::sStackScreen->stackSize()) {
				maExit(0);
			}
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}

private:
};

extern "C" int MAMain() {
	Moblet::run(new GraphunApp());
	return 0;
}


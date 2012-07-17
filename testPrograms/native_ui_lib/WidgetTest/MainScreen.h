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

/**
 * @file MainScreen.h
 * @author Bogdan Iusco.
 * @date 17 Jan 2012
 *
 * Application's screen.
 * Displays widgets for setting & getting widget values.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Application's screen.
 * Displays widgets for setting & getting widget values.
 */
class MainScreen:
	public Screen,
	public ButtonListener,
	public SliderListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	virtual void buttonClicked(Widget* button);

	/**
	 * This method is called when the value of the slider was modified by
	 * the user.
	 * @param slider The slider object that generated the event.
	 * @param sliderValue The new slider's value.
	 */
	virtual void sliderValueChanged(
		Slider* slider,
		const int sliderValue);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * Button on which the widget values will be set.
	 */
	Button* mDemoWidget;

	/**
	 * Used for setting demo widget alpha value.
	 */
	Slider* mAlphaValueSlider;

	/**
	 * Display demo's button alpha value.
	 */
	Label* mAlphaLabel;

	/**
	 * Get the alpha value.
	 */
	Button* mGetAlphaButton;
};


#endif /* MAINSCREEN_H_ */

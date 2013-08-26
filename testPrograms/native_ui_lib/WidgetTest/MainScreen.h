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

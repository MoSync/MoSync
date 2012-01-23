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
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"

#include "MAHeaders.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mImage(NULL),
	mGetImageAlpha(NULL),
	mImageAlpha(NULL),
	mImageSlider(NULL),
	mLayout(NULL),
	mGetLayoutAlpha(NULL),
	mLayoutAlpha(NULL),
	mLayoutSlider(NULL)
{
	createMainLayout();
	mGetImageAlpha->addButtonListener(this);
	mGetLayoutAlpha->addButtonListener(this);
	mImageSlider->addSliderListener(this);
	mLayoutSlider->addSliderListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mGetImageAlpha->removeButtonListener(this);
	mGetLayoutAlpha->removeButtonListener(this);
	mImageSlider->removeSliderListener(this);
	mLayoutSlider->removeSliderListener(this);
}

/**
 * This method is called when there is an event for this widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
void MainScreen::handleWidgetEvent(Widget* widget,
		MAWidgetEventData* widgetEventData)
{

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mImage = new Image();
	mMainLayout->addChild(mImage);

	int result = mImage->setImage(RES_IMAGE);
	printf("result setImage = %d", result);
	mImage->fillSpaceHorizontally();
	mImage->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);

	Label* infoImage = new Label();
	infoImage->setText("Set the image transparency");
	mMainLayout->addChild(infoImage);

	// Slider with alpha value, button for getting the value and
	// label for printing it.
	mImageSlider = new Slider();
	mImageSlider->fillSpaceHorizontally();
	mImageSlider->setMaximumValue(100);
	mImageSlider->setValue(100);
	mMainLayout->addChild(mImageSlider);

	mGetImageAlpha = new Button();
	mGetImageAlpha->setText("Get Image alpha");
	mMainLayout->addChild(mGetImageAlpha);

	mImageAlpha = new Label();
	mImageAlpha->setText("The image alpha = ");
	mMainLayout->addChild(mImageAlpha);

	// Spacer
	Layout* spacer = new VerticalLayout();
	spacer->fillSpaceHorizontally();
	spacer->setHeight(40);
	mMainLayout->addChild(spacer);

	Label* infoLayout = new Label();
	infoLayout->setText("Set the blue layout transparency");
	mMainLayout->addChild(infoLayout);

	mLayoutSlider = new Slider();
	mLayoutSlider->fillSpaceHorizontally();
	mLayoutSlider->setMaximumValue(100);
	mLayoutSlider->setValue(0);
	mMainLayout->addChild(mLayoutSlider);

	mGetLayoutAlpha = new Button();
	mGetLayoutAlpha->setText("Get layout alpha");
	mMainLayout->addChild(mGetLayoutAlpha);

	mLayoutAlpha = new Label();
	mLayoutAlpha->setText("The layout alpha = ");
	mMainLayout->addChild(mLayoutAlpha);

	mLayout = new VerticalLayout();
	mLayout->setBackgroundColor(0x0000FF);
	mLayout->fillSpaceHorizontally();
	mLayout->setAlpha(0.0);
	mLayout->setHeight(50);
	mMainLayout->addChild(mLayout);
}


/**
 * This method is called when the value of the slider was modified by
 * the user.
 * @param slider The slider object that generated the event.
 * @param sliderValue The new slider's value.
 */
void MainScreen::sliderValueChanged(
	Slider* slider,
	const int sliderValue)
{
	float alphaValue = (float) sliderValue / 100;
	if ( slider == mImageSlider )
	{
		mImage->setAlpha(alphaValue);
	}
	else if ( slider == mLayoutSlider )
	{
		mLayout->setAlpha(alphaValue);
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if ( mGetImageAlpha == button )
	{
		float alphaValue = mImage->getAlpha();
		char buf[MAX_BUF_SIZE];
		sprintf(buf, "Image Alpha: %f",alphaValue);
		mImageAlpha->setText(buf);
	}
	else if ( mGetLayoutAlpha == button )
	{
		float alphaValue = mLayout->getAlpha();
		char buf[MAX_BUF_SIZE];
		sprintf(buf, "Layout Alpha: %f",alphaValue);
		mLayoutAlpha->setText(buf);
	}
}

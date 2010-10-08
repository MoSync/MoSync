/* Copyright (C) 2010 MoSync AB

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

/**
* \file Button.h
* \brief Button widget
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_SLIDER_H_
#define _SE_MSAB_MAUI_SLIDER_H_

#include "Style.h"
#include "Widget.h"
#include <MAUtil/String.h>
#include <MAUtil/ListenerSet.h>

namespace MAUI {

	class Slider;

	class SliderListener {
	public:
		virtual void onValueChange(Slider* slider, double value) = 0;
	};

	class Slider : public Widget {
	public:
		enum Orientation {
			VERTICAL,
			HORIZONTAL
		};

		Slider(int x=0, int y=0, int width=0, int height=0,
			Orientation ori=HORIZONTAL, double minValue=0.0, double maxValue=1.0,
			double defaultValue=0.0);
		bool pointerPressed(MAPoint2d p, int id);
		bool pointerMoved(MAPoint2d p, int id);
		bool pointerReleased(MAPoint2d p, int id);
		virtual bool isTransparent() const;
		void setFocused(bool focused=true);

		void setMinValue(double min);
		void setMaxValue(double max);
		void setValue(double val);
		double getMinValue() const;
		double getMaxValue() const;
		double getValue() const;

		void addSliderListener(SliderListener* sl);
		void removeSliderListener(SliderListener* sl);

		void update();

		// \param 'moveToPoint' If 'true', when slider pressed, grip will move to the point that was pressed. Otherwise the slider can only be moved by touching the grip.
		void setMoveToPoint(bool moveToPoint);

	protected:
		int getSliderPos() const;
		void restyle();
		void drawWidget();

		//String mCaption;
		bool mPressed;
		Orientation mOrientation;
		double mMinValue, mMaxValue, mValue;
		int mStartX, mStartY;
		double mStartValue;
		WidgetSkin *mAmountSkin;
		WidgetSkin *mBkgSkin;
		MAHandle mGripImage;
		int mSliderWeight;
		int mSliderGripWidth;
		int mSliderGripHeight;
		bool mValueChanged;
		bool mMoveToPoint;

		ListenerSet<SliderListener> mSliderListeners;
	};

	/**
	 * Available properties:
	 * paddingLeft = IntegerProperty
	 * paddingRight = IntegerProperty
	 * paddingTop = IntegerProperty
	 * paddingBottom = IntegerProperty
	 * marginLeft = IntegerProperty
	 * marginRight = IntegerProperty
	 * marginTop = IntegerProperty
	 * marginBottom = IntegerProperty
	 * backgroundSkinFocused = SkinProperty
	 * backgroundSkinUnfocused = SkinProperty
	 * sliderSkin = SkinProperty
	 * sliderAmountSkin = SkinProperty
	 * gripImage = ImageProperty
	 */
	class SliderStyle : public Style {
	public:
		SliderStyle(SkinProperty* slider_amt, SkinProperty* slider_bkg, ImageProperty* grip);

	};

}

#endif /* _SE_MSAB_MAUI_BUTTON_H_ */


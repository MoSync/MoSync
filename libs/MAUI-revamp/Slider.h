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
* \file Slider.h
* \brief Slider widget.
* \author Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_SLIDER_H_
#define _SE_MSAB_MAUI_SLIDER_H_

#include "Style.h"
#include "Widget.h"
#include <MAUtil/String.h>
#include <MAUtil/ListenerSet.h>

namespace MAUI {

	class Slider;

	/**
	 * \brief Listener class for slider events.
	 */
	class SliderListener {
	public:
		/**
		 * Event method called when the slider is moved.
		 * \param slider The slider widget.
		 * \param value The slider handle position. The range of
		 * the handle position is given in the Slider constructor,
		 * or set by methods setMinValue and setMaxValue.
		 */
		virtual void onValueChange(Slider* slider, double value) = 0;
	};

	/**
	 * \brief Slider widget.
	 */
	class Slider : public Widget {
	public:
		/**
		 * \brief Slider orientation type.
		 */
		enum Orientation {
			VERTICAL,
			HORIZONTAL
		};

		/**
		 * Constructor.
		 * \param x Left coordinate of the slider.
		 * \param y Top coordinate of the slider.
		 * \param width Width of the slider.
		 * \param height Height of the slider.
		 * \param orientation Orientation of the slider.
		 * \param minValue The minimum value of the slider handle position.
		 * \param maxValue The maximum value of the slider handle position.
		 * \param defaultValue The start position of the slider handle.
		 */
		Slider(
			int x=0, 
			int y=0, 
			int width=0, 
			int height=0,
			Orientation orientation=HORIZONTAL, 
			double minValue=0.0, 
			double maxValue=1.0,
			double defaultValue=0.0);
		
		/**
		 * The pointer is pressed.
		 * \param point The pointer coordinate.
		 * \param id TODO: Document this param.
		 * \return TODO: Document return value.
		 */
		bool pointerPressed(MAPoint2d point, int id);
		
		/**
		 * The pointer is moved.
		 * \param point The pointer coordinate.
		 * \param id TODO: Document this param.
		 * \return TODO: Document return value.
		 */
		bool pointerMoved(MAPoint2d point, int id);
		
		/**
		 * The pointer is released.
		 * \param point The pointer coordinate.
		 * \param id TODO: Document this param.
		 * \return TODO: Document return value.
		 */
		bool pointerReleased(MAPoint2d p, int id);
		
		/**
		 * Answer if the slider is transparent.
		 * \return true if transparent, false if not.
		 */
		virtual bool isTransparent() const;
		
		/**
		 * Set if this widget is focused.
		 * \param focused true if focused, false is not.
		 */
		void setFocused(bool focused=true);

		/**
		 * Set the value that represents the minimum slider handle position.
		 * \param min The value representing the minimum position.
		 */
		void setMinValue(double min);
		
		/**
		 * Set the value that represents the maximum slider handle position.
		 * \param max The value representing the maximum position.
		 */
		void setMaxValue(double max);
		
		/**
		 * Set the slider handle position.
		 * \param value The value of the slider handle position.
		 */
		void setValue(double value);
		
		/**
		 * Get the value that represents the minimum slider handle position.
		 * \return The value representing the minimum position.
		 */
		double getMinValue() const;
		
		/**
		 * Get the value that represents the maximum slider handle position.
		 * \return The value representing the maximum position.
		 */
		double getMaxValue() const;
		
		/**
		 * Get the slider handle position.
		 * \return The value of the slider handle position.
		 */
		double getValue() const;

		/**
		 * Add a listener to this slider.
		 * \param listener The listener to be added.
		 */
		void addSliderListener(SliderListener* listener);
		
		/**
		 * Remove a listener from this slider.
		 * \param listener The listener to be removed.
		 */
		void removeSliderListener(SliderListener* listener);

		/**
		 * Set how the user should be able to move the slider handle.
		 *
		 * \param moveToPoint If true, when slider pressed, 
		 * handle will move to the point that was pressed. If false,
		 * the slider can only be moved by touching the handle.
		 */
		void setMoveToPoint(bool moveToPoint);

	protected:
		/**
		 * TODO: Document.
		 */
		virtual void updateInternal();

		/**
		 * TODO: Document.
		 */
		int getSliderPos() const;
		
		/**
		 * TODO: Document.
		 */
		void restyle();
		
		/**
		 * TODO: Document.
		 */
		void drawWidget();

		// TODO: Document why this line is commented out or remove it.
		//String mCaption;
		
		/**
		 * The pointer pressed state.
		 */
		bool mPressed;
		
		/**
		 * Orientation of the slider.
		 */
		Orientation mOrientation;
		
		/**
		 * Minimum value of the slider.
		 */
		double mMinValue;
		
		/**
		 * Maximum value of the slider.
		 */
		double mMaxValue;
		
		/**
		 * Current value of the slider (the handle position).
		 */
		double mValue;
		
		/**
		 * TODO: Document.
		 */
		int mStartX;
		
		/**
		 * TODO: Document.
		 */
		int mStartY;
		
		/**
		 * TODO: Document.
		 */
		double mStartValue;
		
		/**
		 * TODO: Document.
		 */
		Drawable *mAmountSkin;
		
		/**
		 * TODO: Document.
		 */
		Drawable *mBkgSkin;
		
		/**
		 * Image for the grip handle that represents the slider position.
		 */
		MAHandle mGripImage;
		
		/**
		 * TODO: Document.
		 */
		int mSliderWeight;
		
		/**
		 * Width of grip image.
		 */
		int mSliderGripWidth;
		
		/**
		 * Height of grip image.
		 */
		int mSliderGripHeight;
		
		/**
		 * TODO: Document.
		 */
		bool mValueChanged;
		
		/**
		 * Determines if the used can move the grip handle by clicking
		 * anywhere on the slider, or only by dragging it.
		 */
		bool mMoveToPoint;

		/**
		 * Listeners for this slider.
		 */
		ListenerSet<SliderListener> mSliderListeners;
	};

	/**
	 * \brief Style object for sliders.
	 *
	 * Available properties:
	 * paddingLeft = IntegerProperty
	 * paddingRight = IntegerProperty
	 * paddingTop = IntegerProperty
	 * paddingBottom = IntegerProperty
	 * marginLeft = IntegerProperty
	 * marginRight = IntegerProperty
	 * marginTop = IntegerProperty
	 * marginBottom = IntegerProperty
	 * backgroundSkinFocused = DrawableProperty
	 * backgroundSkinUnfocused = DrawableProperty
	 * sliderSkin = DrawableProperty
	 * sliderAmountSkin = DrawableProperty
	 * gripImage = ImageProperty
	 */
	class SliderStyle : public Style {
	public:
		/**
		 * Constructor.
		 * \param slider_amt TODO: Rename to meaningful name and document.
		 * \param slider_bkg TODO: Rename to meaningful name and document.
		 * \param grip TODO: Document.
		 */
		SliderStyle(
			DrawableProperty* slider_amt, 
			DrawableProperty* slider_bkg, 
			ImageProperty* grip);
	};
}

#endif /* _SE_MSAB_MAUI_SLIDER_H_ */

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

/** 
* \file Layout.h 
* \brief Layout and navigation of child widgets in a grid
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_STACK_LAYOUT_H_
#define _SE_MSAB_MAUI_STACK_LAYOUT_H_

#include "Widget.h"

namespace MAUI {

	/** \brief Layout and navigation of child widgets in a grid.
	   *
	   * This widget is responsible for laying out its children in a grid.
	   **/

	class StackLayout : public Widget, protected WidgetListener {
	public:
		// Enum to describe stack layout alignment
		// SLA_TOP_LEFT means alignment==TOP if the stack layout is VERTICAL and LEFT if it's HORIZONTAL etc.
		// Autosize means it should autosize the widgets in the oposite direction of the stack layuout orientation
		enum Alignment {
			SLA_TOP_LEFT,
			SLA_CENTER,
			SLA_BOTTOM_RIGHT,
			SLA_AUTO_SIZE
		};

		enum Orientation {
			SLO_VERTICAL,
			SLO_HORIZONTAL
		};

		/** Constructor.
		  * \param gridXSize the number of columns in the grid.
		  * \param gridYSize the number of rows in the grid.
		  **/
		StackLayout(int x, int y, int width, int height, Orientation ori=SLO_VERTICAL, Alignment ali=SLA_AUTO_SIZE);

		/** Overloaded implementation of Widget::add(), with the same external semantics.
		    **/
		void add(Widget *w);

		/** Overloaded implementation of Widget::clear(), with the same external semantics.
		    **/
		void clear();

		/** Overloaded implementation of drawWidget().
		   **/
		void drawWidget();

		/**  Sets the x margin of each widget relative to the grid cell it resides in. 
		    **/
		void setMarginX(int p);

		/**  Sets the y margin of each widget relative to the grid cell it resides in. 
		    **/
		void setMarginY(int p);

		/** Sets the horizontal alignment of the child widget within their grid cells.
		    **/	
		void setAlignment(Alignment alignment);

		void setOrientation(Orientation orienation);

		/** Sets the position of the widget, relative to the parent **/
		void setPosition(int x, int y);

		/** Sets the width of the widget  **/
		void setWidth(int width);

		/** Sets the height of the widget  **/
		void setHeight(int height);

		virtual bool isTransparent() const;

	protected:
		virtual void updateInternal();
		void rebuild();

//		bool mMustRebuild;
		Orientation mOrientation;
		Alignment mAlignment;

		int mMarginX;
		int mMarginY;
	
		//WidgetListener
		void boundsChanged(Widget *widget, const Rect& bounds);

		void focusChanged(Widget *widget, bool focused);

	};

}

#endif /* _SE_MSAB_MAUI_LAYOUT_H_ */

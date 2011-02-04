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
* \file StackLayout.h 
* \brief Stack layout of child widgets.
* \author Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_STACK_LAYOUT_H_
#define _SE_MSAB_MAUI_STACK_LAYOUT_H_

#include "Widget.h"

namespace MAUI {

	/** 
	 * \brief Widget that layouts childred in a stack.
	 */
	class StackLayout : public Widget, protected WidgetListener {
	public:
		/**
		 * Enum to describe stack layout alignment.
		 * SLA_TOP_LEFT means alignment==TOP if the stack layout is VERTICAL 
		 * and LEFT if it's HORIZONTAL etc.
		 *
		 * SLA_AUTO_SIZE means it should autosize the widgets in the oposite 
		 * direction of the stack layuout orientation.
		 *
		 * TODO: The SLA_ prefix is hard to understand, rename it to
		 * STACK_LAYOUT_
		 */
		enum Alignment {
			SLA_TOP_LEFT,
			SLA_CENTER,
			SLA_BOTTOM_RIGHT,
			SLA_AUTO_SIZE
		};

		/**
		 * Enum to describe stack layout orientation.
		 *
		 * TODO: The SLO_ prefix is hard to understand, rename it to
		 * STACK_LAYOUT_ or STACK_ORIENTATION_
		 */
		enum Orientation {
			SLO_VERTICAL,
			SLO_HORIZONTAL
		};

		/** 
		 * Constructor.
		 * \param gridXSize the number of columns in the grid.
		 * \param gridYSize the number of rows in the grid.
		 */
		StackLayout(
			int x=0, 
			int y=0, 
			int width=0, 
			int height=0, 
			Orientation orientation=SLO_VERTICAL, 
			Alignment alignment=SLA_AUTO_SIZE);

		/**
		 * Add a child widget.
		 * Overloaded implementation of Widget::add(), 
		 * with the same external semantics.
		 * \param widget Widget to add.
		 */
		virtual void add(Widget* widget);
		
		/**
		 * Remove a child widget.
		 * \param widget Widget to remove.
		 */
		virtual void remove(Widget* widget);

		/** 
		 * Remove all child widgets.
		 * Overloaded implementation of Widget::clear(), 
		 * with the same external semantics.
		 */
		void clear();

		/**
		 * TODO: Improve documentation of this method.
		 * Overloaded implementation of drawWidget().
		 */
		void drawWidget();

		/**
		 * TODO: Improve documentation of this method.
		 * Set the x margin. 
		 * \param marginX The x margin.
		 */
		void setMarginX(int marginX);

		/**
		 * TODO: Improve documentation of this method.
		 * Set the y margin. 
		 * \param marginY The y margin.
		 */
		void setMarginY(int marginY);

		/**
		 * TODO: Improve documentation of this method.
		 * Set the alignment. 
		 * \param alignment The alignment.
		 */
		void setAlignment(Alignment alignment);

		/**
		 * TODO: Improve documentation of this method.
		 * Set the orientation. 
		 * \param orientation The orientation.
		 */
		void setOrientation(Orientation orienation);

		/** 
		 * TODO: Verify the documentation of this method.
		 * Set the position of the widget, relative to the parent. 
		 * \param x The left coordinte.
		 * \param y The top coordinte.
		 */
		void setPosition(int x, int y);

		/** 
		 * Set the width of the widget.
		 * \param width The width.
		 */
		void setWidth(int width);

		/** 
		 * Set the height of the widget.
		 * \param height The height.
		 */
		void setHeight(int height);

		/** 
		 * Get the transparency of the widget.
		 * \return true if transparent, false if not.
		 */
		virtual bool isTransparent() const;

	protected:
		/**
		 * TODO: Document.
		 */
		virtual void updateInternal();
		
		/**
		 * TODO: Document.
		 */
		void rebuild();

//		bool mMustRebuild;

		/**
		 * TODO: Document.
		 */
		Orientation mOrientation;
		
		/**
		 * TODO: Document.
		 */
		Alignment mAlignment;

		/**
		 * TODO: Document.
		 */
		int mMarginX;
		
		/**
		 * TODO: Document.
		 */
		int mMarginY;
		
		/**
		 * TODO: Improve documentation.
		 * Callback method in WidgetListener.
		 * \param widget The widget.
		 * \param bounds TODO: Document.
		 */
		void boundsChanged(Widget *widget, const Rect& bounds);

		/**
		 * TODO: Document.
		 * \param widget The widget.
		 * \param focused TODO: Document.
		 */
		void focusChanged(Widget *widget, bool focused);
	};
}

#endif /* _SE_MSAB_MAUI_STACK_LAYOUT_H_ */

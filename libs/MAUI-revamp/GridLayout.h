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
* \file GridLayout.h 
* \brief Layout and navigation of child widgets in a grid.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_GRID_LAYOUT_H_
#define _SE_MSAB_MAUI_GRID_LAYOUT_H_

#include "Widget.h"

namespace MAUI {
	/** 
	 * \brief Layout and navigation of child widgets in a grid.
	 *
	 * This widget is responsible for laying out its children in a grid.
	 */
	class GridLayout : public Widget, protected WidgetListener {
	public:
		/**
		 * Constants for horizontal alignment.
		 */
		enum HorizontalAlignment {
			HA_LEFT,
			HA_CENTER,
			HA_RIGHT
		};

		/**
		 * Constants for vertical alignment.
		 */
		enum VerticalAlignment {
			VA_TOP,
			VA_CENTER,
			VA_BOTTOM
		};

		/** Constructor.
		 * \param x Left coordinate of the grid component.
		 * \param y Top coordinate of the grid component.
		 * \param width Width of the grid component.
		 * \param height Height of the grid component.
		 * \param gridXSize The number of columns in the grid.
		 * \param gridYSize The number of rows in the grid.
		 */
		GridLayout(
			int x=0, 
			int y=0, 
			int width=0, 
			int height=0, 
			int gridXSize=2, 
			int gridYSize=2);

		/** 
		 * Adds a child widget to the grid layout.
		 * Overloaded implementation of Widget::add(), 
		 * with the same external semantics.
		 * \param child The child widget to add.
		 */
		virtual void add(Widget* child);
		
		/** 
		 * Removes a child widget to the grid layout.
		 * \param child The child widget to remove.
		 */
		virtual void remove(Widget* child);

		/** 
		 * Removes all child widgets.
		 * Overloaded implementation of Widget::clear(), 
		 * with the same external semantics.
		 */
		void clear();

		/** 
		 * Render the widget.
		 * Overloaded implementation of drawWidget().
		 */
		void drawWidget();

		/**
		 * Sets the x margin of each child widget relative to the 
		 * grid cell it resides in. 
		 * \param marginX The horizontal margin.
		 */
		void setMarginX(int marginX);

		/**
		 * Sets the y margin of each child widget relative to the 
		 * grid cell it resides in. 
		 * \param marginY The vertical margin.
		 */
		void setMarginY(int marginY);

		/** 
		 * TODO: Does this set the alignment for all child widgets?
		 * Sets the horizontal alignment of the child widget within 
		 * their grid cells.
		 * \param The horizontal alignment.
		 */
		void setHorizontalAlignment(HorizontalAlignment alignment);

		/** 
		 * TODO: Does this set the alignment for all child widgets?
		 * Sets the vertical alignment of the child widget within 
		 * their grid cells.
		 * \param The vertical alignment.
		 */
		void setVerticalAlignment(VerticalAlignment alignment);

		/** 
		 * Sets the horizontal autosize property, determining wether 
		 * the grid columns are resized so that the widest of its 
		 * children fits.
		 * \param autosize If true autosize is on, if false off.
		 */
		void setAutoSizeX(bool autosize = true);

		/** 
		 * Sets the vertical autosize property, determining wether 
		 * the grid rows are resized so that the widest of its 
		 * children fits.
		 * \param autosize If true autosize is on, if false off.
		 */
		void setAutoSizeY(bool autosize = true);

		/** 
		 * Sets the number of columns in the grid.
		 * \param numColumns The number of columns.
		 */
		void setNumColumns(int numColumns);

		/** 
		 * Sets the number of rows in the grid.
		 * \param numRows The number of rows.
		 */
		void setNumRows(int numRows);

		/** 
		 * Sets the position of the widget, relative to the parent.
		 * \param x The left corrdinate of the widget.
		 * \param y The top corrdinate of the widget.
		 */
		void setPosition(int x, int y);

		/** 
		 * Sets the width of the widget. 
		 * \param width The width of the widget.
		 */
		void setWidth(int width);

		/** 
		 * Sets the height of the widget. 
		 * \param height The height of the widget.
		 */
		void setHeight(int height);

		/** 
		 * Returns the transparency of the widget.
		 * \return true if transparent, false if opaque.
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

		/** 
		 * TODO: Document.
		 */
		bool mMustRebuild;
		
		/** 
		 * Horizontal alignment type.
		 */
		HorizontalAlignment mAlignmentX;
		
		/** 
		 * Vertical alignment type.
		 */
		VerticalAlignment mAlignmentY;
		
		/** 
		 * Horizontal margin for grid cells.
		 */
		int mMarginX;
		
		/** 
		 * Vertical margin for grid cells.
		 */
		int mMarginY;
		
		/** 
		 * Flag that controls horizontal autosize of grid cells.
		 */
		bool mAutoSizeX;
		
		/** 
		 * Flag that controls vertical autosize of grid cells.
		 */
		bool mAutoSizeY;
		
		/** 
		 * Number of grid columns.
		 */
		int mGridXSize;
		
		/** 
		 * Number of grid rows.
		 */
		int mGridYSize;

		/**
		 * TODO: Document. 
		 * Implementation of WidgetListener method?
		 * \param widget Widget that has changed its position/size.
		 * \param bounds The new position/size.
		 */
		void boundsChanged(Widget *widget, const Rect& bounds);

		/**
		 * TODO: Document. 
		 * Implementation of WidgetListener method?
		 * \param widget Widget that has changed its focus.
		 * \param focused True if gaining focus, false if losing focus.
		 */
		void focusChanged(Widget *widget, bool focused);
	};
}

#endif /* _SE_MSAB_MAUI_LAYOUT_H_ */

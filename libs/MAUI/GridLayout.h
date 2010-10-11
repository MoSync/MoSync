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

#ifndef _SE_MSAB_MAUI_GRID_LAYOUT_H_
#define _SE_MSAB_MAUI_GRID_LAYOUT_H_

#include "Widget.h"

namespace MAUI {

	/** \brief Layout and navigation of child widgets in a grid.
	   *
	   * This widget is responsible for laying out its children in a grid.
	   **/

	class GridLayout : public Widget, protected WidgetListener {
	public:
		enum HorizontalAlignment {
			HA_LEFT,
			HA_CENTER,
			HA_RIGHT
		};

		enum VerticalAlignment {
			VA_TOP,
			VA_CENTER,
			VA_BOTTOM
		};

		/** Constructor.
		  * \param gridXSize the number of columns in the grid.
		  * \param gridYSize the number of rows in the grid.
		  **/
		GridLayout(int x, int y, int width, int height, int gridXSize, int gridYSize);

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
		void setHorizontalAlignment(HorizontalAlignment alignment);

		/** Sets the vertical alignment of the child widget within their grid cells.
		    **/	
		void setVerticalAlignment(VerticalAlignment alignment);

		/** Sets the horizontal autosize property, determining wether the grid columns
		    * are resized so that the widest of its children fits.
		    **/	
		void setAutoSizeX(bool f = true);

		/** Sets the vertical autosize property, determining wether the grid rows
		    * are resized so that the widest of its children fits.
		    **/	
		void setAutoSizeY(bool f = true);

		/** Sets the number of columns in the grid.
		    **/
		void setNumColumns(int numColumns);

		/** Sets the number of rows in the grid.
		    **/
		void setNumRows(int numRows);

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

		bool mMustRebuild;
		HorizontalAlignment mAlignmentX;
		VerticalAlignment mAlignmentY;
		int mMarginX;
		int mMarginY;
		bool mAutoSizeX;
		bool mAutoSizeY;
		int mGridXSize;
		int mGridYSize;

		//WidgetListener
		void boundsChanged(Widget *widget, const Rect& bounds);

		void focusChanged(Widget *widget, bool focused);

	};

}

#endif /* _SE_MSAB_MAUI_LAYOUT_H_ */

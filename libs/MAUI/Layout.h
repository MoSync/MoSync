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
* \file Layout.h 
* \brief Layout and navigation of child widgets in a grid
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_LAYOUT_H_
#define _SE_MSAB_MAUI_LAYOUT_H_

#include "Widget.h"

namespace MAUI {

	/** \brief Layout and navigation of child widgets in a grid.
	   *
	   * This widget is responsible for laying out its children in a grid. It also provides 
	   * navigation facilites within that grid, so that the children can be selected and triggered.
	   * The widget doesn't draw its background by default, use setDrawBackground(true) to enable it.
	   **/

	class Layout : public Widget, protected WidgetListener {
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
		  * \param x the horizontal position of the Layout relative to its parent's top left padded corner.
		  * \param y the vertical position of the Layout relative to its parent's top left padded corner
		  * \param width the width of the Layout.
		  * \param height the height of the Layout.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Layout to be added to the parent's children.
		  **/
		Layout(int x, int y, int width, int height, Widget* parent=NULL);
		/** Constructor.
		  * \param x the horizontal position of the Label relative to its parent's top left padded corner.
		  * \param y the vertical position of the Label relative to its parent's top left padded corner
		  * \param width the width of the Label.
		  * \param height the height of the Label.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Label to be added to the parent's children.
		  * \param gridXSize the number of columns in the grid.
		  * \param gridYSize the number of rows in the grid.
		  **/
		Layout(int x, int y, int width, int height, Widget* parent, int gridXSize, int gridYSize);

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
		/** Navigate to the cell above the currently selected one **/
		void goUp();
		/** Navigate to the cell below the currently selected one **/
		void goDown();
		/** Navigate to the cell to the right of the currently selected one **/
		void goRight();
		/** Navigate to the cell to the left the currently selected one **/
		void goLeft();
		/** Cause the currently selected cell's widget to be triggered  **/
		void trigger();

		virtual void update();

	protected:
		bool mustRebuild;
		void rebuild();

		HorizontalAlignment alignmentX;
		VerticalAlignment alignmentY;
		int marginX;
		int marginY;
		bool autoSizeX;
		bool autoSizeY;
		int gridXSize;
		int gridYSize;
		int selectedIndex;

		//WidgetListener
		void boundsChanged(Widget *widget, const Rect& bounds);
	};

}

#endif /* _SE_MSAB_MAUI_LAYOUT_H_ */

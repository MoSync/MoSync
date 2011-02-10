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
 * \file ListBox.h 
 * \brief List box widget, horizontal or vertical layout.
 * \author Patrick Broman and Niklas Nummelin
 */

#ifndef _SE_MSAB_MAUI_LISTBOX_H_
#define _SE_MSAB_MAUI_LISTBOX_H_

#include "Widget.h"
#include "Engine.h"
#include <MAUtil/Environment.h>
#include "TouchMotionTracker.h"

namespace MAUI {
	class ListBox;

	/** 
	 * \brief Listener interface that receives notifications 
	 * when an item in a ListBox is selected.
	 */
	class ItemSelectedListener {
	public:
		/** 
		 * This function is called whenever the selected item 
		 * in a ListBox changes.
		 * \param sender The ListBox that triggered the event.
		 * \param selectedWidget The widget selected.
		 * \param unselectedWidget The previously selected widget.
		 * TODO: Can unselectedWidget be NULL if no item was
		 * previously selected?
		 */
		virtual void itemSelected(
			ListBox *sender, 
			Widget *selectedWidget, 
			Widget *unselectedWidget) = 0;
		
		/** 
		 * This function is called whenever the selection is blocked, 
		 * due to bounds. 
		 * \param sender The ListBox that triggered the event.
		 * \param direction The selection direction that caused the
		 * ListBox to hit its bounds. -1 when selectPreviousItem was 
		 * called and 1 when selectedNextItem was called. 
		 */
		virtual void blocked(ListBox *sender, int direction) = 0;
	};

	
	/** 
	 * \brief List box widget for horizontal or vertical lists.
	 *
	 * The MAUI ListBox is a very generic and flexible container that 
	 * enables laying out widgets in vertical or horizontal lists. 
	 * Unlike list boxes you might find in other UI toolkits, this one
	 * doesn't only support text-based items - it supports putting in any 
	 * type of widget. You could add an image followed by a label,
	 * followed by another, nested ListBox. Furthermore, the navigation 
	 * is decoupled from input. For instance, it provides functions such 
	 * as selectNextItem() rather than assuming that the way of doing this 
	 * should always be done by pressing the "down" key. The widget draws 
	 * its background by default, use setDrawBackground(false) to disable it.
	 */
	class ListBox : 
		public Widget, 
		protected MAUtil::TimerListener, 
		protected WidgetListener {
	public:
		/**
		 * Orientation type.
		 */
		enum ListBoxOrientation {
			LBO_HORIZONTAL,
			LBO_VERTICAL
		};

		/**
		 * Animation type.
		 */
		enum ListBoxAnimationType {
			LBA_NONE,
			LBA_LINEAR
		};

		/** 
		 * Constructor.
		 * \param x Left coordinate of ListBox.
		 * \param x Top coordinate of ListBox.
		 * \param width Width coordinate of ListBox.
		 * \param height Height coordinate of ListBox.
		 * \param orientation Controls the orientation of the ListBox.
		 * \param animType Controls the type of animation used when 
		 * scrolling the ListBox content.
		 * \param wrapping Controls the behavior of the ListBox when 
		 * navigating. When set to true (default), the ListBox will 
		 * wrap around to selecting the first element when moving beyond 
		 * the last one, and equivalently wrap around to the last one when 
		 * moving back beyond the first one.
		 */
		ListBox(
			int x=0, 
			int y=0, 
			int width=0, 
			int height=0,
			ListBoxOrientation orientation=LBO_VERTICAL, 
			ListBoxAnimationType animType=LBA_NONE,
			bool wrapping=true
		);

		/**
		 * Destructor. 
		 */
		virtual ~ListBox();

		/**
		 * Checks if this ListBox is transparent.
		 * \return true if transparent, false if not.
		 */
		virtual bool isTransparent() const;

		/** 
		 * Adds a widget as a child of the ListBox. 
		 * \param w The widget to add.
		 */
		virtual void add(Widget *w);
		
		/** 
		 * Removes a child widget from the ListBox. 
		 * \param w The child widget to remove.
		 */
		virtual void remove(Widget *w);

		/**
		 * Overloaded implementation of Widget::clear(), with the same 
		 * external semantics.
		 */
		void clear();

		// Renders the ListBox.
		// TODO: Why is this commented out?
		//void draw(bool forceDraw=false);

		/**
		 * Navigates to the next item in the ListBox - down if 
		 * the orientation is vertical, right if it's horizontal. 
		 * \param shouldFireListeners If true listeners are informed.
		 */
		void selectNextItem(bool shouldFireListeners=true);
		
		/** 
		 * Navigates to the previous item in the ListBox - up if 
		 * the orientation is vertical, left if it's horizontal. 
		 * \param shouldFireListeners If true listeners are informed.
		 */
		void selectPreviousItem(bool shouldFireListeners=true);
		
		/**
		 * Adds an item selection listener.
		 * \param listener Listener to add.
		 */
		void addItemSelectedListener(ItemSelectedListener *listener);
		
		/**
		 * Removes an item selection listener.
		 * \param listener Listener to remove.
		 */
		void removeItemSelectedListener(ItemSelectedListener *listener);
		
		/** 
		 * Specifies which item in the ListBox should be selected.
		 * \param selectedIndex Index of item to select.
		 */
		void setSelectedIndex(int selectedIndex);
		
		/** 
		 * Sets the orientation of the ListBox.
		 * \param orientation Orientation of the ListBox.
		 */
		void setOrientation(ListBoxOrientation orientation);
		
		/** 
		 * Sets the type of animation to be used by the ListBox.
		 * \param type The animation type.
		 */
		void setAnimationType(ListBoxAnimationType type);
		
		/** 
		 * Controls the selected item wrapping behavior of the ListBox.
		 * \param wrapping If true then wrap selection, if false no wrapping.
		 */
		void setWrapping(bool wrapping=true);
		
		/** 
		 * \return The type of animation used by the ListBox.
		 */
		ListBoxAnimationType getAnimationType() const;
		
		/** 
		 * \return true if the ListBox selction will wrap around when 
		 * moving beyond the first or last element, false if not.
		 */
		bool isWrapping() const;
		
		/** 
		 * \return The index of the currently selected child widget.
		 */
		int	getSelectedIndex() const;

		/** 
		 * Sets autosizing on a ListBox. This autosizes the children 
		 * of the listbox in the opposite direction of the orientation. 
		 * For example, if the list box is vertically oriented, the widgets 
		 * will be resized to the width of the listbox (the padded width), 
		 * and vice versa.
		 * \param autosize If true autosize, if false no autosize.
		 */
		void setAutoSize(bool autosize=true);

		// Overloaded setWidth. If autosizing is set and the orientation is 
		// vertical on the ListBox this will set the width of the children.
		// TODO: Why is this commented out?
		//void setWidth(int w);

		// Overloaded setHeight. If autosizing is set and the orientation is 
		// horizontal on the ListBox this will set the height of the children.
		// TODO: Why is this commented out?
		//void setHeight(int h);
	
		/**
		 * \return true if child widgets is outside of the front of the list.
		 */
		bool listFrontOutsideBounds() const;

		/** 
		 * \return true if child widgets is outside of the back of the list. 
		 */
		bool listBackOutsideBounds() const;

		/** 
		 * \return The current scroll offset of the ListBox in pixels. 
		 */
		int getScrollOffset() const;

		/** 
		 * Sets the scroll offset of the ListBox in pixels.
		 * \param scrollOffset The scroll offset in pixels.
		 */
		void setScrollOffset(int scrollOffset);
		
		/** 
		 * \return true if the ListBox can get focus, false if not.
		 */
		virtual bool isFocusable() const;
		
		/** 
		 * \return true if the ListBox can get focus in key mode, 
		 * false if not.
		 * TODO: What does "key mode" mean?
		 */
		virtual bool isFocusableInKeyMode() const;
		
		/**
		 * TODO: Document param id and return value, or refer
		 * to superclass comment.
		 * Called when the pointer is pressed.
		 * \param point Pointer location.
		 * \param id ?
		 * \return ?
		 */
		virtual bool pointerPressed(MAPoint2d point, int id);
		
		/**
		 * TODO: Document param id and return value, or refer
		 * to superclass comment.
		 * Called when the pointer is moved.
		 * \param point Pointer location.
		 * \param id ?
		 * \return ?
		 */
		virtual bool pointerMoved(MAPoint2d point, int id);
		
		/**
		 * TODO: Document param id and return value, or refer
		 * to superclass comment.
		 * Called when the pointer is released.
		 * \param point Pointer location.
		 * \param id ?
		 * \return ?
		 */
		virtual bool pointerReleased(MAPoint2d point, int id);
		
		/**
		 * TODO: Document return value, or refer
		 * to superclass comment.
		 * Called when a key is pressed.
		 * \param keyCode
		 * \param nativeCode
		 * \return ?
		 */
		virtual bool keyPressed(int keyCode, int nativeCode);
		bool keyPressedSelect(int keyCode, int nativeCode);

		/**
		 * Sets the item in the ListBox that has focus.
		 * \param w The widget that has focus.
		 */
		void setFocusedWidget(Widget *w);

		/**
		 * TODO: Document method, or refer to superclass comment.
		 */
		virtual void setEnabled(bool enabled=true);
		
		/**
		 * TODO: Document method, or refer to superclass comment.
		 */
		virtual void setFocused(bool focused=true);

		/**
		 * TODO: Document method, or refer to superclass comment.
		 */
		virtual int getTranslationX() const;
		
		/**
		 * TODO: Document method, or refer to superclass comment.
		 */
		virtual int getTranslationY() const;

	protected:
		/**
		 * TODO: Document method.
		 */
		void runTimerEvent();
		
		/**
		 * TODO: Document method.
		 */
		void drawWidget();
		
		/**
		 * TODO: Document method.
		 */
		void rebuild();
		
		/**
		 * TODO: Document method.
		 */
		void boundsChanged(Widget *widget, const Rect& bounds);
		
		/**
		 * TODO: Document method.
		 */
		void focusChanged(Widget *widget, bool focused);
		
		/** 
		 * TODO: Document method. 
		 */
		virtual void updateInternal();

		/** 
		 * Listeners of item selected events. 
		 */
		ListenerSet<ItemSelectedListener> mItemSelectedListeners;
		
		/** 
		 * Controls wrapping of ListBox item selection. 
		 */
		bool mWrapping;
		
		/** 
		 * The animation type of the ListBox.
		 */
		ListBoxAnimationType mAnimationType;

		/** 
		 * The orientation type of the ListBox.
		 */
		ListBoxOrientation mOrientation;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		int mYOffsetFrom;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		int mYOffsetTo;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		int mYOffset;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		int mYOffsetInc;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		int mAnimTimeStart;
		
		/** 
		 * Index of the currently selected item in the ListBox.
		 */
		int mSelectedIndex;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		bool mAutoSize;

		/** 
		 * TODO: Document how this variable is used.
		 */
		bool mTouched;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		TouchMotionTracker mTouchMotionTracker;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		double mTouchDirX;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		double mTouchDirY;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		double mTouchVelX;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		double mTouchVelY;
		
		/** 
		 * TODO: Document how this variable is used.
		 */
		double mTimeOfRelease;
		
		/** 
		 * TODO: D0cument how this variable is used.
		 */
		int mTouchedYOffset;

		/** 
		 * The currently focused widget.
		 */
		Widget *mFocusedWidget;
	};
}

#endif /* _SE_MSAB_MAUI_LISTBOX_H_ */

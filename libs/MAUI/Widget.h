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

/** \file Widget.h
 *
 * This file contains the declaration of the base Widget
 * class in MAUI.
 *
 * \author Patrick Broman and Niklas Nummelin
 *
 */

#ifndef _SE_MSAB_MAUI_WIDGET_H_
#define _SE_MSAB_MAUI_WIDGET_H_

#include <MAUtil/Vector.h>
#include <MAUtil/RefCounted.h>
#include <MAUtil/String.h>
#include <MAUtil/Geometry.h>
#include "WidgetSkin.h"
#include "InputManager.h"

namespace MAUI {

	using namespace MAUtil;

	class Widget;

#define fireBoundsChanged() \
		Vector_each(WidgetListener*, wl, widgetListeners) { \
			(*wl)->boundsChanged(this, this->bounds); \
		} \

	/** A WidgetListener can be registered with any Widget to be notified of changes
	  * to the Widget's state or actions it is subjected to.
	  **/
	
	class WidgetListener {
		public:	
			/** This function is called whenever the widget being listened to has its trigger() function
			 * invoked. See the documentation of Widget::trigger().
			**/
			virtual void triggered(Widget *widget) {};
			/** This function is called whenever the widget's selection state changes.
			**/
			virtual void selectionChanged(Widget *widget, bool selected) {};
			virtual void enableStateChanged(Widget *widget, bool enabled) {};
			/** This function is called whenever the widget being listened to is resized or moved.
			**/
			virtual void boundsChanged(Widget *widget, const Rect& bounds) {};
	};

	/** A Widget is an object that has a graphical representation and can
	  * perform (or be subjected to) (inter)actions by the user. It is abstract,
	  * since there is no default implementation of the drawWidget() function.
	  *
	  * Examples of components are the images, labels and listboxes you would
	  * usually find in a user interface.
	  *
	  * The Widget class is the abstract superclass of all MAUI Widgets. It can
	  * also be directly inherited to create new Widgets. For simple widgets,
	  * all you need to do is to implement the drawWidget() function.
	  *
	  * Widgets are rectangular and may not overlap each other. Overlapping
	  * widgets lead to undefined rendering behaviour. A widget is clipped by its 
	  * parent.
	  *  
	  * Widget constructors take a "parent" argument which, if not NULL, causes
	  * the widget being constructed to be added as a child of the parent.
	  * 
	  * When a Widget is deleted, it deletes all its children as well.
	  * 
	  * The position of a widget is always expressed relatively to the top left
	  * corner of the parent widget.
	  *
	  * Widgets have padding parameters, which specify a margin from each side
	  * of the widget by which the widgets' content is offset. This applies both
	  * to the widgets' own content (such as a the caption of a Label) and its 
	  * children.
	  *
	  * Since mobile devices have different input mechanisms, MAUI Widgets are
	  * designed not to depend on any specific form of input. They do not deal
	  * with mouse, stylus or key events. Instead, widgets provide logical actions
	  * that they can perform. For instance, a listbox does not receive MAK_UP or
	  * MAK_DOWN key events - instead, it has the functions selectNextItem() and
	  * selectPreviousItem() to move one up or down among the items it contains.
	  **/
      
	class Widget {
		friend class Screen;
		friend class Layout;
	
	public:

		/**
		 * Constructor for Widget. 'x' and 'y' are the relative position
		 * of the top left corner of the widget, and 'width' and 'height'
		 * is the dimensions of the widget. 'parent' is the parent to the
		 * widget and the widget will be automatically added as a child
		 * to the parent. If parent equals NULL no parent will be set.
		 **/
		Widget(int x, int y, int width, int height, Widget *parent);

		// Hierarchy

		/**
		 * Function used to add childs to a widget. The parent
		 * of the children will automatically be set.
		 * The widget cannot have a parent already.
		 **/
		virtual void add(Widget* w);
		
		/**
		 * Function used to set the parent of a widget. The widget
		 * will not be added to the parent as a child.
		 * The widget cannot have a parent already.
		 **/
		void setParent(Widget *w);

		/**
		 * Function to retrieve the parent of a widget.
		 **/
		Widget* getParent();

		/**
		 * Function to retrieve a reference to the list of childrens for a widget.
		 */
		Vector<Widget*>& getChildren();

		/**
		 * const version of getChildren()
		 **/
		const Vector<Widget*>& getChildren() const;		

		// Rendering, appearance

		/** 
		 * Renders the Widget and all its children recursively.
		 */
		virtual void draw(bool forceDraw=false);
		virtual void update();

		/** 
		 * Registers an idle listener with the current environment that will
		 * redraw the widget and anything else that may be made dirty by
		 * doing so. For instance, if the widget is transparent its parent
		 * also has to be repainted, and so on recursively.
		 */
		void requestRepaint();

		/**
		 * Returns whether or not the widget is transparent. A widget is transparent if it 
		 * has a skin that is transparent in the current selection state (selected/unselected) 
		 * or if it has no solid background color. 
		 */
		virtual bool isTransparent() const;

		/**
		 * Assigns a widget skin to be used during the rendering of the widget. What
		 * The skin applies to varies depending on the widget. Labels extend the skin
		 * over their entire area, while listboxes apply the skin to each individual
		 * item they contain.
		 */
		virtual void setSkin(WidgetSkin *widgetSkin);

		/**
		 * Sets the background drawing state. If b is true, the Widget's background
		 * will be drawn - otherwise not. In most cases, the background is either a
		 * constant color or a skin.
		 */
		void setDrawBackground(bool b=true);

		void setBackgroundColor(int col);

		/**
		 * Returns a pointer to the first child widget or deeper descendant,
		 * surrounding the point. Returns NULL if no widget is found.
		 */
		Widget* widgetAt(const Point& p);
		Widget* widgetAt(int x, int y);

		// Properties, geometry

		/**
		 *  setPosition() and getPosition() deal with the widget's position
		 *  relative to its children. the getBounds() function, however,
		 *  deals with the absolute position of the widget on the screen.
		 *
		 */

		/** Sets the widget's position relative to its parent. **/
		virtual void setPosition(int x, int y);
		
		/** Gets the widget's position relative to its parent. **/
		const Point& getPosition() const;
		
		/** Gets the widget's padded position relative to its parent. **/
		const Point& getPaddedPosition() const;

		/** Sets the width of the widget **/
		virtual void setWidth(int width);
		/** Returns the width of the widget **/
		int getWidth() const;	
		/** Sets the height of the widget **/
		virtual void setHeight(int height);
		/** Returns the height of the widget **/
		int getHeight() const;

		/** 
		 * Returns a Rect containing the absolute screen position of the
		 * widget together with its dimensions. 
		 */
		virtual const Rect& getBounds();
	

		/**
		 *	Check if widget surrounds position 'p'.
		 */ 
		virtual bool contains(const Point& p);

		/**
		 *	Check if widget surrounds position 'x' and 'y'.
		 */ 
		virtual bool contains(int x, int y);

		// Behaviour, notification

		/**
		 *	Set widget selected state (default: true).
		 */ 
		virtual void setSelected(bool selected=true);

		/**
		 *	Is widget selected?.
		 */ 
		bool isSelected() const;
		
		/** Set Widget enabled state **/
		virtual void setEnabled(bool enabled=true);
		/** Is Widget enabled ? **/
		bool isEnabled() const;

		/**
		 * This function is called whenever the widget is "triggered",  which is
		 * roughly equivalent to having been "clicked" on. However, sice MAUI
		 * is independant of input facilities, the more input-neutral concept of
		 * triggering is used. 
		 */ 
		virtual void trigger();

		/**
		 *	Add WidgetListener.
		 */ 
		void addWidgetListener(WidgetListener* wl);

		/**
		 *	Remove WidgetListener.
		 */ 
		void removeWidgetListener(WidgetListener* wl);

		/**
		* returns a reference the the vector of Widget listeners
		*/
		Vector<WidgetListener*>& getWidgetListeners();
		
		/**
		 *  Sets the input manager to use for this widget
		 */
		void setInputManager(InputManager* inputManager);

		/**
		 *	Set left padding width.
		 */ 
		virtual void setPaddingLeft(int l);

		/**
		 *	Set top padding height.
		 */ 
		virtual void setPaddingTop(int t);

		/**
		 *	Set right padding width.
		 */ 
		virtual void setPaddingRight(int r);

		/**
		 *	Set bottom padding height.
		 */ 
		virtual void setPaddingBottom(int b);

		/**
		 *	Get left padding width.
		 */ 
		int getPaddingLeft() const;

		/**
		 *	Get top padding height.
		 */ 
		int getPaddingTop() const;

		/**
		 *	Get right padding width.
		 */ 
		int getPaddingRight() const;

		/**
		 *	Get bottom padding height.
		 */ 
		int getPaddingBottom() const;

		const Rect& getPaddedBounds() const;

		virtual void setParameter(const String& name, const String& value);
		
		/** Handles key presses by invoking the widget's inputmanager **/
		virtual void keyPressed(int keyCode);
		/** Handles key releases by invoking the widget's inputmanager **/
		virtual void keyReleased(int keyCode);
		/** Handles pointer presses by invoking the widget's inputmanager **/
		virtual void pointerPressed(MAPoint2d p);
		/** Handles pointer releases by invoking the widget's inputmanager **/
		virtual void pointerReleased(MAPoint2d p);
		/** Handles pointer movements by invoking the widget's inputmanager **/
		virtual void pointerMoved(MAPoint2d p);

		/**
		 *	Deletes all children.
		 */ 
		virtual ~Widget();
	protected:

		/**
		   * This is the only function that is absolutely neccesary
		   * to implement when inheriting Widget to create a custom widget.
		   * It is responsible for painting the widget. When it is called, the widget
		   * must respect its own padded bounds and not extend rendering beyond that
		   * area.
		 **/
		virtual void drawWidget() = 0;
		void drawBackground();
		/**
		 * This function is used to regenerate the absolute positions of each widget
		 * in the tree. This is done in a depth first manner where each widget takes the
		 * parent absolute position and adds it to their own relative position to calculate
		 * their absolute position.
		 **/
		void updateAbsolutePosition();

		/** 
		 * the recursive function used by updateAbsolutePosition().
		 **/
		void updateAbsolutePositionChildren(int x, int y);


		/**
		 * Used to recalculate the padded bounds from the global bounds.
		 */
		void updatePaddedBounds();

		/** 
		 * Returns the dirty state of the widget, indicating whether it
		 * requires to be redrawn or not. When a widget calls requestRepaint(),
		 * it's dirty state is set to true. Normally, you shouldn't have to use
		 * isDirty() or setDirty() unless possibly when implementing your own widgets.
		 */
		bool isDirty() const;

		/** 
		 * Sets the dirty state of the widget.
		 */
		void setDirty(bool d=true);

		// a list of pointers to the children of the widget
		Vector<Widget*> children;

		// a pointer to the parent of the widget
		Widget* parent;
		
		// this contains the absolute x and y coordinate and the dimensions of the widget
		Rect bounds;
		
		// this contains the x and y coordinate relative to the parent
		int relX, relY;

		bool dirty;

		WidgetSkin *skin;
		int	backColor;
		bool shouldDrawBackground;
		
		// used to keep which state the widget is in (selected or unselected)
		bool selected;
		bool enabled;

		// a list of widget listeners
		Vector<WidgetListener*> widgetListeners;
		
		// The InputManager for this Widget
		InputManager* mInputManager;

		// padding information
		Rect paddedBounds;
		int paddingLeft;
		int paddingTop; 
		int paddingBottom;
		int paddingRight;
	};

}

#endif	//_SE_MSAB_MAUI_WIDGET_H_

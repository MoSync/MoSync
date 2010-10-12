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
* \file Widget.h
* \brief The base widget class
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_WIDGET_H_
#define _SE_MSAB_MAUI_WIDGET_H_

#include <MAUtil/Vector.h>
#include <MAUtil/RefCounted.h>
#include <MAUtil/String.h>
#include <MAUtil/Geometry.h>
#include <MAUtil/ListenerSet.h>
#include "WidgetSkin.h"
#include "InputPolicy.h"
#include "Style.h"

#ifdef MAUI_LOGGING
#include <conprint.h>
#define MAUI_LOG(x, args...) printf(x"\n", ## args)
#else
#define MAUI_LOG(x, ...)
#endif

namespace MAUI {

	using namespace MAUtil;

	/** 
	* \brief The base widget class
	*/

	class Widget;

	/*
	#define fireBoundsChanged() \
	Vector_each(WidgetListener*, wl, mWidgetListeners) { \
	(*wl)->boundsChanged(this, this->mBounds); \
	} \
	*/

	/** \brief A Widget listener which detects trigger, selection, state and positional changes.
	*
	* A WidgetListener can be registered with any Widget to be notified of changes
	* to the Widget's state or actions it is subjected to.
	**/

	class WidgetListener {
	public:

		/**
		* Maybe have these:
		* virtual void onKeyPressed(blahblah);
		* virtual void onKeyReleased(blahblah);
		* virtual void onTouchPressed
		* virtual void onTouchMoved (kanske inte behövs?)
		* virtual void onTouchReleased
		*/

		/**
		* This function is called whenever the widget's focused state changes.
		**/
		virtual void focusChanged(Widget *widget, bool focused) {};

		/**
		* This function is called whenever the widget's enable state changes.
		**/
		virtual void enableStateChanged(Widget *widget, bool enabled) {};

		/**
		*  This function is called whenever the widget being listened to is resized or moved.
		**/
		virtual void boundsChanged(Widget *widget, const Rect& bounds) {};
	};

	/** \brief A MAUI widget; a graphical object the user can interact with.
	* 
	* A Widget is an object that has a graphical representation and can
	* perform (or be subjected to) interactions by the user. It is abstract,
	* since there is no default implementation of the drawWidget() function.
	*
	* Examples of components are the images, labels and listboxes you would
	* usually find in a user interface.
	*
	* The Widget class is the abstract superclass of all MAUI Widgets. It can
	* also be directly inherited to create new Widgets. For simple widgets,
	* all you need to do is to implement the drawWidget() function and isTransparent().
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
	**/

	class Widget {
		friend class Screen;
		friend class GridLayout;

	public:
		/**
		* \brief Sets all default values (see the member documentation for more information).
		* \param x The relative x-position (if used, most layouts override this).
		* \param y The relative y-position (if used, most layouts override this).
		* \param width The width of the widget (some layouts override this).
		* \param height The height of the widget (some layouts override this).
		**/
		Widget(int x=0, int y=0, int width=0, int height=0);

		/**
		* \brief Adds a children to the widget. 
		* \note The parent of the children will automatically be set.
		* \note The widget cannot have a parent already.
		* \param w The widget to be added.
		**/
		virtual void add(Widget* w);

		// todo: Do these functions:
		// virtual void remove(Widget* w);
		// virtual int getNumChildren() const;
		// and remove getChildren()

		/**
		* \brief Function used to remove all children from a widget. 
		* \note The parent of the children will automatically removed.
		**/
		virtual void clear();

		/**
		* \brief Function used to set the parent of a widget. 
		* \note The widget will not be added to the parent as a child.
		* \note The widget cannot have a parent already.
		* \param w The widget that should be set as parent.
		**/
		void setParent(Widget *w);

		/**
		* \brief Function to retrieve the parent of a widget.
		* \returns A pointer to the parent widget, or NULL if no parent is set.
		**/
		Widget* getParent();

		/**
		* \brief Function to retrieve the the internal list of children.
		* \returns A reference to the vector of children.
		* \note Use at your own risk, adding or removing children from this list won't update the widget.
		*/
		Vector<Widget*>& getChildren();

		/**
		* \brief const version of getChildren()
		**/
		const Vector<Widget*>& getChildren() const;		

		/** 
		* \brief Renders the Widget and all its children recursively.
		* \note This function should never be used directly (it's handled by the system). Use requestRepaint instead.
		* \note It should also most likely never have to be overriden, override drawInternal instead.
		* \param forceDraw If it is set to true, the dirty flag won't be taken into account (the widget will be redrawn anyway).
		*/
		virtual void draw(bool forceDraw=false);

		/**
		* \brief Called by the Engine before drawing. If a widget has requested an update, updateInternal will be called on the widget.
		* \note This function should never be used directly (it's handled by the system). Use requestUpdate instead.
		*/
		void update();

		/** 
		* \brief Registers an idle listener with the current environment that will
		* redraw the widget and anything else that may be made dirty by
		* doing so. For instance, if the widget is transparent its parent
		* also has to be repainted, and so on recursively.
		*/
		void requestRepaint();

		/**
		* \brief Returns whether or not the widget is transparent. Must be overriden by implementing widgets.
		*/
		virtual bool isTransparent() const = 0;

		/**
		* \brief Searches all the direct and indirect children for a widget surrounding a point.
		* \param p A point.
		* \returns A pointer to the first child widget or deeper descendant, surrounding \a p. Returns NULL if no widget is found.
		*/
		Widget* widgetAt(const Point& p);

		/**
		* \brief Searches all the direct and indirect children for a widget surrounding a point.
		* \param x x-coordinate (in pixels) for the search.
		* \param y y-coordinate (in pixels) for the search.
		* \returns A pointer to the first child widget or deeper descendant, surrounding the input point. Returns NULL if no widget is found.
		*/	
		virtual Widget* widgetAt(int x, int y);

		/**
		* \brief Searches all the direct and indirect children for a focusable widget surrounding a point.
		* \param p A point.
		* \returns A pointer to the focusable first child widget or deeper descendant, surrounding \a p. Returns NULL if no focusable widget is found.
		*/
		Widget* focusableWidgetAt(const Point& p);

		/**
		* \brief Searches all the direct and indirect children for a focusable widget surrounding a point.
		* \param x x-coordinate (in pixels) for the search.
		* \param y y-coordinate (in pixels) for the search.
		* \returns A pointer to the first focusable child widget or deeper descendant, surrounding the input point. Returns NULL if no focusable widget is found.
		*/	
		virtual Widget* focusableWidgetAt(int x, int y);


		/**
		*  setPosition() and getPosition() deal with the widget's position
		*  relative to its children. the getBounds() function, however,
		*  deals with the absolute position of the widget on the screen.
		*
		*/

		// Geometry, Layout

		/**
		* \brief Sets the position for the widget relative to its parent.
		* \param x x-coordinate (in pixels) relative to the parent.
		* \param y y-coordinate (in pixels) relative to the parent.
		* \note Most layouts will override this.
		*/	
		virtual void setPosition(int x, int y);

		/**
		* \brief Gets the position of the widget relative to its parent.
		* \returns A reference to the point containing the relative position.
		*/	
		const Point& getPosition() const;

		/**
		* \brief Gets the padded position of the widget relative to its parent.
		* \returns A reference to the point containing the relative padded position.
		*/	
		const Point& getPaddedPosition() const;

		/**
		* \brief Sets the width of the widget.
		* \param width The new width in pixels.
		* \note Some layouts will override this.
		*/	
		virtual void setWidth(int width);

		/**
		* \brief Gets the width of the widget.
		* \returns An integer containing the width of the widget in pixels.
		*/	
		int getWidth() const;	

		/**
		* \brief Sets the height of the widget.
		* \param height The new height in pixels.
		* \note Some layouts will override this.
		*/	
		virtual void setHeight(int height);

		/**
		* \brief Gets the height of the widget.
		* \returns An integer containing the height of the widget in pixels.
		*/	
		int getHeight() const;

		/** 
		* \brief Returns a Rect containing the absolute screen position of the widget together with its dimensions. 
		* \returns a reference to a Rect containing the absolute positions and dimensions (in pixels).
		*/
		virtual const Rect& getBounds();

		/**
		* \brief Checks if widget surrounds position 'p'.
		* \returns A boolean value being true if \a p is surrounded by the widget, or false if it's not.
		*/ 
		virtual bool contains(const Point& p);

		/**
		* \brief Checks if widget surrounds the position.
		* \param x The x-coordinate in pixels.
		* \param y The y-coordinate in pixels.
		* \returns A boolean value being true if the position is surrounded by the widget, or false if it's not.
		*/ 
		virtual bool contains(int x, int y);


		// Behaviour, notification

		/**
		* \brief Sets widget focus state (default: true).
		* \param focused The focus state.
		* TODO: change this? several widgets should be able to have focus.
		*/ 
		virtual void setFocused(bool focused=true);

		/**
		* \brief Returns whether the widget is focused or not.
		* \returns \a true if the widget is focused, \a false if it's not.
		*/ 
		bool isFocused() const;

		/**
		* \brief Sets the widget enabled state (default: \a true). If it's not enabled it won't be drawn.
		*/ 
		virtual void setEnabled(bool enabled=true);

		/**
		* \brief Returns whether the widget is enabled or not.
		* \returns \a true if the widget is enabled, \a false if it's not.
		*/ 
		bool isEnabled() const;

		/**
		* \brief Adds a widget listener to this widget. Afterwards notifications of updates to this widget will be published to this listener.
		* \param wl The WidgetListener to be added.
		*/ 
		void addWidgetListener(WidgetListener* wl);

		/**
		* \brief Removes a widget listener from this widget. Afterwards notifications of updates to this widget will not be published to this listener.
		* \param wl The WidgetListener to be added.
		*/ 
		void removeWidgetListener(WidgetListener* wl);

		/**
		* returns a reference the the vector of Widget listeners
		*/
		//Vector<WidgetListener*>& getWidgetListeners();


		/**
		* \brief Sets the left padding width.
		* \param l The padding to be set (in pixels).
		*/ 
		virtual void setPaddingLeft(int l);

		/**
		* \brief Sets the top padding height.
		* \param t The padding to be set (in pixels).
		*/ 
		virtual void setPaddingTop(int t);

		/**
		* \brief Sets the right padding width.
		* \param r The padding to be set (in pixels).
		*/ 
		virtual void setPaddingRight(int r);

		/**
		* \brief Sets the bottom padding height.
		* \param b The padding to be set (in pixels).
		*/ 
		virtual void setPaddingBottom(int b);

		/**
		* \brief Gets the left padding width.
		* \returns The padding (in pixels).
		*/ 
		int getPaddingLeft() const;

		/**
		* \brief Gets the top padding height.
		* \returns The padding (in pixels).
		*/ 
		int getPaddingTop() const;

		/**
		* \brief Gets the right padding width.
		* \returns The padding (in pixels).
		*/ 
		int getPaddingRight() const;

		/**
		* \brief Gets the bottom padding height.
		* \returns The padding (in pixels).
		*/ 
		int getPaddingBottom() const;

		/** 
		* \brief Returns a Rect containing the padded absolute screen position of the widget together with its dimensions. 
		* \returns a reference to a Rect containing the padded absolute positions and dimensions (in pixels).
		*/
		const Rect& getPaddedBounds() const;

		/**
		*	\note Deletes all children.
		*/ 
		virtual ~Widget();


		// new stuff, put it in the right places...

		/** 
		* \brief Called by the system whenever this widget has focus and a key is pressed.
		* \returns true if focus should be held, otherwise false.
		*/
		virtual bool keyPressed(int keyCode, int nativeCode);

		/** 
		* \brief Called by the system whenever this widget has focus and a key is released.
		* \returns true if focus should be held, otherwise false.
		*/
		virtual bool keyReleased(int keyCode, int nativeCode);

		// what about: returns true if focus should be held after release.

		/** 
		* \brief Called by the system if the screen is touched, this widget is focusable and has been pressed.
		* \returns true if focus should be held, otherwise false
		*/
		virtual bool pointerPressed(MAPoint2d p, int id);

		/** 
		* \brief Called by the system has recieved touch motion events, this widget has focus and has previously been pressed.
		* \returns true if focus should be held, otherwise false
		*/
		virtual bool pointerMoved(MAPoint2d p, int id);

		/** 
		* \brief Called by the system if it has recieved a touch release event, this widget has focus and has previously been pressed.
		* \returns true if focus should be held, otherwise false
		*/
		virtual bool pointerReleased(MAPoint2d p, int id);

		// TODO: maybe add a bool isFocusableInTouchMode
		/** 
		* \brief Returns whether the widget is focusable in touch mode.
		* \returns true if it is focusable in touch mode, otherwise not.
		*/
		virtual bool isFocusable() const;

		/** 
		* \brief Returns whether the widget is focusable in key mode.
		* \returns true if it is focusable in touch mode, otherwise not.
		*/
		virtual bool isFocusableInKeyMode() const;

		/** 
		* \brief Returns the closest focusable (in key mode) in the specified direction.
		* \returns A pointer to the closest focusable or NULL if there's none.
		*/
		Widget* getFocusableInDirectionFrom(Widget* w, Direction dir);

		/** 
		* \brief Sets the input policy for a widget. All widgets uses a DefaultInputPolicy by default. Use this if you want to override the way a widget finds focus.
		* \param ip A pointer to an InputPolicy.
		*/
		void setInputPolicy(InputPolicy* ip);

		/** 
		* \brief Returns the input policy currently assigned to the widget.
		* \returns A pointer to the InputPolicy.
		*/
		InputPolicy* getInputPolicy();

		/** 
		* \brief Sets the style that the widget should use. Different widgets make use of different style properties (see the corresponding widgets for more information).
		* \param style A pointer to the style to be set.
		* \note This function will automatically call restyle().
		*/
		void setStyle(const Style* style);

		/** 
		* \brief Gets the style currently in use by the widget.
		* \returns A const pointer, pointing at the style.
		*/
		const Style* getStyle();

		/** 
		* \brief Gets the x-coordinate to which a widget will translate all its children.
		* \returns The x-coordinate (in pixels).
		*/
		virtual int getTranslationX() const;

		/** 
		* \brief Gets the y-coordinate to which a widget will translate all its children.
		* \returns The y-coordinate (in pixels).
		*/
		virtual int getTranslationY() const;

		/** 
		* \brief Requests an update. The update pass is run before all widgets is drawn (see update()).
		*/
		void requestUpdate();

		/** 
		* \brief Sets a pointer to some user specified data (it is convenient to use this as an identifier for the widget).
		* \param userData A pointer to some user data.
		*/
		void setUserData(void *userData);

		/** 
		* \brief Gets a pointer to the user data assigned to the widget.
		* \returns A pointer to the user data, or NULL if there's none.
		*/
		void* getUserData();

	protected:
		Widget* nearestWidget(Widget* w1, Widget* w2, Direction dir);
		Widget* getNearestFocusableInDirectionFrom(Widget* w, Direction dir, Widget* best=NULL);


		/**
		* This is the only function that is absolutely neccesary
		* to implement when inheriting Widget to create a custom widget.
		* It is responsible for painting the widget. When it is called, the widget
		* must respect its own padded bounds and not extend rendering beyond that
		* area.
		**/
		virtual void drawWidget() = 0;

		/**
		* This function is responsible for drawing the background skin if available.
		**/
		void drawBackground();
		
		/**
		* This function is used to regenerate the absolute positions of each widget
		* in the tree. This is done in a depth first manner where each widget takes the
		* parent absolute position and adds it to their own relative position to calculate
		* their absolute position.
		**/
		void updateAbsolutePosition();

		/** 
		* The recursive function used by updateAbsolutePosition().
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
		void setDirty(bool d=true, Widget* caller=NULL);


		/** 
		* Override this if you want to do updates before drawing a widget (called when an update is requested).
		*/
		virtual void updateInternal();

		/** 
		* Override this to retrieve properties from a style.
		*/
		virtual void restyle();

		// a list of pointers to the children of the widget
		Vector<Widget*> mChildren;

		// a pointer to the parent of the widget
		Widget* mParent;

		// this contains the absolute x and y coordinate and the dimensions of the widget
		Rect mBounds;

		// this contains the x and y coordinate relative to the parent
		int mRelX, mRelY;

		bool mDirty;
		bool mHasRequestedUpdate; // must update layout etc.

		// used to keep which state the widget is in (selected or unselected)
		bool mFocused;
		bool mEnabled;

		// a list of widget listeners
		ListenerSet<WidgetListener> mWidgetListeners;

		// padding information
		Rect mPaddedBounds;
		int mPaddingLeft;
		int mPaddingTop;
		int mPaddingBottom;
		int mPaddingRight;

		InputPolicy* mInputPolicy;

		Drawable *mFocusedSkin;
		Drawable *mUnfocusedSkin;

		void* mUserData;

	private:
		const Style* mStyle;
	};

}

#endif	//_SE_MSAB_MAUI_WIDGET_H_

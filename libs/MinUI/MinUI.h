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

#include <ma.h>
#include <maassert.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Graphics.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include <MAUtil/CharInput.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define WHITE 0xffffff
#define BLACK 0

namespace MinUI {
	class Widget {
	public:
		Widget();
		virtual ~Widget() {}
	public:
		/**
		* Returns the minumum pixel space needed to properly draw the Widget.
		*/
		virtual MAPoint2d getMinimumSize() = 0;

		/**
		* Returns the pixel space needed to draw the Widget well.
		*/
		virtual MAPoint2d getOptimalSize() = 0;

		//virtual MAPoint2d snapSize(const MAPoint2d& s, bool up);

		/**
		* If the new size is smaller than the minimum size,
		* there's been a programming error by the caller.
		*/
		virtual void setSize(const MAPoint2d& s) = 0;

		/**
		* Draws the Widget using the MAUtil Graphics API,
		* and the size set by setSize().
		*/
		void draw();

		void setBackground(bool on, int color = BLACK);
		bool hasBackground() const {
			if(mParent)
				if(mParent->hasBackground() && (mParent->backgroundColor() == mBackgroundColor))
					return false;
			return mHasBackground;
		}
		int backgroundColor() const { return mBackgroundColor; }

		/**
		* Informs the Widget that it is a child of \a parent.
		* Should only be called by container Widgets.
		*/
		void setParent(Widget* parent) {
			mParent = parent;
			mEnabled = parent->mEnabled;
		}

		/**
		* Enables drawing of the Widget.
		*/
		void enable(bool e = true) { mEnabled = e; doEnable(e); }
	protected:
		/**
		* Called by draw(), which by itself only draws the background.
		*/
		virtual void doDraw() = 0;

		/**
		* If the Widget has children, it must implement this function to pass
		* enablement on to them.
		*/
		virtual void doEnable(bool e) {}

		/**
		* Requests that the Widget be redrawn when event processing is done.
		*/
		void setRedraw();
	private:
		int mBackgroundColor;	//valid only if mHasBackground == true
		bool mHasBackground;
		Widget* mParent;
		bool mEnabled;
	};

	class SimpleMinSizeWidget : public Widget {
	public:
		MAPoint2d getMinimumSize();
		MAPoint2d getOptimalSize();
		void setSize(const MAPoint2d& s);
	protected:
		MAPoint2d mMinimumSize;
	};

	/**
	* Contains a single root Widget. Handles input.
	*/
	class View : public MAUtil::KeyListener {
	public:
		View();
		virtual ~View();

		void show();
		void hide();
	protected:
		Widget* mRootWidget;

		/**
		* Called at the beginning of show().
		*/
		virtual void doShow() {}

		/**
		* Called at the end of hide().
		*/
		virtual void doHide() {}
	};

	/**
	* Contains other Widgets in a table.
	* Default size is screen size.
	*/
	class GridLayout : public SimpleMinSizeWidget {
	public:
		/**
		* \> 0 means static, \< 0 means dynamic, 0 is illegal.
		* \todo Enable turning it sideways.
		*/
		GridLayout(int columns);

		virtual ~GridLayout();

		/**
		* Takes ownership of the widget.
		*
		* If \a row is \< 0, the widget is placed in the first available row in the specified column.
		* \param optiSize If true, the widget will be sized as close to its optimal size as possible.
		* Otherwise, the minimum size will be used.
		*/
		/*	\old crap
		* If \a column is also \< 0, the widget is placed in the first available column in the last row.
		* If \a column only is \< 0, the widget is placed in the first available column in the specified row.
		*/
		void add(Widget* w, int column, int row = -1, bool optiSize = false);

		/**
		* Makes sure that the given cell is visible.
		*/
		void scrollTo(int column, int row);

		void setShowIncompleteWidgets(bool show = true);

		//void setAllocateSpaceForScrollbar(bool asfs = true);

		Widget* getWidget(int column, int row);

		int getNColumns() const { return mNColumns; }

		int getNRows() const { return mRows.size(); }

		/**
		* Deletes all widgets in the layout.
		*/
		void clear();

	public:
		//Widget
		/**
		* The minimum size of a Layout is the size required to display the largest of its cells,
		* as determined by rearrange().
		*/
		MAPoint2d getMinimumSize();
		MAPoint2d getOptimalSize();
		void setSize(const MAPoint2d& s);
		void doDraw();
		void doEnable(bool e);

	protected:
		/**
		* Uses getMinimumSize() and setSize() of child Widgets.
		* Each row's height is detemined by the largest of minimum heights of its widgets.
		* Each column's width is similarly calculated.
		*/
		void rearrange();

		void cleanup();

		struct GWidget {
			Widget* w;
			bool optiSize;
		};

		struct Measurement {
			int chosen, min, opt;

			void in(int opt, int min, int max);
		};

		struct Row {
			GWidget* widgets;
			Measurement height;
		};

		MAUtil::Vector<Row> mRows;
		const int mNColumns;
		MAUtil::Vector<Measurement> mColumnWidths;
		MAPoint2d mSize;
		MAPoint2d mTopLeftWidgetIndex;
		MAPoint2d mOptimalSize;
		bool mShowIncompleteWidgets;
		bool mDirty;
	};

	class Image : public SimpleMinSizeWidget {
	public:
		Image(MAHandle imgHandle);
	protected:
		//Widget
		void doDraw();
		
		MAHandle mHandle;
	};

	/**
	* Uses maDrawText().
	*/
	class Textbox : public SimpleMinSizeWidget, MAUtil::KeyListener,
		protected MAUtil::CharInputListener
	{
	public:
		enum InputMode {
			IM_LOWERCASE,
			IM_UPPERCASE,
			IM_NUMBERS
		};

		/**
		* Width, height and length is measured in characters.
		* \param minWidth Guarantees that the Textbox will be able to show this many characters at once.
		* \param maxLength The maximum length of the string contained in the Textbox.
		*/
		Textbox(int minWidth, int maxLength, int textColor, int cursorColor, bool handleInput = true);

		void activate();
		void deactivate();
		void setInputMode(InputMode inputMode);
		const MAUtil::String& getText() const { return mText; }
		bool moveCursorHorizontal(int steps);
		bool deletePreviousCharacter();
		void setQwerty(bool on = true);

		//Widget
		MAPoint2d getOptimalSize();

		//KeyListener
		void keyPressEvent(int keyCode);
		void keyReleaseEvent(int keyCode);

	protected:
		//Widget
		void setSize(const MAPoint2d& s);
		void doDraw();

		//CharInputListener
		void characterChanged(char c);
		void characterDeployed(char c);

		MAPoint2d mSize;	//in pixels
		const int mTextColor, mCursorColor;
		MAUtil::String mText;
		int mCursor;
		InputMode mInputMode;
		int mMaxLength;
		int mDrawOffset;

		bool mHandleInput, mActive;
	};

	class Label : public SimpleMinSizeWidget {
	public:
		/**
		* \a text must be valid for the duration of the Label.
		*/
		Label(const char* text, int color);
	protected:
		//Widget
		void doDraw();

		const char* mText;
		const int mColor;
	};

	class StringLabel : public Label {
	public:
		/** Keeps its own copy of \a string. */
		StringLabel(const MAUtil::String& string, int color);
	protected:
		MAUtil::String mString;
	};

	class MultilineLabel : public StringLabel {
	public:
		MultilineLabel(const MAUtil::String& string, int color);
	protected:
		//Widget
		void doDraw();

		int mLineHeight;
	};

	class HyperlinkListener {
	public:
		virtual void click(int linkIndex) = 0;
	};

	class HyperlinkLabel : public MultilineLabel, MAUtil::PointerListener {
	public:
		HyperlinkLabel(HyperlinkListener* listener, const MAUtil::String& string, int baseColor, int linkColor);
	protected:
		//Widget
		void doDraw();

		//PointerListener
		void pointerPressEvent(MAPoint2d p);
		void pointerMoveEvent(MAPoint2d p);
		void pointerReleaseEvent(MAPoint2d p);

		struct Link {
			int top, left, bottom, right;
			int index;
		};

		int drawPartialString(char* start, char* end, int x, int y, bool isLink, int linkIndex = -1);
		static bool isPointInLink(const Link& link, MAPoint2d p);

		HyperlinkListener* mListener;
		int mLinkColor;
		MAUtil::Vector<Link> mLinks;
		int mLinkIndex;	//index into mLinks, not Link.index
	};
}

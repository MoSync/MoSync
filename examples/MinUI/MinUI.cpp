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

#include <mastring.h>
#include "MinUI.h"

using namespace MAUtil;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

namespace MinUI {

	//******************************************************************************
	// WidgetRedrawer
	//******************************************************************************

	class WidgetRedrawer : public IdleListener {
	public:
		void setRedraw(Widget* w) {
			mWidget = w;
			Environment::getEnvironment().addIdleListener(this);
		}
	private:
		void idle() {
			mWidget->draw();
			maUpdateScreen();
			Environment::getEnvironment().removeIdleListener(this);
		}

		Widget* mWidget;
	};
	static WidgetRedrawer sRedrawer;

	//******************************************************************************
	// Widget
	//******************************************************************************
	Widget::Widget() : mHasBackground(false), mParent(NULL),
		mEnabled(false) {}

	void Widget::setBackground(bool on, int color) {
		mHasBackground = on;
		mBackgroundColor = color;
		setRedraw();
	}

	void Widget::draw() {
		if(!mEnabled)
			return;
		if(hasBackground()) {
			maSetColor(mBackgroundColor);
			Gfx_fillRect(0, 0, 10000, 10000);	//will be clipped
		}
		doDraw();
	}

	//TODO: optimize so that only the changed parts are redrawn
	void Widget::setRedraw() {
		if(mParent != NULL) {
			mParent->setRedraw();
		} else if(mEnabled) {
			sRedrawer.setRedraw(this);
		}
	}

	//******************************************************************************
	// SimpleMinSizeWidget
	//******************************************************************************
	MAPoint2d SimpleMinSizeWidget::getMinimumSize() {
		return mMinimumSize;
	}
	MAPoint2d SimpleMinSizeWidget::getOptimalSize() {
		return mMinimumSize;
	}
	void SimpleMinSizeWidget::setSize(const MAPoint2d& s) {
		ASSERT_MSG(s.x >= mMinimumSize.x && s.y >= mMinimumSize.y,
			"set to less than minimum size");
	}

	//******************************************************************************
	// ViewManager
	//******************************************************************************
	class ViewManager {
	private:
		friend class View;
		static void setView(View*);
	};

	void ViewManager::setView(View* v) {
		static View* currentView = NULL;
		if(currentView) {
			currentView->hide();
		}
		currentView = v;
	}

	//******************************************************************************
	// View
	//******************************************************************************
	View::View() : mRootWidget(NULL) {
	}
	View::~View() {
	}

	void View::show() {
		ViewManager::setView(this);
		doShow();
		Environment::getEnvironment().addKeyListener(this);
		if(mRootWidget) {
			mRootWidget->enable();
			mRootWidget->draw();
		}
	}

	void View::hide() {
		Environment::getEnvironment().removeKeyListener(this);
		if(mRootWidget) {
			mRootWidget->enable(false);
		}
		doHide();
	}

	//******************************************************************************
	// GridLayout
	//******************************************************************************
	GridLayout::GridLayout(int columns) :
		mNColumns(columns), mColumnWidths(columns), mDirty(false)
	{
		ASSERT_MSG(columns > 0, "too few columns");

		//default size: screen
		MAExtent e = maGetScrSize();
		mSize.x = EXTENT_X(e);
		mSize.y = EXTENT_Y(e);

		mColumnWidths.resize(columns);
		rearrange();
		mTopLeftWidgetIndex.x = mTopLeftWidgetIndex.y = 0;
		setShowIncompleteWidgets();
	}

	GridLayout::~GridLayout() {
		clear();
	}

	void GridLayout::add(Widget* w, int column, int row, bool optiSize) {
		ASSERT_MSG(column < mNColumns && column >= 0, "column out of bounds");
		if(row < 0) {
			for(row=0; row<mRows.size(); row++) {
				if(mRows[row].widgets[column].w == NULL) {
					break;
				}
			}
			if(row == mRows.size()) {	//no free spot found
				//add a new row
				Row r;
				r.widgets = new GWidget[mNColumns];
				memset(r.widgets, 0, sizeof(GWidget) * mNColumns);
				mRows.add(r);
			}
		}
		ASSERT_MSG(row < mRows.size(), "row out of bounds");
		GWidget& wr = mRows[row].widgets[column];
		if(wr.w != NULL) {
			delete wr.w;
		}
		wr.w = w;
		wr.optiSize = optiSize;
		w->setParent(this);
		mDirty = true;
	}

	Widget* GridLayout::getWidget(int column, int row) {
		ASSERT_MSG(column < mNColumns, "column out of bounds");
		ASSERT_MSG(row < mRows.size(), "row out of bounds");
		return mRows[row].widgets[column].w;
	}

	void GridLayout::scrollTo(int column, int row) {
		ASSERT_MSG(column < mNColumns, "column out of bounds");
		ASSERT_MSG(row < mRows.size(), "row out of bounds");

		if(column < mTopLeftWidgetIndex.x) {
			mTopLeftWidgetIndex.x = column;
			setRedraw();
		} else if(column > mTopLeftWidgetIndex.x) {
			//int y = mTopLeftWidgetIndex.y;
			//if it's visible already, do nothing
			int width = 0;
			int x = column;
			while(width < mSize.x && x >= mTopLeftWidgetIndex.x) {
				width += mColumnWidths[x].chosen;
				x--;
			}
			if(width > mSize.x) {
				mTopLeftWidgetIndex.x = x + 1;
				setRedraw();
			}
		}
		if(row < mTopLeftWidgetIndex.y) {
			mTopLeftWidgetIndex.y = row;
			setRedraw();
		} else if(row > mTopLeftWidgetIndex.y) {
			int height = 0;
			int y = row;
			while(height < mSize.y && y >= mTopLeftWidgetIndex.y) {
				height += mRows[y].height.chosen;
				y--;
			}
			if(height > mSize.y) {
				mTopLeftWidgetIndex.y = y + 1;
				setRedraw();
			}
		}
	}

	void GridLayout::clear() {
		Vector_each(Row, iRow, mRows) {
			for(int i=0; i<mNColumns; i++) {
				Widget* w = iRow->widgets[i].w;
				if(w)
					delete w;
			}
			delete iRow->widgets;
		}
		mRows.clear();
		setRedraw();
	}

	void GridLayout::setShowIncompleteWidgets(bool show) {
		mShowIncompleteWidgets = show;
		setRedraw();
	}

	void GridLayout::cleanup() {
		if(mDirty) {
			mDirty = false;
			rearrange();
		}
	}

	void GridLayout::doEnable(bool e) {
		Vector_each(Row, iRow, mRows) {
			for(int i=0; i<mNColumns; i++) {
				Widget* w = iRow->widgets[i].w;
				if(w)
					w->enable(e);
			}
		}
	}
	MAPoint2d GridLayout::getMinimumSize() {
		cleanup();
		return SimpleMinSizeWidget::getMinimumSize();
	}
	MAPoint2d GridLayout::getOptimalSize() {
		cleanup();
		return mOptimalSize;
	}
	void GridLayout::setSize(const MAPoint2d& s) {
		mSize = s;
		cleanup();
		SimpleMinSizeWidget::setSize(s);
	}
	void GridLayout::doDraw() {
		cleanup();
		int y=0;
		for(int row=mTopLeftWidgetIndex.y; row<mRows.size(); row++) {
			const int height = mRows[row].height.chosen;
			int x=0;
			for(int col=mTopLeftWidgetIndex.x; col<mNColumns; col++) {
				int width = mColumnWidths[col].chosen;
				if(col == (mNColumns - 1)) {	//last column
					if((x + width) < mSize.x)
						width += mSize.x - (x + width);
				}
				if(x > mSize.x)
					break;
				if(!mShowIncompleteWidgets && (x + width) > mSize.y)
					break;
				if(mRows[row].widgets[col].w != NULL) {
					Gfx_pushRect(x, y, width, height);
					mRows[row].widgets[col].w->draw();
					Gfx_popRect();
				}
				x += width;
			}
			y += height;
		}
	}

	void GridLayout::Measurement::in(int aOpt, int aMin, int aMax) {
		chosen = MAX(chosen, MIN(aOpt, MAX(aMin, aMax)));
		min = MAX(min, aMin);
		opt = MAX(opt, aOpt);
	}

	void GridLayout::rearrange() {
		//minSize, optSize and chosenSize are all different.
		//chosenSize of this widget is known and unchangable; it is mSize.
		//minSize and optSize are calculated in this function.
		//chosenSize of children are calculated and set in this function.

		mMinimumSize.x = mMinimumSize.y = 0;
		mOptimalSize.x = mOptimalSize.y = 0;
		//zero column widths
		Vector_each(Measurement, itr, mColumnWidths) {
			itr->chosen = itr->min = itr->opt = 0;
		}
		//calculate table heights and widths
		Vector_each(Row, iRow, mRows) {
			iRow->height.chosen = iRow->height.min = iRow->height.opt = 0;
			for(int i=0; i<mNColumns; i++) {
				GWidget& gw = iRow->widgets[i];
				if(gw.w) {
					MAPoint2d ms = gw.w->getMinimumSize();
					if(gw.optiSize) {
						MAPoint2d os = gw.w->getOptimalSize();
						iRow->height.in(os.y, ms.y, mSize.y);
						mColumnWidths[i].in(os.x, ms.x, mSize.x);
					} else {
						iRow->height.in(ms.y, ms.y, ms.y);
						mColumnWidths[i].in(ms.x, ms.x, ms.x);
					}
				}
			}
		}
		//calculate minimum and optimal widths
		for(int i=0; i<mNColumns; i++) {
			mMinimumSize.x = MAX(mMinimumSize.x, mColumnWidths[i].min);
			mOptimalSize.x = MAX(mOptimalSize.x, mColumnWidths[i].opt);
		}

		//set child sizes and calculate minimum and optimal height
		Vector_each(Row, iRow, mRows) {
			for(int i=0; i<mNColumns; i++) {
				Widget* w = iRow->widgets[i].w;
				if(w) {
					MAPoint2d s = { mColumnWidths[i].chosen, iRow->height.chosen };
					w->setSize(s);
				}
			}
			mMinimumSize.y = MAX(mMinimumSize.y, iRow->height.min);
			mOptimalSize.y += iRow->height.opt;
		}
		//finish up
		setSize(mSize);
		setRedraw();
	}

	//******************************************************************************
	// Label
	//******************************************************************************
	Label::Label(const char* text, int color) :
		mText(text), mColor(color)
	{
		MAExtent s = maGetTextSize(text);
		mMinimumSize.x = EXTENT_X(s);
		mMinimumSize.y = EXTENT_Y(s);
	}
	void Label::doDraw() {
		maSetColor(mColor);
		Gfx_drawText(0, 0, mText);
	}

	//******************************************************************************
	// MultilineLabel
	//******************************************************************************
	MultilineLabel::MultilineLabel(const MAUtil::String& string, int color) :
		StringLabel(string, color)
	{
		mLineHeight = mMinimumSize.y;	//estimate. hope it holds.
		//find max size
		char* ptr = (char*)mText;
		mMinimumSize.y = 0;
		mMinimumSize.x = 0;
		while(*ptr != 0) {
			char* newLine = strchr(ptr, '\n');
			if(newLine) {
				*newLine = 0;
			}
			MAExtent lineSize = maGetTextSize(ptr);
			mMinimumSize.x = MAX(mMinimumSize.x, EXTENT_X(lineSize));
			mMinimumSize.y += mLineHeight;//EXTENT_Y(lineSize);
			if(newLine) {
				*newLine = '\n';
				ptr = newLine + 1;
			} else {
				break;
			}
		}
	}
	void MultilineLabel::doDraw() {
		maSetColor(mColor);
		char* ptr = (char*)mText;
		int y = 0;
		while(1) {
			char* newLine = strchr(ptr, '\n');
			if(newLine) {
				*newLine = 0;
			}
			Gfx_drawText(0, y, ptr);
			if(newLine) {
				*newLine = '\n';
				ptr = newLine + 1;
				y += mLineHeight;
			} else {
				break;
			}
		}
	}

	//******************************************************************************
	// HyperlinkLabel
	//******************************************************************************
	HyperlinkLabel::HyperlinkLabel(HyperlinkListener* listener, const MAUtil::String& string, int baseColor, int linkColor)
		: MultilineLabel(string, baseColor), mListener(listener), mLinkColor(linkColor), mLinkIndex(-1)
	{
		//TODO: proper estimate
		Environment::getEnvironment().addPointerListener(this);
	}

#define UNDERSCORE_HEIGHT 1
	int HyperlinkLabel::drawPartialString(char* start, char* end, int x, int y, bool isLink, int linkIndex) {
		char endChar = *end;
		*end = 0;
		maSetColor(isLink ? mLinkColor : mColor);
		Gfx_drawText(x, y, start);
		MAExtent size = maGetTextSize(start);
		if(isLink) {
			int lineY = y + mLineHeight - UNDERSCORE_HEIGHT;
			Gfx_line(x, lineY, x + EXTENT_X(size), lineY);
		}
		*end = endChar;
		if(isLink) {
			MAPoint2d trans = Gfx_getTranslation();
			Link link = { y + trans.y, x + trans.x, y + mLineHeight + trans.y, x + EXTENT_X(size) + trans.x, linkIndex };
			mLinks.add(link);
		}
		return EXTENT_X(size);
	}

	void HyperlinkLabel::doDraw() {
		maSetColor(mColor);
		char* ptr = (char*)mText;
		char* basePtr = ptr;
		int y = 0;
		int x = 0;
		bool inHyperlink = false;
		int linkIndex = 0;
		mLinks.clear();
		while(*ptr != 0) {
			if(*ptr == 1) {	//start or end of hyperlink
				if(inHyperlink) {	//end of link
					//TODO: add check for empty link
					x += drawPartialString(basePtr, ptr, x, y, true, linkIndex++);
					inHyperlink = false;
				} else {	//start of link
					//TODO: add check for empty normal text
					x += drawPartialString(basePtr, ptr, x, y, false);
					inHyperlink = true;
				}
				basePtr = ptr + 1;
			} else if(*ptr == '\n') {	//end of line
				if(inHyperlink) {
					drawPartialString(basePtr, ptr, x, y, true, linkIndex);
				} else {
					drawPartialString(basePtr, ptr, x, y, false);
				}
				x = 0;
				y += mLineHeight;
				basePtr = ptr + 1;
			}
			ptr++;
		}
		if(basePtr != ptr && !inHyperlink) {
			drawPartialString(basePtr, ptr, x, y, false);
		}
	}

	void HyperlinkLabel::pointerPressEvent(MAPoint2d p) {
		for(int i=0; i< mLinks.size(); i++) {
			const Link& link(mLinks[i]);
			if(isPointInLink(link, p)) {
				mLinkIndex = i;
				break;
			}
		}
	}
	void HyperlinkLabel::pointerMoveEvent(MAPoint2d p) {}
	void HyperlinkLabel::pointerReleaseEvent(MAPoint2d p) {
		if(mLinkIndex >= 0) {
			const Link& link(mLinks[mLinkIndex]);
			if(isPointInLink(link, p)) {
				mListener->click(link.index);
			}
			mLinkIndex = -1;
		}
	}

	bool HyperlinkLabel::isPointInLink(const Link& link, MAPoint2d p) {
		return p.x > link.left && p.x < link.right && p.y > link.top && p.y < link.bottom;
	}

	//******************************************************************************
	// StringLabel
	//******************************************************************************
	StringLabel::StringLabel(const String& string, int color)
		//keeping a copy of the string will keep the c_str() valid.
		: Label(string.c_str(), color), mString(string) {}	

	//******************************************************************************
	// Image
	//******************************************************************************
	Image::Image(MAHandle imgHandle) : mHandle(imgHandle)
	{
		MAExtent s = maGetImageSize(mHandle);
		mMinimumSize.x = EXTENT_X(s);
		mMinimumSize.y = EXTENT_Y(s);
	}
	void Image::doDraw() {
		Gfx_drawImage(mHandle, 0, 0);
	}

	//******************************************************************************
	// Textbox
	//******************************************************************************
	Textbox::Textbox(int minWidth, int maxLength, int textColor, int cursorColor, bool handleInput)
		:	mTextColor(textColor), mCursorColor(cursorColor), mText(maxLength),
		mInputMode(IM_LOWERCASE), mMaxLength(maxLength), mDrawOffset(0),
		mHandleInput(handleInput), mActive(false)
	{
		//ASSERT_MSG(minHeight == 1, "single-line only for now");
		ASSERT_MSG(minWidth <= maxLength, "minWidth > maxLength");
		MAExtent s = maGetTextSize("_");	//biggest character, I think
		mMinimumSize.x = EXTENT_X(s) * minWidth;
		mMinimumSize.y = EXTENT_Y(s) * 1;//minHeight;

		mCursor = 0;
	}

	MAPoint2d Textbox::getOptimalSize() {
		MAPoint2d os;
		os.y = mMinimumSize.y;
		os.x = 10000;	//hack
		return os;
	}

	void Textbox::activate() {
		if(mActive)
			return;
		if(mHandleInput) {
			Environment::getEnvironment().addKeyListener(this);
		}
		CharInput::getCharInput().addCharInputListener(this);
		setInputMode(mInputMode);
		mActive = true;
	}

	void Textbox::deactivate() {
		if(!mActive)
			return;
		if(mHandleInput) {
			Environment::getEnvironment().removeKeyListener(this);
		}
		CharInput::getCharInput().removeCharInputListener(this);
		mActive = false;
	}

	void Textbox::setInputMode(Textbox::InputMode inputMode) {
		CharInput::CharMode cMode;
		switch(inputMode) {
			case IM_LOWERCASE: cMode = CharInput::LOWERCASE; break;
			case IM_UPPERCASE: cMode = CharInput::UPPERCASE; break;
			case IM_NUMBERS: cMode = CharInput::NUMBERS; break;
			default: BIG_PHAT_ERROR;
		}
		CharInput::getCharInput().setMode(cMode);
		mInputMode = inputMode;
	}

	bool Textbox::moveCursorHorizontal(int steps) {
		if(mCursor + steps < 0 || mCursor + steps > mText.size())
			return false;
		mCursor += steps;
		return true;
	}

	bool Textbox::deletePreviousCharacter() {
		if(mCursor - 1 < 0)
			return false;
		mCursor--;
		mText.remove(mCursor, 1);
		return true;
	}

	void Textbox::keyPressEvent(int keyCode, int nativeCode) {
		switch(keyCode) {
			case MAK_LEFT:
				if(moveCursorHorizontal(-1)==false)
					return;
				break;
			case MAK_RIGHT:
				if(moveCursorHorizontal(1)==false)
					return;
				break;
			case MAK_CLEAR:
				deletePreviousCharacter();
				break;
			default:
				return;
		}

		CharInput::getCharInput().forceDeployment();
		setRedraw();
	}
	void Textbox::keyReleaseEvent(int keyCode) {
	}

	void Textbox::characterChanged(char c) {
	}
	void Textbox::characterDeployed(char c) {
		if(mText.length() >= mMaxLength)
			return;	//TODO: do something better to alert the user he's out of space
		if(mCursor == mText.length())
			mText += c;
		else
			mText.insert(mCursor, c);
		mCursor++;
		setRedraw();
	}

	void Textbox::setSize(const MAPoint2d& s) {
		mSize = s;
		SimpleMinSizeWidget::setSize(s);
	}
	void Textbox::doDraw() {
		//TODO: scroll properly

		//Extent textSize = maGetTextSize(mText.c_str());
		const char* str = mText.c_str();
		int cursorX;
		if(mCursor == 0) {
			cursorX = 0;
			mDrawOffset = 0;
		} else {
			char temp = mText[mCursor];
			mText[mCursor] = 0;
#if 1
			if(mCursor < mDrawOffset) {
				mDrawOffset = mCursor - 1;
			}
			mDrawOffset--;
			do {
				mDrawOffset++;
				MAExtent s = maGetTextSize(str + mDrawOffset);
				cursorX = EXTENT_X(s);
			} while(cursorX >= mSize.x);
#else
			mDrawOffset--;
			do {
				mDrawOffset++;
				Extent s = maGetTextSize(str + mDrawOffset);
				cursorX = EXTENT_X(s);
			} while(cursorX >= mSize.x);
#endif
			mText[mCursor] = temp;
			MAASSERT(mDrawOffset < mCursor);
		}

		maSetColor(mTextColor);
		Gfx_drawText(0, 0, str + mDrawOffset);

		maSetColor(mCursorColor);
		Gfx_line(cursorX, 0, cursorX, mMinimumSize.y);
	}

}

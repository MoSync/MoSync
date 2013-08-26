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
 * \file NativeEditBox.h
 * \brief Class that wraps maTextBox.
 * \author Patrick Broman and Niklas Nummelin
 */

#ifndef _SE_MSAB_MAUI_NATIVE_EDIT_BOX_H_
#define _SE_MSAB_MAUI_NATIVE_EDIT_BOX_H_

#include "Label.h"
#include <MAUtil/Environment.h>
#include <MAUtil/String.h>

namespace MAUI {

class NativeEditBox;

/**
 * \brief Listener for a native edit box.
 */
class NativeEditBoxListener {
public:
	/**
	 * Called when the user has pressed the OK button on
	 * the native edit box, closing it.
	 * TODO: We should also detect cancel. Add a third arg for this?
	 * \param editBox The editbox that was closed.
	 * \param text The text entered by the user.
	 */
	virtual void nativeEditFinished(
		NativeEditBox* editBox,
		const String& text) = 0;

	/**
	* Called when a Unicode character needs to be converted to Latin-1.
	* \param unicode The Unicode character to be converted.
	* \return A Latin-1 character, or 0. If 0 is returned, NativeEditBox calls maPanic().
	*/
	virtual unsigned char nativeEditUnicode(int unicode) = 0;
};

/**
 * \brief Widget that displays text and opens a native edit box
 * to edit the text.
 */
class NativeEditBox : public Label, public TextBoxListener {
public:
	/**
	 * Constructor.
	 * \param mainListener The first listener. May not be removed.
	 * \param x The left coordinate of the widget.
	 * \param y The top coordinate of the widget.
	 * \param width The width of the widget.
	 * \param height The height of the widget.
	 * \param maxSize Max number of characters in the widget, including the terminating zero.
	 * \param options Textbox options.
	 * \param initialText The text to be initially displayed.
	 * \param titleString The title of the textbox.
	 */
	NativeEditBox(
		NativeEditBoxListener* mainListener,
		int x=0,
		int y=0,
		int width=0,
		int height=0,
		int maxSize=128,
		int options=MA_TB_TYPE_ANY,
		const MAUtil::String& initialText="",
		const MAUtil::WString& titleString=L"");

	/**
	 * Destructor.
	 */
	~NativeEditBox();

	/**
	 * Set the textbox options.
	 * \param options MA_TB_* constants defined in maapi.h.
	 */
	void setOptions(int options);

	/**
	 * Set the max number of characters in the textbox.
	 * \param size The max number of characters.
	 */
	void setMaxSize(int size);

	/**
	 * Set the title of the textbox.
	 * \param titleString The title string.
	 */
	void setTitleString(const MAUtil::WString& titleString);

	/**
	 * \return The title string of the textbox.
	 */
	const MAUtil::WString& getTitleString() const;

	bool keyPressed(int keyCode, int nativeCode);

	/**
	 * TODO: Document or refer to superclass comment.
	 */
	bool pointerPressed(MAPoint2d p, int id);

	/**
	 * TODO: Document or refer to superclass comment.
	 */
	bool pointerMoved(MAPoint2d p, int id);

	/**
	 * TODO: Document or refer to superclass comment.
	 */
	bool pointerReleased(MAPoint2d p, int id);

	/**
	 * TODO: What is the caption? The content of the label?
	 * \param caption If the caption is larger than mMaxSize,
	 * mMaxSize will be changed to the length of the caption.
	 */
	virtual void setCaption(const String& caption);

	/**
	 * TODO: Document or refer to superclass comment.
	 */
	void activate();

	/**
	 * Add a listener for the edit box.
	 */
	void addNativeEditBoxListener(NativeEditBoxListener* listener);

	/**
	 * Remove a listener for the edit box.
	 */
	void removeNativeEditBoxListener(NativeEditBoxListener* listener);

protected:
	/// TextBoxListener
	void textBoxClosed(int res, int length);

	/**
	 * The title of the native textbox.
	 */
	MAUtil::WString mTitleString;

	/**
	 * TODO: Is this comment correct?
	 * Content of the textbox/label.
	 */
	wchar_t* mString;

	/**
	 * TODO: Is this comment correct?
	 * Max number of characters in the textbox.
	 */
	int mMaxSize;

	/**
	 * TODO: Is this comment correct?
	 * Options for maTextBox.
	 */
	int mOptions;

	/**
	 * TODO: Document this variable.
	 */
	int mStartX;

	/**
	 * TODO: Document this variable.
	 */
	int mStartY;

	/**
	 * Listeners.
	 */
	ListenerSet<NativeEditBoxListener> mEditBoxListeners;

	/// Main listener.
	NativeEditBoxListener* mListener;
};

} // namespace MAUI

#endif // _SE_MSAB_MAUI_NATIVE_EDIT_BOX_H_

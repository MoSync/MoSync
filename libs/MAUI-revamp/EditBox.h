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
* \file EditBox.h 
* \brief Edit box widget, multi and single line.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_EDITBOX_H_
#define _SE_MSAB_MAUI_EDITBOX_H_

#include "Widget.h"
#include "Font.h"
#include <MAUtil/String.h>
#include <MAUtil/CharInput.h>
#include <MAUtil/Environment.h>

#include "Label.h"

namespace MAUI {

	class EditBox;

	/** 
	 * \brief Listener for EditBox events.
	 * 
	 * Each time the text is changed the textChanged callback will be invoked.
	 */
	class EditBoxListener {
	public:
		/**
		 * Called when the text in an EditBox has changed.
		 * \param editBox The EditBox that has changed.
		 * \param text The new text.
		 */
		virtual void textChanged(EditBox* editBox, const String& text);
		
		/**
		 * Called when the cursor inside an EditBox has moved.
		 * \param editBox The EditBox that has changed.
		 * \param newCursorPosition The new cursor position.
		 * \param lastCursorPosition The old cursor position.
		 */
		virtual void cursorMoved(
			EditBox* editBox, 
			int newCursorPosition, 
			int lastCursorPosition);
			
		/**
		 * Called when a character is deleted in an EditBox.
		 * \param editBox The EditBox that has changed.
		 * \param ch The deleted character.
		 */
		virtual void characterDeleted(EditBox* editBox, char ch);
		
		/**
		 * TODO: What does it mean to "change" a character?
		 * Document this. 
		 *
		 * Called when a character is changed in an EditBox.
		 * \param editBox The EditBox that has changed.
		 * \param ch The changed character.
		 */
		virtual void characterChanged(EditBox* editBox, char ch);
		
		/**
		 * Called when a character is added to an EditBox.
		 * \param editBox The EditBox that has changed.
		 * \param ch The added character.
		 */
		virtual void characterAdded(EditBox* editBox, char ch);
	};

	/** 
	 * \brief Edit box widget, multi and single line
	 * 
	 * This widget has two modes: multi line and single line.
	 * You can either make a custom behaviour of navigating in
	 * the editbox by calling the navigation functions such as: 
	 * moveCursorVertical, moveCursorHorizontal etc, or use the 
	 * default behaviour. Be sure to set the editbox as selected 
	 * when you want to use it (when using the default behaviour 
	 * it is then added as a keylistener to the system and responds 
	 * to all keypresses until it is deselected. Be sure to implement 
	 * this behaviour using a widgetlistener and the activate/deactivate 
	 * functions if you are making a custom behaviour). 
	 * See Label for more information.
	 */
	class EditBox : 
		public Label, protected CharInputListener, protected KeyListener {
	public:
		/** 
		 * Enumeration for input modes. 
		 */
		enum InputMode {
			IM_NUMBERS,
			IM_STANDARD,
			IM_QWERTY
		};

		/** Constructor.
		 * \param x the horizontal position of the Label relative to 
		 *  its parent's top left padded corner.
		 * \param y the vertical position of the Label relative to its 
		 *  parent's top left padded corner
		 * \param width the width of the Label.
		 * \param height the height of the Label.
		 * \param parent pointer to the parent widget. Passing anything else
		 *  than NULL causes the Label to be added to the parent's children.
		 * \param text dictates the initial content. 
		 * \param backColor controls the background color. 
		 * \param font the font used to draw the text in this widget. 
		 *  If NULL, the default font is used. See Engine::getDefaultFont().
		 * \param manageNavigation determines whether the EditBox should 
		 *  automatically respond to key presses when it is selected in a 
		 *  default fashion. 
		 * \param multiLine controls whether multiple lines of text are 
		 *  supported. 
		 * \param maxLength controls the maximum number of characters that 
		 *  can be entered.
		 * \param inputMode the input mode.
		 */
		EditBox(
			int x=0, 
			int y=0, 
			int width=0, 
			int height=0, 
			const String &text="",
			bool manageNavigation=true, 
			bool multiLine=false,
			int maxLength=64, 
			InputMode inputMode=IM_STANDARD);

		/** 
		 * Sets the color of the cursor.
		 * \param color The cursor color.
		 */
		void setCursorColor(int color);
		
		/** 
		 * Returns the color used to draw the cursor.
		 * \return The cursor color.
		 */
		int getCursorColor() const;
		
		/** 
		 * TODO: Document what automatic navigation management is.
		 *
		 * Turns automatic managment of navigation on or off.
		 * \param manageNavigation If true turn on automatic managment 
		 *  of navigation, if false turn it off.
		 */
		void setManageNavigation(bool manageNavigation=true);

		// /** Turns multiline mode on or off **/
		//void setMultiLine(bool b=true);
		
		// /** Returns whether multiline is enabled or not **/
		//bool isMultiLine();

		/** 
		 * Returns whether navigation is being managed.
		 * \return true if automatic managment of navigation is on, 
		 * false if it is off.
		 */
		bool isManagingNavigation();
		
		/**
		 * Sets the maximum number of characters allowed.
		 * \param maxLength The maximum number of characters in the EditBox.
		 */
		void setMaxLength(int maxLength);
		
		/** 
		 * Returns the maximum number of characters allowed.
		 * \return The maximum number of characters in the EditBox.
		 */
		int getMaxLength();
		
		/** 
		 * TODO: Is selected the same as focus? Clarify this.
		 * 
		 * Set whether the EditBox is selected.
		 * \param selected Selection state of the EditBox.
		 */
		void setSelected(bool selected=true);
		
		/** 
		 * Registers the EditBox as a KeyListener with the current 
		 * Environment, updating the content in response to key presses. 
		 */ 
		void activate();
		
		/** 
		 * Unregisters the KeyListener used to update the content. 
		 */
		void deactivate();
		
		/** 
		 * Returns whether the EditBox is listening for input. 
		 * \return true if listening for key events, false if not.
		 */
		bool isActive() const;
	
		/** 
		 * Implementation of the KeyListener interface, updating the content.
		 * \param keyCode The code of the key that was pressed.
		 * \param nativeCode The native code of the key that was pressed.
		 */
		void keyPressEvent(int keyCode, int nativeCode);
		
		/** 
		 * Implementation of the KeyListener interface, updating the content.
		 * \param keyCode The code of the key that was released.
		 * \param nativeCode The native code of the key that was release.
		 */
		void keyReleaseEvent(int keyCode, int nativeCode);
		
		/**
		 * TODO: Document what it means to change a character.
		 * Implementation of the CharInputListener interface function.
		 * \param ch TODO: Document.
		 */
		void characterChanged(char ch);
		
		/** 
		 * TODO: Document what it means to deploy a character.
		 * Implementation of the CharInputListener interface function.
		 * \param ch TODO: Document.
		 */
		void characterDeployed(char ch);
		
		/** 
		 * Moves the cursor horizontally the specified number of steps. 
		 * Negative steps go left, positive right.
		 * \param steps The number of characters the cursor should be moved,
		 *  use negative value to go to the left.
		 * \param fireListeners If true, notify listeners of this EditBox.
		 * \return TODO: Document return value.
		 */
		bool moveCursorHorizontal(int steps, bool fireListeners=true);
		
		/** 
		 * Removes the character at the current cursor position. 
		 * \return TODO: Document return value.
		 */ 
		bool deleteCurrentCharacter();
		
		/** 
		 * Removes the character before the current cursor position.
		 * \return TODO: Document return value.
		 */
		bool deletePreviousCharacter();
		
		/** 
		 * Clears the text in the EditBox. 
		 */ 
		void clearText();

		/** 
		 * TODO: Document the difference between text and caption.
		 * Set the caption of the EditBox. 
		 * \param caption The caption string.
		 */ 
		virtual void setCaption(const String& caption);
	
		/** 
		 * Sets the text of the EditBox.
		 * \param text The text content string.
		 */
		void setText(const String& text);
		
		/** 
		 * TODO: Document the difference between text and caption.
		 * Note that there is no method getCaption in this class.
		 * Implement method or update the method comment.
		 *
		 * Returns the content of the EditBox.
		 * In password mode, this returns the password, while
		 * getCaption() returns the asterisks.
		 * \return The text content of the EditBox.
		 */
		const String& getText() const;
		
		/** 
		 * Set the input mode of the EditBox.
		 * \param inputMode One of the InputMode enum constants.
		 */
		void setInputMode(InputMode inputMode);
		
		/** 
		 * Returns the input mode of the EditBox.
		 * \param The InputMode enum constant that represents the
		 *  current input mode.
		 */
		InputMode getInputMode() const;

		/**
		 * TODO: Document the difference between "active" and "enabled".
		 * What does this method do?
		 *
		 * \param enabled TODO: Document.
		 */
		void setEnabled(bool enabled = true);

		/**
		 * Add a listener to the EditBox.
		 * \param listener An EditBoxListener object.
		 */
		void addEditBoxListener(EditBoxListener* listener);

		/**
		 * TODO: Document this method.
		 * \param TODO: Document.
		 */
		void setCaseToggle(bool caseToggle=true);

		/** 
		 * Sets password display mode of the EditBox.
		 * In password mode characters are rendered as asterisks (*).
		 * \param enabled If true set password mode, if false set normal mode.
		 */
		void setPasswordMode(bool enabled = true);

	protected:
		/**
		 * TODO: Document method.
		 * \param ch TODO: Document.
		 */
		bool deleteCharacter(int ch);

		/**
		 * Notify listeners that the text of the EditBox has changed.
		 */
		void fireTextChanged();
		
		/**
		 * Notify listeners that the cursor has moved.
		 * \param lastCursor The old cursor position.
		 */
		void fireCursorMoved(int lastCursor);
		
		/**
		 * Notify listeners that a character has been deleted.
		 * \param ch The deleted character.
		 */
		void fireCharacterDeleted(char ch);
		
		/**
		 * Notify listeners that a character has been added.
		 * \param ch The added character.
		 */
		void fireCharacterAdded(char ch);
		
		/**
		 * TODO: Document what it means to "change" a character.
		 *
		 * Notify listeners that a character has been changed.
		 * \param ch The changed character.
		 */
		void fireCharacterChanged(char ch);

		/**
		 * Render the EditBox.
		 */
		void drawWidget();

		// TODO: Delete if not needed.
		//Font* font;
		//String text;
		
		/**
		 * Text cursor postion.
		 */
		int mCursorIndex;
		
		/**
		 * Flag for automatic navigation management.
		 */
		bool mManageNavigation;
	
		/**
		 * Flag that determines if the EditBox will receive key events.
		 */
		bool mActive;
		
		/**
		 * Color of the text cursor.
		 */
		int mCursorColor;
		
		/**
		 * TODO: Document this flag and its relation to the mActive flag.
		 */
		bool mCharacterInputActive;

		//bool multiLine;
		
		/**
		 * The index of the line that contains the text cursor (?).
		 */
		int mCurrentLine;
		
		//int currentIndex;

		/**
		 * Maximum number of characters allowed in the EditBox.
		 */
		int mMaxLength;

		/**
		 * The current input mode of the EditBox.
		 */
		InputMode mInputMode;

		/**
		 * List of listeners for this EditBox.
		 */
		Vector<EditBoxListener*> mEditBoxListeners;

		/**
		 * TODO: Document this flag.
		 */
		bool mCaseToggle;

		/**
		 * Flag for how to display the text, if true hide the text
		 * by displaying asterisk characters.
		 */
		bool mPasswordMode;
		
		/**
		 * The actual content of the EditBox in password mode.
		 */
		String mPassword;
	};
}

#endif	//_SE_MSAB_MAUI_EDITBOX_H_

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

/** \file EditBox.h Copyright (c) Mobile Sorcery AB 2005-2006 **/

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

	/** Use this to listen for events from the EditBox. Each time the text is changed the textChanged callback will be invoked. */
	class EditBoxListener {
	public:
		virtual void textChanged(EditBox *eb, const String& text);
		virtual void cursorMoved(EditBox *eb, int newCursor, int lastCursor);
		virtual void characterDeleted(EditBox *eb, char c);
		virtual void characterChanged(EditBox *eb, char c);
		virtual void characterAdded(EditBox *eb, char c);
	};

	/** This is an EditBox widget. It has two modes: multi line and single line.
	  * You can either make a custom behaviour of navigating in the editbox by calling
	  * the navigation functions such as: moveCursorVertical, moveCursorHorizontal etc.
	  * or use the default behaviour. Be sure to set the editbox as selected when you want to 
	  * use it (when using the default behaviour it is then added as a keylistener to the system
	  * and responds to all keypresses until it is deselected. Be sure to implement this behaviour
	  * using a widgetlistener and the activate/deactivate functions if you are making a custom behaviour). 
	  **/
	class EditBox : 
//		public Widget, CharInputListener, KeyListener {
	public Label, CharInputListener, KeyListener {
	public:
		/** Enumeration for modes **/
		enum InputMode {
			IM_NUMBERS,
			IM_STANDARD
		};

		EditBox(int x, int y, int width, int height, Widget* parent);

		/** Constructor.
		  * \param x the horizontal position of the Label relative to its parent's top left padded corner.
		  * \param y the vertical position of the Label relative to its parent's top left padded corner
		  * \param width the width of the Label.
		  * \param height the height of the Label.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Label to be added to the parent's children.
		  * \param text dictates the initial content. 
		  * \param backColor controls the background color. 
			* \param font the font used to draw the text in this widget. If NULL, the default font is used. See Engine::getDefaultFont().
		  * \param manageNavigation determines whether the EditBox should automatically respond to key presses when it is selected in a default fashion. 
		  * \param multiLine controls whether multiple lines of text are supported. 
		  * \param maxLength controls the maximum number of characters that can be entered.
			* \param inputMode the input mode.
		  **/
		EditBox(int x, int y, int width, int height, Widget* parent, const String &text="",
			int backColor=0, Font* font=NULL, bool manageNavigation=true, bool multiLine=false,
			int maxLength=64, InputMode inputMode=IM_STANDARD);

		/** Sets the color of the cursor **/
		void setCursorColor(int color);
		/** Returns the color used to draw the cursor. **/
		int getCursorColor() const;
		/** Turns automatic managment of navigation on or off **/
		void setManageNavigation(bool b=true);

		/** Turns multiline mode on or off **/
		//void setMultiLine(bool b=true);
		/** Returns whether multiline is enabled or not **/
		//bool isMultiLine();

		/** Returns whether navigation is being managed **/
		bool isManagingNavigation();
		/** Sets the maximum number of characters allowed **/
		void setMaxLength(int maxLength);
		/** Returns the maximum number of characters allowed **/
		int getMaxLength();
		/** Returns whether the EditBox is selected **/
		void setSelected(bool selected=true);
		/** Registers the EditBox as a KeyListener with the current Environment, updating the content in response to key presses **/ 
		void activate();
		/** Unregisters the KeyListener used to update the content **/
		void deactivate();
		/** Returns whether the EditBox is listening for input **/
		bool isActive() const;
	
		/** Implementation of the KeyListener interface, updating the content **/
		void keyPressEvent(int keyCode);
		/** Implementation of the KeyListener interface, updating the content **/
		void keyReleaseEvent(int keyCode);
		/** Implementation of the CharInputListener interface function **/
		void characterChanged(char c);
		/** Implementation of the CharInputListener interface function **/
		void characterDeployed(char c);
		/** Moves the cursor horizontally the specified number of steps. Negative steps go left, positive right. **/
		bool moveCursorHorizontal(int steps, bool fireListeners=true);
		/** Removes the character at the current cursor position. **/ 
		bool deleteCurrentCharacter();
		/** Removes the character before the current cursor position. **/
		bool deletePreviousCharacter();
		/** Clears the text. **/ 
		void clearText();

		virtual void setCaption(const String& caption);
	
		/** Sets the text. **/
		void setText(const String& text);
		/** Returns the text. **/
		const String& getText() const;
		/** Selects the input mode. **/
		void setInputMode(InputMode inputMode);
		/** Returns the input mode **/
		InputMode getInputMode() const;

		void setEnabled(bool enabled = true);

		void addEditBoxListener(EditBoxListener *ebl);

		void setCaseToggle(bool caseToggle=true);

		/** Sets password mode.
		* Finished characters will be rendered only as asterisks (*).
		**/
		void setPasswordMode(bool enabled = true);

		
	private:
		bool deleteCharacter(int c);

		void fireTextChanged();
		void fireCursorMoved(int lastCursor);
		void fireCharacterDeleted(char c);
		void fireCharacterAdded(char c);
		void fireCharacterChanged(char c);

		void drawWidget();

		//Font* font;
		//String text;
		int cursorIndex;
		bool manageNavigation;
	
		bool active;
		int cursorColor;
		bool characterInputActive;

		//bool multiLine;
		int currentLine;
		//int currentIndex;

		int maxLength;

		InputMode inputMode;

		Vector<EditBoxListener*> editBoxListeners;

		bool caseToggle;

		bool passwordMode;
		String password;
	};
}

#endif	//_SE_MSAB_MAUI_EDITBOX_H_

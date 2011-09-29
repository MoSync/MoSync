/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file OptionsDialog.h
 * @author Bogdan Iusco.
 *
 * \brief Use the OptionsDialog class to present the user a set of alternatives
 * for how to proceed with a given task. You can also use this class to prompt
 * the user to confirm a potentially dangerous action.
 * The OptionsDialog contains an optional title and one or more buttons, each
 * corresponds to an action to take.
 * OptionsDialog class uses singleton pattern so make sure you reset dialog's
 * content each time before you use it.
 */

#ifndef NATIVEUI_OPTIONS_DIALOG_H_
#define NATIVEUI_OPTIONS_DIALOG_H_

#include <IX_WIDGET.h>
#include <MAUtil/String.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Vector.h>

//#include "OptionsBoxListener.h"

	class OptionsBoxListener;

	/**
	 * \brief Use the OptionsDialog class to present the user a set of
	 * alternatives for how to proceed with a given task. You can also use this
	 * class to prompt the user to confirm a potentially dangerous action.
	 * The OptionsDialog contains an optional title and one or more buttons,
	 * each corresponds to an action to take.
	 * OptionsDialog class uses singleton pattern so make sure you reset
	 * dialog's content each time before you use it.
	 */
	class OptionsBox: public MAUtil::CustomEventListener
	{
	public:
		/**
		 * Get the singleton instance.
		 * @return The shared singleton instance.
		 */
		static OptionsBox* getInstance();

		/**
		 * Destroy the single instance of this class.
		 * Call this method only when the application will exit.
		 */
		static void destroyInstance();

		/**
		 * Display the options dialog.
		 */
		virtual void show();

		/**
		 * Set dialog's title.
		 * @param title A string to display in the title area of the dialog.
		 */
		virtual void setTitle(const MAUtil::WString& title);

		/**
		 * Get dialog's title.
		 * @return The string displayed in the title area of the dialog.
		 */
		virtual MAUtil::WString getTitle() const;

		/**
		 * Set the title of the destructive button.
		 * The button is added automatically on the top of the options dialog.
		 * On iOS platform the is displayed in red to indicate that it
		 * represents a destructive behavior.
		 * On Android platform the button will look the same as the other
		 * buttons.
		 * @param buttonTitle The given button title.
		 * If the param is an empty string the button will not be shown.
		 */
		virtual void setDestructiveButtonTitle(
				const MAUtil::WString& buttonTitle);

		/**
		 * Show or hide the destructive button.
		 * @param visible If true destructive button will be shown, otherwise
		 * it will be hidden.
		 */
		virtual void setDestructiveButtonVisible(const bool visible);

		/**
		 * Check if destructive button is visible.
		 * @return true if it's visible, false otherwise.
		 */
		virtual bool isDestructiveButtonVisible() const;

		/**
		 * Set the title of the cancel button.
		 * The button is added automatically on the bottom of the options dialog.
		 * On iOS platform the is displayed in black to indicate that it
		 * represents a cancel action.
		 * On Android platform the button will look the same as the other
		 * buttons.
		 * @param buttonTitle The given button title.
		 * If the param is an empty string the button will not be shown.
		 */
		virtual void setCancelButtonTitle(const MAUtil::WString& buttonTitle);

		/**
		 * Show or hide the cancel button.
		 * @param visible If true cancel button will be shown, otherwise
		 * it will be hidden.
		 */
		virtual void setCancelButtonVisible(const bool visible);

		/**
		 * Check if cancel button is visible.
		 * @return true if it's visible, false otherwise.
		 */
		virtual bool isCancelButtonVisible() const;

		/**
		 * Add a button to options dialog.
		 * @param buttonTitle The title of the button.
		 * If the param is an empty string the button will not be shown.
		 */
		virtual void addButton(const MAUtil::WString& buttonTitle);

		/**
		 * Count the number of buttons shown in options dialog.
		 * The destructive and cancel button are not counted.
		 * @return The number of buttons.
		 */
		virtual int countButtons() const;

		/**
		 * Insert a button at a given index, moving all existing elements
		 * beginning at index one step forward.
		 * @param buttonTitle Button's title.
		 * @param index A position where the button will be inserted.
		 * Must be >= 0 and	 < countButtons().
		 * @return MAW_RES_OK if the button was inserted, or
		 * MAW_RES_INVALID_PROPERTY_VALUE if the index param is invalid.
		 */
		virtual int insertButton(const MAUtil::WString& buttonTitle,
				const int index);

		/**
		 * Remove the button at index.
		 * @param index The index of the button that should be removed.
		 * Must be >= 0 and < countButtons().
		 * @return MAW_RES_OK if the button was removed, or
		 * MAW_RES_INVALID_PROPERTY_VALUE if the index param is invalid.
		 */
		virtual int removeButton(const int index);

		/**
		 * Remove all buttons.
		 * The destructive and cancel buttons are not removed.
		 * Use setDestructiveButtonVisible() and setCancelButtonVisible()
		 * methods to hide the buttons.
		 */
		virtual void removeAllButtons();

		/**
		 * Remove all the buttons.
		 * The destructive and cancel button are removed too.
		 */
		virtual void resetDialog();

		/**
		 * Add an options dialog event listener.
		 * @param listener The listener that will receive options dialog events.
		 */
		virtual void addOptionsBoxListener(
				OptionsBoxListener* listener);

		/**
		 * Remove an options dialog even listener.
		 * @param listener The listener that receives options dialog even events.
		 */
		virtual void removeOptionsBoxListener(
				OptionsBoxListener* listener);

		/**
		 * Implementation of CustomEventListener interface.
		 * This method will get called whenever there is a
		 * widget event generated by the system.
		 * @param event The new received event.
		 */
		virtual void customEvent(const MAEvent& event);

	protected:
		/**
		 * Constructor is protected since this is a singleton.
		 */
		OptionsBox();

		/**
		 * Destructor.
		 */
		virtual ~OptionsBox();

		/**
		 * Deletes all strings(button titles) from vector.
		 */
		void deleteAllButtons();

	private:
		/**
		 * The single instance of this class.
		 */
		static OptionsBox* sInstance;

		/**
		 * Array with listeners.
		 */
		MAUtil::Vector<OptionsBoxListener*> mListeners;

		/**
		 * The title of the dialog.
		 */
		MAUtil::WString* mDialogTitle;

		/**
		 * The title of the destructive button.
		 */
		MAUtil::WString* mDestructiveButtonTitle;

		/**
		 * The title of the cancel button.
		 */
		MAUtil::WString* mCancelButtonTitle;

		/**
		 * The titles of any additional buttons that user wants to add.
		 */
		MAUtil::Vector<MAUtil::WString*> mOtherButtonTitles;

		/**
		 * If true the destructive button will be shown.
		 */
		bool mIsDestructiveButtonVisible;

		/**
		 * If true the cancel button will be shown.
		 */
		bool mIsCancelButtonVisible;
	};


#endif /* NATIVEUI_OPTIONS_DIALOG_H_ */

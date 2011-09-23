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
 * @file OptionsDialog.cpp
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

#include "OptionsDialog.h"

#include <IX_WIDGET.h>
#include <maapi.h>

#include "WidgetUtil.h"

namespace NativeUI
{
	/**
	 * Initialize the singleton variable to NULL.
	 */
	OptionsDialog* OptionsDialog::sInstance = NULL;

	/**
	 * Constructor.
	 */
	OptionsDialog::OptionsDialog()
	{
		mDialogTitle = new MAUtil::WString();
		mDestructiveButtonTitle = new MAUtil::WString();
		mCancelButtonTitle = new MAUtil::WString();

		// Add me as a custom event listener.
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);
	}

	/**
	 * Destructor.
	 */
	OptionsDialog::~OptionsDialog()
	{
		delete mDialogTitle;
		delete mDestructiveButtonTitle;
		delete mCancelButtonTitle;
		this->deleteAllButtons();

		// Remove me as a custom event listener.
		MAUtil::Environment::getEnvironment().removeCustomEventListener(this);
	}

	/**
	 * Get the singleton instance.
	 * @return The shared singleton instance.
	 */
	OptionsDialog* OptionsDialog::getInstance()
	{
		if (NULL == OptionsDialog::sInstance)
		{
			OptionsDialog::sInstance = new OptionsDialog();
		}

		return sInstance;
	}

	/**
	 * Destroy the single instance of this class.
	 * Call this method only when the application will exit.
	 */
	void OptionsDialog::destroyInstance()
	{
		delete OptionsDialog::sInstance;
	}

	/**
	 * Display the options dialog.
	 */
	void OptionsDialog::show()
	{
		MAUtil::WString destructiveBtn = L"";
		if (mIsDestructiveButtonVisible)
		{
			destructiveBtn = *mDestructiveButtonTitle;
		}

		MAUtil::WString canceBtn = L"";
		if (mIsCancelButtonVisible)
		{
			canceBtn = *mCancelButtonTitle;
		}

		// Write other button titles into memory.
		// First calculate how much memory is needed.
		int countBytes = sizeof(int);
		int countStrings = mOtherButtonTitles.size();
		for (int stringIndex = 0; stringIndex < countStrings; stringIndex++)
		{
			MAUtil::WString* string = mOtherButtonTitles[stringIndex];
			// Add the size(in bytes) of the string and the null character
			// (needed for separating strings).
			countBytes += (string->length() * sizeof(wchar)) + sizeof(wchar);
		}

		char* buffer = new char[countBytes];
		*(int*) buffer = countStrings;

		wchar_t* dst = (wchar_t*) (buffer + sizeof(int));
		for (int stringIndex = 0; stringIndex < countStrings; stringIndex++)
		{
			MAUtil::WString* string = mOtherButtonTitles[stringIndex];
			const wchar_t* array = string->pointer();
			while (true)
			{
				*dst = *array;
				dst++;
				if (*array == 0)
				{
					break;
				}
				array++;
			}
		}

		MAAddress memoryAddress = buffer;
		maWidgetShowOptionsDialog(
				mDialogTitle->pointer(),
				destructiveBtn.pointer(),
				canceBtn.pointer(),
				memoryAddress,
				countBytes);

		delete[] buffer;
	}

	/**
	 * Set dialog's title.
	 * @param title A string to display in the title area of the dialog.
	 */
	void OptionsDialog::setTitle(const MAUtil::WString& title)
	{
		*mDialogTitle = title;
	}

	/**
	 * Get dialog's title.
	 * @return The string displayed in the title area of the dialog.
	 */
	MAUtil::WString OptionsDialog::getTitle() const
	{
		return *mDialogTitle;
	}

	/**
	 * Set the title of the destructive button.
	 * The button is added automatically on the top of the options dialog.
	 * On iOS platform the is displayed in red to indicate that it
	 * represents a destructive behavior.
	 * On Android platform the button will look the same as the other
	 * buttons.
	 * @param buttonTitle The given button title.
	 * If the param is an empty string the button will not be added.
	 */
	void OptionsDialog::setDestructiveButtonTitle(
			const MAUtil::WString& buttonTitle)
	{
		*mDestructiveButtonTitle = buttonTitle;
	}

	/**
	 * Show or hide the destructive button.
	 * @param visible If true destructive button will be shown, otherwise
	 * it will be hidden.
	 */
	void OptionsDialog::setDestructiveButtonVisible(const bool visible)
	{
		mIsDestructiveButtonVisible = visible;
	}

	/**
	 * Check if destructive button is visible.
	 * @return true if it's visible, false otherwise.
	 */
	bool OptionsDialog::isDestructiveButtonVisible() const
	{
		return mIsDestructiveButtonVisible;
	}

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
	void OptionsDialog::setCancelButtonTitle(const MAUtil::WString& buttonTitle)
	{
		*mCancelButtonTitle = buttonTitle;
	}

	/**
	 * Show or hide the cancel button.
	 * @param visible If true cancel button will be shown, otherwise
	 * it will be hidden.
	 */
	void OptionsDialog::setCancelButtonVisible(const bool visible)
	{
		mIsCancelButtonVisible = visible;
	}

	/**
	 * Check if cancel button is visible.
	 * @return true if it's visible, false otherwise.
	 */
	bool OptionsDialog::isCancelButtonVisible() const
	{
		return mIsCancelButtonVisible;
	}

	/**
	 * Add a button to options dialog.
	 * @param buttonTitle The title of the button.
	 * If the param is an empty string the button will not be shown.
	 */
	void OptionsDialog::addButton(const MAUtil::WString& buttonTitle)
	{
		if (0 != buttonTitle.size())
		{
			MAUtil::WString* string = new MAUtil::WString();
			*string = buttonTitle;
			mOtherButtonTitles.add(string);
		}

	}

	/**
	 * Count the number of buttons shown in options dialog.
	 * The destructive and cancel button are not counted.
	 * @return The number of buttons.
	 */
	int OptionsDialog::countButtons() const
	{
		return mOtherButtonTitles.size();
	}

	/**
	 * Insert a button at a given index, moving all existing elements
	 * beginning at index one step forward.
	 * @param buttonTitle Button's title.
	 * @param index A position where the button will be inserted.
	 * Must be >= 0 and	 < countButtons().
	 * @return MAW_RES_OK if the button was inserted, or
	 * MAW_RES_INVALID_PROPERTY_VALUE if the index param is invalid.
	 */
	 int OptionsDialog::insertButton(const MAUtil::WString& buttonTitle,
			const int index)
	{
		 if (0 <= index && index < mOtherButtonTitles.size())
		 {
			 MAUtil::WString* string = new MAUtil::WString();
			 *string = buttonTitle;
			 mOtherButtonTitles.insert(index, string);
			 return MAW_RES_OK;
		 }
		 else
		 {
			 return MAW_RES_INVALID_PROPERTY_VALUE;
		 }
	}

	/**
	 * Remove the button at index.
	 * @param index The index of the button that should be removed.
	 * Must be >= 0 and < countButtons().
	 * @return MAW_RES_OK if the button was removed, or
	 * MAW_RES_INVALID_PROPERTY_VALUE if the index param is invalid.
	 */
	int OptionsDialog::removeButton(const int index)
	{
		 if (0 <= index && index < mOtherButtonTitles.size())
		 {
			 MAUtil::WString* string = mOtherButtonTitles[index];
			 delete string;
			 mOtherButtonTitles.remove(index);
			 return MAW_RES_OK;
		 }
		 else
		 {
			 return MAW_RES_INVALID_PROPERTY_VALUE;
		 }
	}

	/**
	 * Remove all buttons.
	 * The destructive and cancel buttons are not removed.
	 * Use setDestructiveButtonVisible() and setCancelButtonVisible()
	 * methods to hide the buttons.
	 */
	void OptionsDialog::removeAllButtons()
	{
		this->deleteAllButtons();
	}

	/**
	 * Remove all the buttons.
	 * The destructive and cancel button are removed too.
	 */
	void OptionsDialog::resetDialog()
	{
		*mDialogTitle = L"";

		*mDestructiveButtonTitle = L"";
		mIsDestructiveButtonVisible = false;

		*mCancelButtonTitle = L"";
		mIsCancelButtonVisible = false;

		this->deleteAllButtons();
	}

	/**
	 * Add an options dialog event listener.
	 * @param listener The listener that will receive options dialog events.
	 */
	void OptionsDialog::addOptionsDialogListener(
			OptionsDialogListener* listener)
	{
		mListeners.add(listener);
	}

	/**
	 * Remove an options dialog even listener.
	 * @param listener The listener that receives options dialog even events.
	 */
	void OptionsDialog::removeOptionsDialogListener(
			OptionsDialogListener* listener)
	{
		removeListenerFromVector(mListeners, listener);
	}

	/**
	 * Implementation of CustomEventListener interface.
	 * This method will get called whenever there is a
	 * widget event generated by the system.
	 * @param event The new received event.
	 */
	void OptionsDialog::customEvent(const MAEvent& event)
	{
		// Check if this is a widget event.
		if (EVENT_TYPE_WIDGET == event.type)
		{
			// Get the widget event data structure.
			MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;
			if (MAW_EVENT_OPTION_DIALOG_BUTTON_CLICKED == eventData->eventType)
			{
				// Check if the destructive button was pressed.
				int buttonIndex = eventData->optionDialogButtonIndex;
				if (mIsDestructiveButtonVisible &&
						0 == buttonIndex)
				{
					for (int listenerIndex = 0;
							listenerIndex < mListeners.size();
							listenerIndex++)
					{
						mListeners[listenerIndex]->optionsDialogDestructiveButtonClicked();
					}
					return;
				}

				// Check if the cancel button was pressed.
				int cancelButtonIndex = 0;
				if (mIsDestructiveButtonVisible)
				{
					cancelButtonIndex++;
				}
				cancelButtonIndex += mOtherButtonTitles.size();
				if (mIsCancelButtonVisible &&
						cancelButtonIndex == buttonIndex)
				{
					for (int listenerIndex = 0;
							listenerIndex < mListeners.size();
							listenerIndex++)
					{
						mListeners[listenerIndex]->optionsDialogCancelButtonClicked();
					}
					return;
				}

				// Other button was pressed.
				if (mIsDestructiveButtonVisible)
				{
					buttonIndex--;
				}
				MAUtil::WString* buttonTitle = mOtherButtonTitles[buttonIndex];
				for (int listenerIndex = 0;
					listenerIndex < mListeners.size();
					listenerIndex++)
				{
					mListeners[listenerIndex]->optionsDialogButtonClicked(
							buttonIndex,
							*buttonTitle);
				}

			}
		}
	}

	/**
	 * Deletes all strings(button titles) from vector.
	 */
	void OptionsDialog::deleteAllButtons()
	{
		for (int stringIndex = 0; stringIndex < mOtherButtonTitles.size();
				stringIndex++)
		{
			MAUtil::WString* string = mOtherButtonTitles[stringIndex];
			delete string;
		}
		mOtherButtonTitles.clear();
	}
} // namespace NativeUI

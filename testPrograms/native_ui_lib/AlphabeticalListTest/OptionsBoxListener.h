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
 * @file OptionsDialogListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for OptionsDialog events.
 */

#ifndef NATIVEUI_OPTIONS_BOX_LISTENER_H_
#define NATIVEUI_OPTIONS_BOX_LISTENER_H_

#include <MAUtil/String.h>

	/**
	 * \brief Listener for OptionsDialog events.
	 */
	class OptionsBoxListener
	{
	public:
		/**
		 * This method is called when the destructive button from options
		 * dialog was clicked.
		 */
		virtual void optionsBoxDestructiveButtonClicked()
		{
		}
		;

		/**
		 * This method is called when the cancel button from options dialog was
		 * clicked.
		 */
		virtual void optionsBoxCancelButtonClicked()
		{
		}
		;

		/**
		 * This method is called when a button from options dialog was
		 * clicked.
		 * This method is not called if the destructive or cancel button were
		 * clicked.
		 * @param buttonIndex The index of the button that was clicked.
		 * @param buttonTitle The title of the button that was clicked.
		 */
		virtual void optionsBoxButtonClicked(const int buttonIndex,
			const MAUtil::WString& buttonTitle)
		{
		}
		;
	};

#endif /* NATIVEUI_OPTIONS_BOX_LISTENER_H_ */

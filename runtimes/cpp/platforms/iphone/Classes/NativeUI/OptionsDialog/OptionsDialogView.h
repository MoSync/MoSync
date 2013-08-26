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
 * @file OptionsDialogView.h
 *
 * \brief Use the OptionsDialog class to present the user a set of alternatives
 * for how to proceed with a given task. You can also use this class to prompt
 * the user to confirm a potentially dangerous action.
 * The OptionsDialog contains an optional title and one or more buttons, each
 * corresponds to an action to take.
 * OptionsDialog class uses singleton pattern so make sure you reset dialog's
 * content each time before you use it.
 */

#ifndef _MOSYNC_UI_OPTION_DIALOG_
#define _MOSYNC_UI_OPTION_DIALOG_

#import <Foundation/Foundation.h>
#include <helpers/cpp_defs.h>

@interface OptionsDialogView :  NSObject<UIActionSheetDelegate> {

    NSString* mDialogTitle;

    NSString* mDestructiveButtonTitle;

    NSString* mCancelButtonTitle;

    NSMutableArray* mOtherButtonTitles;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(OptionsDialogView*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Shows the image picker to the user.
 */
-(void) show:(const wchar*) title
destructiveButtonTitle:(const wchar* ) destructiveButtonTitle
cancelButtonTitle:(const wchar*) cancelButtonTitle
otherButtonTitles:(const void*) otherButtonTitles
otherButtonTitlesSize:(const int) otherButtonTitlesSize;


/**
 * Read an array of string from a given address for a specified field.
 * Strings will be stored in mOtherButtonTitles array.
 * @param address The specified address.
 *                The address must have the following structure:
 *                     - the first element must be a 4-byte int that specifies
 *                       the number of strings that can be read.
 *                     - first null terminated string(UTF-16 encoding).
 *                     - second null terminated string(UTF-16 encoding).
 *                     - etc
 * @param size The size of the buffer buffer from where the string will be read.
 */
-(void) readStringArray:(const void*) address
               withSize:(const int) size;

@end

#endif
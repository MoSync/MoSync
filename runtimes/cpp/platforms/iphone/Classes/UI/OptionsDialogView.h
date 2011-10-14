/* Copyright (C) 2011 Mobile Sorcery AB

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
//
//  OptionDialogView.h
//  MoSync
//
//  Created by Bogdan Iusco on 9/20/11.
//  Copyright 2011 MoSync. All rights reserved.
//

#ifndef _MOSYNC_UI_OPTION_DIALOG_
#define _MOSYNC_UI_OPTION_DIALOG_

#import <Foundation/Foundation.h>
#include <helpers/cpp_defs.h>

@interface OptionDialogView :  NSObject<UIActionSheetDelegate> {

    NSString* mDialogTitle;

    NSString* mDestructiveButtonTitle;

    NSString* mCancelButtonTitle;

    NSMutableArray* mOtherButtonTitles;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(OptionDialogView*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Shows the image picker to the user.
 */
-(void) show:(const char*) title
destructiveButtonTitle:(const char* ) destructiveButtonTitle
cancelButtonTitle:(const char*) cancelButtonTitle
otherButtonTitles:(const void*) otherButtonTitles;


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
 */
-(void) readStringArray:(const void*) address;

@end

#endif
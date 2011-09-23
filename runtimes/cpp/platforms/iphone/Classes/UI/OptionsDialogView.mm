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
 * @file OptionsDialogView.cpp
 *
 * \brief Use the OptionsDialog class to present the user a set of alternatives
 * for how to proceed with a given task. You can also use this class to prompt
 * the user to confirm a potentially dangerous action.
 * The OptionsDialog contains an optional title and one or more buttons, each
 * corresponds to an action to take.
 * OptionsDialog class uses singleton pattern so make sure you reset dialog's
 * content each time before you use it.
 */

#import "OptionsDialogView.h"
#import "MoSyncUISyscalls.h"
#import "MoSyncUI.h"
#import "ScreenWidget.h"
#import <cwchar>

static OptionsDialogView *sharedInstance = nil;

@implementation OptionsDialogView

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(OptionsDialogView*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[OptionsDialogView alloc] init];
    }

    return sharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sharedInstance release];
}

- (id)init
{
    mOtherButtonTitles = [[NSMutableArray alloc] init];
    return [super init];
}

/**
 * Shows the image picker to the user.
 */
-(void) show:(const wchar*) title
destructiveButtonTitle:(const wchar* ) destructiveButtonTitle
cancelButtonTitle:(const wchar*) cancelButtonTitle
otherButtonTitles:(const void*) otherButtonTitles
otherButtonTitlesSize:(const int) otherButtonTitlesSize
{
    [mDialogTitle release];
    [mDestructiveButtonTitle release];
    [mCancelButtonTitle release];

    int titleLength = wcslen((wchar_t*) title) * sizeof(wchar_t);
    if (0 == titleLength)
    {
        mDialogTitle = nil;
    }
    else
    {
        NSString* titleString = [[NSString alloc] initWithBytes:title
                                                         length:titleLength
                                                       encoding:NSUTF16LittleEndianStringEncoding];
        mDialogTitle = titleString;
        [mDialogTitle retain];
        [titleString release];
    }

    int destructiveBtnTitleLength = wcslen((wchar_t*) destructiveButtonTitle) * sizeof(wchar_t);
    if (0 == destructiveBtnTitleLength)
    {
        mDestructiveButtonTitle = nil;
    }
    else
    {
        NSString* destructiveButtonTitleString = [[NSString alloc] initWithBytes:destructiveButtonTitle
                                                                          length:destructiveBtnTitleLength
                                                                        encoding:NSUTF16LittleEndianStringEncoding];
        mDestructiveButtonTitle = destructiveButtonTitleString;
        [mDestructiveButtonTitle retain];
        [destructiveButtonTitleString release];
    }

    int cancelBtnTitleLength = wcslen((wchar_t*) cancelButtonTitle) * sizeof(wchar_t);
    if (0 == cancelBtnTitleLength)
    {
        mCancelButtonTitle = nil;
    }
    else
    {
        NSString* cancelButtonTitleString = [[NSString alloc] initWithBytes:cancelButtonTitle
                                                                     length:cancelBtnTitleLength
                                                                   encoding:NSUTF16LittleEndianStringEncoding];
        mCancelButtonTitle = cancelButtonTitleString;
        [mCancelButtonTitle retain];
        [cancelButtonTitleString release];
    }

    [self readStringArray:otherButtonTitles withSize:otherButtonTitlesSize];
    [self performSelectorOnMainThread:@selector(showOptionDialog) withObject:nil waitUntilDone:NO];
}

-(void) showOptionDialog
{
    MoSyncUI* mosyncUI = getMoSyncUI();
    ScreenWidget* shownScreen = (ScreenWidget*)[mosyncUI getCurrentlyShownScreen];
    UIViewController* controller = [shownScreen getController];
    UIView* view = [controller view];
    UIActionSheet *actionSheet = [[UIActionSheet alloc]
                                  initWithTitle:mDialogTitle
                                  delegate:self
                                  cancelButtonTitle:nil
                                  destructiveButtonTitle: nil
                                  otherButtonTitles:nil
								  ];

    // Add buttons to action sheet.
    int currentButtonIndex = 0;
    // Add destructive  button
    if (mDestructiveButtonTitle)
    {
        [actionSheet addButtonWithTitle:mDestructiveButtonTitle];
        [actionSheet setDestructiveButtonIndex:currentButtonIndex];
        currentButtonIndex++;
    }

    // Add other buttons to action sheet.
    for (int buttonIndex = 0; buttonIndex < [mOtherButtonTitles count]; buttonIndex++)
    {
        NSString* otherButtonTitleString = [mOtherButtonTitles objectAtIndex:buttonIndex];
        [actionSheet addButtonWithTitle:otherButtonTitleString];
        currentButtonIndex++;
    }

    // Add cancel button to action sheet.
    if (mCancelButtonTitle)
    {
        [actionSheet addButtonWithTitle:mCancelButtonTitle];
        [actionSheet setCancelButtonIndex:currentButtonIndex];
    }

    // Show and release action sheet.
	[actionSheet showInView:view];
	[actionSheet release];
}

-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_OPTION_DIALOG_BUTTON_CLICKED;
    eventData->optionDialogButtonIndex = buttonIndex;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

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
               withSize:(const int) size
{
    [mOtherButtonTitles removeAllObjects];
    int noStrings = *(int*) address;
    int bytesRead = sizeof(int);
    bool invalidBuffer = false;

    UInt16* src = (UInt16*) ((int)address + sizeof(int));
    for (int i = 0; ((i < noStrings) && !invalidBuffer); i++)
    {
        UInt16* stringSrc = src;

        // Calculate the length of the string.
        int stringLength = 0;
        bool condition = true;
        while (condition)
        {
            if (0 == *src)
            {
                src++;
                condition = false;
            }
            else
            {
                src++;
                stringLength += sizeof(UInt16);
            }

            // Check if there are more bytes that can be read.
            bytesRead += sizeof(UInt16);
            if (bytesRead > size)
            {
                invalidBuffer = true;
                condition = false;
            }
        }

        if (!invalidBuffer)
        {
            // Create string with UTF-16 encoding.
            NSString* string = [[NSString alloc] initWithBytes:stringSrc length:stringLength
                                                      encoding:NSUTF16LittleEndianStringEncoding];
            [mOtherButtonTitles addObject:string];
            [string release];
        }
    }
}

-(void) dealloc
{
    [mDialogTitle release];
    [mDestructiveButtonTitle release];
    [mCancelButtonTitle release];
    [mOtherButtonTitles release];

    [super dealloc];
}

@end

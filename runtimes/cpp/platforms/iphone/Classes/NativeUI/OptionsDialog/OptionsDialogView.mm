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
#import "NSStringExpanded.h"

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

    mDialogTitle = [[NSString stringWithUTF16String:title] retain];
    if (mDialogTitle.length == 0)
    {
        [mDialogTitle release];
        mDialogTitle = nil;
    }

    mDestructiveButtonTitle = [[NSString stringWithUTF16String:destructiveButtonTitle] retain];
    if (mDestructiveButtonTitle.length == 0)
    {
        [mDestructiveButtonTitle release];
        mDestructiveButtonTitle = nil;
    }

    mCancelButtonTitle = [[NSString stringWithUTF16String:cancelButtonTitle] retain];
    if (mCancelButtonTitle.length == 0)
    {
        [mCancelButtonTitle release];
        mCancelButtonTitle = nil;
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
	event.type = EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED;
    event.optionsBoxButtonIndex = buttonIndex;
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

//
//  OptionDialogView.m
//  MoSync
//
//  Created by Bogdan Iusco on 9/20/11.
//  Copyright 2011 MoSync. All rights reserved.
//

#import "OptionDialogView.h"
#import "MoSyncUISyscalls.h"
#import "MoSyncUI.h"
#import "ScreenWidget.h"

static OptionDialogView *sharedInstance = nil;

@implementation OptionDialogView

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(OptionDialogView*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[OptionDialogView alloc] init];
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
-(void) show:(const char*) title
destructiveButtonTitle:(const char* ) destructiveButtonTitle
cancelButtonTitle:(const char*) cancelButtonTitle
otherButtonTitles:(const void*) otherButtonTitles
{
    NSLog(@"Show");
    [mDialogTitle release];
    [mDestructiveButtonTitle release];
    [mCancelButtonTitle release];

    if (0 == strlen(title))
    {
        mDialogTitle = nil;
    }
    else
    {
        NSString* titleString = [NSString stringWithUTF8String:title];
        mDialogTitle = titleString;
        [mDialogTitle retain];
    }

    if (0 == strlen(destructiveButtonTitle))
    {
        mDestructiveButtonTitle = nil;
    }
    else
    {
        NSString* destructiveButtonTitleString = [NSString stringWithUTF8String:destructiveButtonTitle];
        mDestructiveButtonTitle = destructiveButtonTitleString;
        [mDestructiveButtonTitle retain];
    }

    if (0 == strlen(cancelButtonTitle))
    {
        mDialogTitle = nil;
    }
    else
    {
        NSString* cancelButtonTitleString = [NSString stringWithUTF8String:cancelButtonTitle];
        mCancelButtonTitle = cancelButtonTitleString;
        [mCancelButtonTitle retain];
    }
    [self readStringArray:otherButtonTitles];
    [self performSelectorOnMainThread:@selector(showOptionDialog) withObject:nil waitUntilDone:NO];
}

-(void) showOptionDialog
{
    MoSyncUI* mosyncUI = getMoSyncUI();
    ScreenWidget* shownScreen = (ScreenWidget*)[mosyncUI getCurrentlyShownScreen];
    UIViewController* controller = [shownScreen getController];
    UIView* view = [controller view];
    UIActionSheet *actionSheet = [[UIActionSheet alloc]
                                  initWithTitle:nil
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
	NSLog(@"button %i clicked", buttonIndex );
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
 */
-(void) readStringArray:(const void*) address
{

    [mOtherButtonTitles removeAllObjects];
    int noStrings = *(int*) address;

    UInt16* src = (UInt16*) ((int)address + sizeof(int));
    for (int i = 0; i < noStrings; i++)
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
        }

        if (0 != stringLength)
        {
            // Create string with UTF-16 encoding.
            NSString* buttonName = [[NSString alloc] initWithBytes:stringSrc length:stringLength
                                                      encoding:NSUTF16LittleEndianStringEncoding];
            [mOtherButtonTitles addObject:buttonName];
            [buttonName release];
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

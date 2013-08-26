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
 * @file MoSyncUIAlertView.mm
 * @author Bogdan Iusco
 *
 * @brief Shown an alert view on the screen.
 * Used by the maAlert and maMessageBox syscalls.
 */

#import "MoSyncUIAlertView.h"
#import "Platform.h"

@interface MoSyncUIAlertView ()

/**
 * Holds button titles for next shown alert view.
 * When an alert view is shown using showAlertView method, all its containing
 * items will be removed.
 * Should contain NSString type objects.
 */
@property(nonatomic, retain) NSMutableArray* buttonTitles;

/**
 * When an alert is shown on the screen, the object is added to this array.
 * When the dialog is dissmised, the object is removed from this array.
 * Should contain UIALertView type objects.
 */
@property(nonatomic, retain) NSMutableArray* alertViews;

/**
 * Create an NNString object from a char array.
 * @param stringChar Char array. If NULL or empty string the returned value
 * will be nil.
 */
+ (NSString*)stringFromChar:(const char*)stringChar;

@end

@implementation MoSyncUIAlertView

static MoSyncUIAlertView *sharedInstance = nil;

@synthesize title = _title;
@synthesize message = _message;
@synthesize buttonTitles = _buttonTitles;
@synthesize alertViews = _alertViews;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+ (MoSyncUIAlertView*)getInstance
{
	if (!sharedInstance)
    {
        sharedInstance = [[MoSyncUIAlertView alloc] init];
    }

    return sharedInstance;
}

/**
 * Releases the class instance.
 */
+ (void)deleteInstance
{
	[sharedInstance release];
	sharedInstance = nil;
}

/**
 * Add a new button for the next shown alert view(when showAlertView is called).
 * @param title Title for the new button. If it's nil or empty string it will not be added.
 */
- (void)addButtonTitle:(NSString*)title
{
	if (title && title.length > 0)
	{
		[self.buttonTitles addObject:title];
	}
}

/**
 * Show alert view with previously set attributes(such as title, message).
 * Alert view's title and message will be set to nil. Also, previously added
 * buttons will be removed.
 */
- (void)showAlertView
{
	dispatch_sync(dispatch_get_main_queue(), ^{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:self.title
														message:self.message
													   delegate:self
											  cancelButtonTitle:nil
											  otherButtonTitles:nil];
		for (NSString* buttonTitle in self.buttonTitles)
		{
			[alert addButtonWithTitle:buttonTitle];
		}
		[self.alertViews addObject:alert];
		[alert show];
		[alert release];
		alert = nil;

		[self.buttonTitles removeAllObjects];
		self.title = nil;
		self.message = nil;
	});
}

/**
 * Show alert view with given options.
 * Calling this method doesn't modify previously set title, message or any added button.
 * @param title The string that appears in the alert view's title bar.
 * @param message The text message displayed by the alert view.
 * @param button1 First button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button2 Second button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button1 Third button's title in the alert view. If nil or empty string
 * the button will not be shown.
 */
- (void)showAlertView:(NSString*)title
			  message:(NSString*)message
			  button1:(NSString*)button1
			  button2:(NSString*)button2
			  button3:(NSString*)button3
{
	dispatch_sync(dispatch_get_main_queue(), ^{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
														message:message
													   delegate:self
											  cancelButtonTitle:nil
											  otherButtonTitles:nil];

		// Add only valid strings as buttons.
		if (button1)
		{
			[alert addButtonWithTitle:button1];
		}
		if (button2)
		{
			[alert addButtonWithTitle:button2];
		}
		if (button3)
		{
			[alert addButtonWithTitle:button3];
		}

		[self.alertViews addObject:alert];
		[alert show];
		[alert release];
		alert = nil;
	});
}

/**
 * Show alert view with given options.
 * Calling this method doesn't modify previously set title, message or any added button.
 * @param titleChar The string that appears in the alert view's title bar.
 * @param messageChar The text message displayed by the alert view.
 * @param button1Char First button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button2Char Second button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button1Char Third button's title in the alert view. If nil or empty string
 * the button will not be shown.
 */
- (void)showAlertViewChar:(const char*)titleChar
			  messageChar:(const char*)messageChar
			  button1Char:(const char*)button1Char
			  button2Char:(const char*)button2Char
			  button3Char:(const char*)button3Char
{
		NSString* title   = [MoSyncUIAlertView stringFromChar:titleChar];
		NSString* message = [MoSyncUIAlertView stringFromChar:messageChar];
		NSString* button1 = [MoSyncUIAlertView stringFromChar:button1Char];
		NSString* button2 = [MoSyncUIAlertView stringFromChar:button2Char];
		NSString* button3 = [MoSyncUIAlertView stringFromChar:button3Char];

		[self showAlertView:title
					message:message
					button1:button1
					button2:button2
					button3:button3];
}

/**
 * Implemtation for buttonTitles getter.
 */
- (NSMutableArray*)buttonTitles
{
	if (!_buttonTitles)
	{
		_buttonTitles = [[NSMutableArray alloc] init];
	}
	return _buttonTitles;
}

/**
 * Implemtation for alertViews getter.
 */
- (NSMutableArray*)alertViews
{
	if (!_alertViews)
	{
		_alertViews = [[NSMutableArray alloc] init];
	}
	return _alertViews;
}

/**
 * Create an NNString object from a char array.
 * @param stringChar Char array. If NULL or empty string the returned value
 * will be nil.
 */
+ (NSString*)stringFromChar:(const char*)stringChar
{
	if (stringChar != nil && (strlen(stringChar) != 0))
	{
		return [[[NSString alloc] initWithBytes:stringChar
										 length:strlen(stringChar)
									   encoding:NSUTF8StringEncoding] autorelease];
	}
	else
	{
		return nil;
	}
}

#pragma mark UIALertViewDelegate

/**
 * Called when user clicks a button.
 * @param alertView Object containing the button clicked.
 * @param buttonIndex The index of the button that was clicked. The button indices start at 0.
 */
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	MAEvent event;
	event.type = EVENT_TYPE_ALERT;
	event.alertButtonIndex = buttonIndex + 1;
	Base::gEventQueue.put(event);

	[self.alertViews removeObject:alertView];
}

/**
 * Release contained objects.
 */
- (void)dealloc
{
	[_title release];
	[_message release];
	[_buttonTitles release];
	[_alertViews release];

	[super dealloc];
}

@end

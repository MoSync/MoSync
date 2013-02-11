/*
 Copyright (C) 2013 MoSync AB

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

#import "MoSyncUIUtils.h"

#include "Platform.h"
#include "MoSyncMain.h"

@interface MessageBoxHandler : UIViewController <UIAlertViewDelegate> {
	BOOL kill;
	NSString *title;
	NSString *msg;
	NSString *cancelTitle;
	NSString *button1Title;
	NSString *button2Title;
	NSString *button3Title;
}

@property BOOL kill;
@property (copy, nonatomic) NSString* title;
@property (copy, nonatomic) NSString* msg;
@property (copy, nonatomic) NSString* cancelTitle;
@property (copy, nonatomic) NSString* button1Title;
@property (copy, nonatomic) NSString* button2Title;
@property (copy, nonatomic) NSString* button3Title;

- (void)alertViewCancel:(UIAlertView *)alertView;
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

@implementation MessageBoxHandler

@synthesize kill;
@synthesize title;
@synthesize msg;
@synthesize cancelTitle;
@synthesize button1Title;
@synthesize button2Title;
@synthesize button3Title;

- (void)alertViewCancel:(UIAlertView *)alertView {
	// don't know if this is allowed...
	[self release];
	if(kill)
		MoSync_Exit();
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	MAEvent event;
	event.type = EVENT_TYPE_ALERT;
	event.alertButtonIndex = buttonIndex + 1;
	Base::gEventQueue.put(event);
	[self release];
}

@end

@interface TextBoxData : UIViewController <UIAlertViewDelegate> {
	NSString *title;
	NSString *inText;
	wchar* outText;
	int maxSize;
	int constraints;
	UITextField *textField;
}

@property (copy, nonatomic) NSString* title;
@property (copy, nonatomic) NSString* inText;
@property wchar* outText;
@property int maxSize;
@property int constraints;
@property (assign, nonatomic) UITextField* textField;
@end

@implementation TextBoxData

@synthesize title;
@synthesize inText;
@synthesize outText;
@synthesize maxSize;
@synthesize constraints;
@synthesize textField;

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	MAEvent event;
	event.type = EVENT_TYPE_TEXTBOX;
	if(buttonIndex == 1) {
		int length = MIN(textField.text.length, maxSize-1);
		[textField.text getCharacters:(unichar*)outText range:NSMakeRange(0, length)];
		outText[length] = '\0';
		event.textboxResult = MA_TB_RES_OK;
		event.textboxLength = length;
	} else {
		event.textboxResult = MA_TB_RES_CANCEL;
	}
	Base::gEventQueue.put(event);
}

@end

@implementation MoSyncUIUtils

+(void) showMessageBox: (NSString*)msg withTitle: (NSString*)title shouldKill: (bool)kill
{
	MessageBoxHandler *mbh = [[MessageBoxHandler alloc] retain];
	mbh.kill = kill;
	mbh.title = title;
	mbh.msg = msg;
	mbh.cancelTitle = @"OK";
	[self performSelectorOnMainThread: @selector(messageBox:) withObject:(id)mbh waitUntilDone:NO];
    // Think about using dispatch async
/*    dispatch_async(dispatch_get_main_queue(), ^{
        [MoSyncUIUtils messageBox:mbh];
    });*/
}

+(void) showAlert:(NSString*)msg
        withTitle:(NSString*)title
      button1Title:(NSString*)buton1
      button2Title:(NSString*)buton2
      button3Title:(NSString*)buton3
{
    MessageBoxHandler *mbh = [[MessageBoxHandler alloc] retain];
    mbh.kill = NO;
    mbh.title = title;
    mbh.msg = msg;
    mbh.cancelTitle = nil;
    mbh.button1Title = buton1;
    mbh.button2Title = buton2;
    mbh.button3Title = buton3;
    [self performSelectorOnMainThread: @ selector(messageBox:) withObject:(id)mbh waitUntilDone:NO];
}

+(void) showTextBox:(NSString*)title
		 withInText:(NSString*)inText
			outText:(wchar*)outText
			maxSize:(int)maxSize
	 andConstraints:(int)constraints
{
	TextBoxData *textBoxData = [TextBoxData alloc];
	textBoxData.title = title;
	textBoxData.inText = inText;
	textBoxData.outText = outText;
	textBoxData.maxSize = maxSize;
	textBoxData.constraints = constraints;

	[self performSelectorOnMainThread: @ selector(textBox:) withObject:(id)textBoxData waitUntilDone:NO];
}

+(void) messageBox:(id) obj
{
	MessageBoxHandler *mbh = (MessageBoxHandler*) obj;
	UIAlertView *alert = [[UIAlertView alloc]
                          initWithTitle:mbh.title
                          message:mbh.msg
                          delegate:mbh
                          cancelButtonTitle:mbh.cancelTitle
                          otherButtonTitles:mbh.button1Title, mbh.button2Title, mbh.button3Title, nil];

	if (mbh.cancelTitle == nil) {
		alert.cancelButtonIndex = -1;
	}
    [alert show];
    [alert release];
}

+(void) textBox:(id) obj
{
	TextBoxData *textBoxData = (TextBoxData*)obj;
	UIAlertView *textBoxAlert = [[UIAlertView alloc] initWithTitle:textBoxData.title message:@"\n\n" delegate:textBoxData cancelButtonTitle:NSLocalizedString(@"Cancel",nil) otherButtonTitles:NSLocalizedString(@"OK",nil), nil];

	UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(16,50,252,25)];
	textField.font = [UIFont systemFontOfSize:18];
	textField.backgroundColor = [UIColor whiteColor];
	textField.secureTextEntry = (textBoxData.constraints&MA_TB_FLAG_PASSWORD)?YES:NO; // password mode?
	textField.keyboardAppearance = UIKeyboardAppearanceAlert;
	textField.delegate = self;
	textField.text = textBoxData.inText;
	[textField becomeFirstResponder];
	[textBoxAlert addSubview:textField];

	textBoxData.textField = textField;

	[textBoxAlert show];
	[textBoxAlert release];
	[textField release];
}

@end

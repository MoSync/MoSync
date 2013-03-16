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

#include "ImagePickerController.h"
#include "Platform.h"
#include "MoSyncMain.h"

/*
 * C functions
 */
void MoSyncUIUtils_ShowMessageBox(const char *title, const char *msg, bool kill)
{
	NSString* nsTitle = nil;
	if(title != nil)
		nsTitle = [[NSString alloc] initWithBytes:title length:strlen(title) encoding:NSUTF8StringEncoding];

    [MoSyncUIUtils showMessageBox:[[NSString alloc] initWithBytes:msg length:strlen(msg) encoding:NSUTF8StringEncoding]
                        withTitle:nsTitle
                       shouldKill:kill];
}

void MoSyncUIUtils_ShowAlert(const char* title, const char* message, const char* button1, const char* button2, const char* button3)
{
	NSString* nsTitle = nil;
	if(title != nil && (strlen(title) != 0))
		nsTitle = [[NSString alloc] initWithBytes:title length:strlen(title) encoding:NSUTF8StringEncoding];

	NSString* nsButton1 = nil;
	if(button1 != nil && (strlen(button1) != 0))
		nsButton1 = [[NSString alloc] initWithBytes:button1 length:strlen(button1) encoding:NSUTF8StringEncoding];

	NSString* nsButton2 = nil;
	if(button2 != nil && (strlen(button2) != 0))
		nsButton2 = [[NSString alloc] initWithBytes:button2 length:strlen(button2) encoding:NSUTF8StringEncoding];

	NSString* nsButton3 = nil;
	if(button3 != nil && (strlen(button3) != 0))
		nsButton3 = [[NSString alloc] initWithBytes:button3 length:strlen(button3) encoding:NSUTF8StringEncoding];

    [MoSyncUIUtils showAlert:[[NSString alloc] initWithBytes:message length:strlen(message) encoding:NSUTF8StringEncoding]
                   withTitle:nsTitle
                button1Title:nsButton1
                button2Title:nsButton2
                button3Title:nsButton3];
}

void MoSyncUIUtils_ShowTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints) {
    [MoSyncUIUtils
	 showTextBox:[[NSString alloc] initWithCharacters:(const unichar*)title length:wcharLength(title)]
	 withInText:[[NSString alloc] initWithCharacters:(const unichar*)inText length:wcharLength(inText)]
	 outText:(wchar*)outText
	 maxSize:maxSize
	 andConstraints:constraints
	 ];
}

void MoSyncUIUtils_ShowImagePicker()
{
    [ImagePickerController getInstance].returnDataType = MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE;
    [[ImagePickerController getInstance] show];
}

void MoSyncUIUtils_ShowImagePicker(int returnType)
{
    [ImagePickerController getInstance].returnDataType = returnType;
    [[ImagePickerController getInstance] show];
}

/*
 * ObjC
 */
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
	MessageBoxHandler *mbh = [[MessageBoxHandler alloc] autorelease];
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
                          otherButtonTitles:nil];

	// Add only valid strings as buttons.
	if (mbh.button1Title)
	{
		[alert addButtonWithTitle:mbh.button1Title];
	}
	if (mbh.button2Title)
	{
		[alert addButtonWithTitle:mbh.button2Title];
	}
	if (mbh.button3Title)
	{
		[alert addButtonWithTitle:mbh.button3Title];
	}

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

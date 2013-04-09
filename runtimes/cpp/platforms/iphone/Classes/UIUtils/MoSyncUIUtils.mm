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
#include "MoSyncUIAlertView.h"
#include <base/Syscall.h>

/*
 * C functions
 */

void MoSyncUIUtils_ShowAlert(const char* title, const char* message, const char* button1, const char* button2, const char* button3)
{
	[[MoSyncUIAlertView getInstance] showAlertViewChar:title
										   messageChar:message
										   button1Char:button1
										   button2Char:button2
										   button3Char:button3];
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

int MoSyncUIUtils_SaveImageToGallery(MAHandle imageHandle)
{
    int returnCode = MA_MEDIA_RES_IMAGE_EXPORT_FAILED;

    Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
    if ( imageResource )
    {
        // Get corresponding orientation.
        UIImageOrientation orientation = UIImageOrientationUp;
		switch (imageResource->orientation)
        {
			case 1:
				orientation = UIImageOrientationUp;
				break;
			case 2:
				orientation = UIImageOrientationUpMirrored;
				break;
			case 3:
				orientation = UIImageOrientationDown;
				break;
			case 4:
				orientation = UIImageOrientationDownMirrored;
				break;
			case 5:
				orientation = UIImageOrientationLeftMirrored;
				break;
			case 6:
				orientation = UIImageOrientationRight;
				break;
			case 7:
				orientation = UIImageOrientationRightMirrored;
				break;
			case 8:
				orientation = UIImageOrientationLeft;
				break;
			default:
				break;
		}
		UIImage* image = [UIImage imageWithCGImage:imageResource->image scale:1.0 orientation:orientation];

        ImageSaveListener *sImageSaveListener = [[ImageSaveListener alloc] initWithImageHandle:imageHandle];
        UIImageWriteToSavedPhotosAlbum(image, sImageSaveListener,
                                       @selector(image: didFinishSavingWithError: contextInfo:) , nil);
        [sImageSaveListener release];

        returnCode = MA_MEDIA_RES_OK;
    }
    return returnCode;
}

// Used in image saving operation
@implementation ImageSaveListener

-(id)initWithImageHandle:(MAHandle) anImageHandle
{
    self = [super init];
    if ( self )
    {
        mImageHandle = anImageHandle;
    }
    return self;
}


-(void)dealloc
{
    [super dealloc];
}

// This is called at the end of the saving opperation.
- (void)               image: (UIImage *) image
    didFinishSavingWithError: (NSError *) error
                 contextInfo: (void *) contextInfo {
    MAEvent event;

	event.type = EVENT_TYPE_MEDIA_EXPORT_FINISHED;
    event.mediaType = MA_MEDIA_TYPE_IMAGE;
    event.mediaHandle = mImageHandle;
    event.operationResultCode = (nil == error) ? MA_MEDIA_RES_OK : MA_MEDIA_RES_IMAGE_EXPORT_FAILED;

	Base::gEventQueue.put(event);
}

@end

// TextBox data
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

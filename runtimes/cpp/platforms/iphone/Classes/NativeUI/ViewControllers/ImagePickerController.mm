/* Copyright (C) 2011 MoSync AB

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

#define TEST_RESULT(resultCode) if(0 > resultCode)\
                                {\
                                    return resultCode;\
                                }

#import "ImagePickerController.h"
#import "MoSyncUISyscalls.h"
#import "ScreenWidget.h"
#import "ScreenWidgetController.h"
#include "Platform.h"
#include "Syscall.h"

static ImagePickerController *sharedInstance = nil;

@implementation ImagePickerController

@synthesize returnDataType = _returnDataType;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(ImagePickerController*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[ImagePickerController alloc] init];
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
    mImagePicker = [[UIImagePickerController alloc] init];
    mImagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    mImagePicker.delegate = self;

    _returnDataType = MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE;

    return [super init];
}

/**
 * Shows the image picker to the user.
 */
-(void) show
{
    MoSyncUI* mosyncUI = getMoSyncUI();
    [mosyncUI showModal:mImagePicker];
}

/**
 * Hides the image picker.
 * The previous view will be shown.
 */
-(void) hide
{
    MoSyncUI* mosyncUI = getMoSyncUI();
	[mosyncUI hideModal];
}

/**
 * The user has picked an image.
 * From UIImagePickerControllerDelegate.
 * @param picker The controller object managing the image picker interface.
 * @param info A dictionary containing the picked image.
 */
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSLog(@"imagePickerController IN: %d", maGetMilliSecondCount());
    UIImage* selectedImage = (UIImage*) [info objectForKey:UIImagePickerControllerOriginalImage];

    if ( MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE == _returnDataType )
    {
        MAEvent event;
        event.type = EVENT_TYPE_IMAGE_PICKER;
        event.imagePickerState = 1;
        MAHandle handle = [self getImageHandle:selectedImage];

        event.imagePickerItem = handle;
        Base::gEventQueue.put(event);
    }
    else if ( MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA == _returnDataType )
    {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSLog(@"imagePickerController START ENCODING BLOCK: %d", maGetMilliSecondCount());

            MAEvent event;
            event.type = EVENT_TYPE_IMAGE_PICKER;
            event.imagePickerState = 1;

            NSData *imageData = nil;
            // Workaround for rotation bug of UIImagePNGRepresentation.
            if (selectedImage.imageOrientation != UIImageOrientationUp)
            {
                UIImage *normalizedImage;
                UIGraphicsBeginImageContextWithOptions(selectedImage.size, NO, selectedImage.scale);
                [selectedImage drawInRect:(CGRect){0, 0, selectedImage.size}];
                normalizedImage = UIGraphicsGetImageFromCurrentImageContext();
                UIGraphicsEndImageContext();
                NSLog(@"imagePickerController BEFORE ENCODING: %d", maGetMilliSecondCount());
                imageData = UIImagePNGRepresentation(normalizedImage);
                //imageData = UIImageJPEGRepresentation(normalizedImage, 1.0);
                NSLog(@"imagePickerController AFTER ENCODING: %d", maGetMilliSecondCount());
            }
            else
            {
                NSLog(@"imagePickerController BEFORE ENCODING: %d", maGetMilliSecondCount());
                imageData = UIImagePNGRepresentation(selectedImage);
                //imageData = UIImageJPEGRepresentation(selectedImage, 1.0);
                NSLog(@"imagePickerController AFTER ENCODING: %d", maGetMilliSecondCount());
            }

            NSUInteger lenghtOfData = [imageData length];
            NSLog(@"lenght: %d", lenghtOfData);

            //We create a placeholder resource that holds the url string
            MAHandle imgDataHandle = (MAHandle) Base::gSyscall->resources.create_RT_PLACEHOLDER();

            Base::MemStream* ms = new Base::MemStream(lenghtOfData);
            Base::gSyscall->resources.add_RT_BINARY(imgDataHandle, ms);

            ms->seek(Base::Seek::Start, 0);

            ms->write(imageData.bytes, lenghtOfData);
            event.imagePickerEncodingType = MA_IMAGE_PICKER_ITEM_ENCODING_PNG;
            event.imagePickerItem = imgDataHandle;
            Base::gEventQueue.put(event);
        });
    }

    [self hide];
    NSLog(@"imagePickerController OUT: %d", maGetMilliSecondCount());
}

/**
 * The user has cancelled the pick operation.
 * @param picker The controller object managing the image picker interface.
 */
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    MAEvent event;
	event.type = EVENT_TYPE_IMAGE_PICKER;
    event.imagePickerState = 0;
    Base::gEventQueue.put(event);

    [self hide];
}

/**
 * The user has cancelled the pick operation on an iPad.
 * @param picker The controller object managing the popover window.
 */
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popover
{
	[self imagePickerControllerDidCancel:mImagePicker];
}

/**
 * Gets an image handle for a given image.
 * @param image The given image.
 * @return A image handle or RES_OUT_OF_MEMORY in case of error.
 */
- (MAHandle)getImageHandle:(UIImage*)image
{
    NSData* data = UIImageJPEGRepresentation(image, 0);
    int size = [data length];

    MAHandle dataPlaceholder = maCreatePlaceholder();
    TEST_RESULT(maCreateData(dataPlaceholder, size));
    const void* src = [data bytes];
    maWriteData(dataPlaceholder,src,0,size);

    MAHandle imageHandle = maCreatePlaceholder();
    TEST_RESULT(maCreateImageFromData(imageHandle, dataPlaceholder, 0, size));
    maDestroyObject(dataPlaceholder);

    return imageHandle;
}

@end

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
    UIImage* selectedImage = (UIImage*) [info objectForKey:UIImagePickerControllerOriginalImage];

    MAEvent event;
    event.type = EVENT_TYPE_IMAGE_PICKER;
    event.imagePickerState = 1;

    if ( MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE == _returnDataType )
    {
        MAHandle handle = [self getImageHandle:selectedImage];
        event.imagePickerItem = handle;
    }
    else if ( MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA == _returnDataType )
    {
        MAHandle handle = [self getImageDataHandle:selectedImage];
        event.imagePickerEncodingType = MA_IMAGE_PICKER_ITEM_ENCODING_JPEG;
        event.imagePickerItem = handle;
    }

    Base::gEventQueue.put(event);

    [self hide];
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

/**
 * Gets an image data handle for a given image.
 * @param image The given image.
 * @return A image data handle or RES_OUT_OF_MEMORY in case of error.
 */
- (MAHandle)getImageDataHandle:(UIImage*)image
{
    NSData* data = UIImageJPEGRepresentation(image, 0);
    int size = [data length];

    MAHandle dataPlaceholder = maCreatePlaceholder();
    TEST_RESULT(maCreateData(dataPlaceholder, size));
    const void* src = [data bytes];
    maWriteData(dataPlaceholder,src,0,size);

    return dataPlaceholder;
}

@end

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

#import "ImagePickerController.h"
#import "MoSyncUISyscalls.h"
#import "ScreenWidget.h"
#import "ScreenWidgetController.h"
#include "Platform.h"
#include "Syscall.h"

static ImagePickerController *sharedInstance = nil;

@implementation ImagePickerController

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
    
    return [super init];
}

/**
 * Shows the image picker to the user.
 */
-(void) show
{
//    NSLog(@"show Image Picker");
    MoSyncUI* mosyncUI = getMoSyncUI();
    ScreenWidget* shownScreen = (ScreenWidget*)[mosyncUI getCurrentlyShownScreen];
    UIViewController* controller = [shownScreen getController];    
	
    [self performSelectorOnMainThread: @ selector(displayImagePicker:) withObject:controller waitUntilDone:YES];
}

/**
 * Hides the image picker.
 * The previous view will be shown.
 */
-(void) hide
{
    MoSyncUI* mosyncUI = getMoSyncUI();
    ScreenWidget* shownScreen = (ScreenWidget*)[mosyncUI getCurrentlyShownScreen];
    UIViewController* controller = [shownScreen getController];
    [controller dismissModalViewControllerAnimated:YES];
}

/**
 * The user has picked an image.
 * From UIImagePickerControllerDelegate.
 * @param picker The controller object managing the image picker interface. 
 * @param info A dictionary containing the picked image.
 */
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSLog(@"didFinishPickingMediaWithInfo %@", info);
    UIImage* selectedImage = (UIImage*) [info objectForKey:UIImagePickerControllerOriginalImage];
    MAHandle handle = [self getImageHandle:selectedImage];
    NSLog(@"image handle = %d", handle);
    
    MAEvent event;
	event.type = EVENT_TYPE_IMAGE_PICKER;
	event.imagePickerState = 1;
    event.imagePickerItem = handle;
    
    Base::gEventQueue.put(event);
    
    [self hide];
}

/**
 * The user has cancelled the pick operation.
 * @param picker The controller object managing the image picker interface.
 */ 
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    //    NSLog(@"imagePickerControllerDidCancel");  
    MAEvent event;
	event.type = EVENT_TYPE_IMAGE_PICKER;
    event.imagePickerState = 0;
    Base::gEventQueue.put(event);
    
    [self hide];
}

/**
 * Displays the image picker.
 * This function must be called on the main thread.
 * @param The current UIViewController object.
 */
-(void) displayImagePicker:(id) obj
{
    //    NSLog(@"main thread display picker");
    UIViewController* controller = (UIViewController*) obj;
    [controller presentModalViewController:mImagePicker animated:true];    
}

/**
 * Gets an image handle for a given image.
 * @param image The given image.
 * @return A image handle or RES_OUT_OF_MEMORY in case of error.
 */
-(MAHandle) getImageHandle:(UIImage*) image
{
    NSData* data = UIImageJPEGRepresentation(image, 0);
    int size = [data length];
    
    MAHandle placeholder = maCreatePlaceholder();
    int result = maCreateData(placeholder, size);
    NSLog(@"reuslt maCreateData = %d", result);
    const void* src = [data bytes];
    maWriteData(placeholder,src,0,size);

    return placeholder;
}

@end

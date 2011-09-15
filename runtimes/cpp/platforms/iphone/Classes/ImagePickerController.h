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

#import <Foundation/Foundation.h>
#import "helpers/cpp_defs.h"

/**
 * Dispalys an image piker to the user.
 */
@interface ImagePickerController :  NSObject<UINavigationControllerDelegate,UIImagePickerControllerDelegate,UIPopoverControllerDelegate> {

    UIImagePickerController* mImagePicker;
}

/**
 * Returns an instance to the shared singleton. 
 * @return The shared object.
 */
+(ImagePickerController*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Shows the image picker to the user.
 */
-(void) show;

/**
 * Hides the image picker.
 * The previous view will be shown.
 */
-(void) hide;

/**
 * The user has picked an image.
 * From UIImagePickerControllerDelegate.
 * @param picker The controller object managing the image picker interface. 
 * @param info A dictionary containing the picked image.
 */
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;

/**
 * The user has cancelled the pick operation on an iPhone.
 * @param picker The controller object managing the image picker interface.
 */
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;

/**
 * The user has cancelled the pick operation on an iPad.
 * @param picker The controller object managing the popover window.
 */
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popover;

/**
 * Displays the image picker.
 * This function must be called on the main thread.
 * @param The current UIViewController object.
 */
-(void) displayImagePicker:(id) controller;

/**
 * Gets an image handle for a given image.
 * @param image The given image.
 * @return A image handle or RES_OUT_OF_MEMORY in case of error.
 */
-(MAHandle) getImageHandle:(UIImage*) image;

@end

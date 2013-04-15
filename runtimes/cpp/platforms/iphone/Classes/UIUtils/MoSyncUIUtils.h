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

#import <Foundation/Foundation.h>
#include <helpers/cpp_defs.h>

// C
void MoSyncUIUtils_ShowAlert(const char* title, const char* message, const char* button1, const char* button2, const char* button3);
void MoSyncUIUtils_ShowTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints);

void MoSyncUIUtils_ShowImagePicker();
void MoSyncUIUtils_ShowImagePicker(int returnType);

// Saves the image with the given handle to photo album.
int MoSyncUIUtils_SaveImageToGallery(MAHandle imageDataHandle);

// Obj
@interface MoSyncUIUtils : NSObject

+(void) showTextBox:(NSString*)title
         withInText:(NSString*)inText
			outText:(wchar*)outText
			maxSize:(int)maxSize
     andConstraints:(int)constraints;

@end

/**
 * Used as a listener for operation of saving images to photo gallery.
 */
@interface ImageSaveListener : NSObject
{
    // Handle of the image that must be saved to gallery
    MAHandle mImageHandle;
}

-(id)initWithImageHandle:(MAHandle) anImageHandle;

-(void)               image: (UIImage *) image
    didFinishSavingWithError: (NSError *) error
                 contextInfo: (void *) contextInfo;

@end

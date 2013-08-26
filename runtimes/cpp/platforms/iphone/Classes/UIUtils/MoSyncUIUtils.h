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

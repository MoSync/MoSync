/*
 Copyright (C) 2011 MoSync AB

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

/**
 * @file Capture.h
 * @author Bogdan Iusco
 * @date 6 Dec 2011
 *
 * @brief Used for taking photos and recording videos.
 */

#import <Foundation/Foundation.h>

#import "helpers/cpp_defs.h"

/**
 * @brief Design pattern: singlenton.
 * Used for taking photos and recording videos.
 */
@interface Capture : NSObject<UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
    /**
     * System-supplied user interfaces for taking pictures and movies.
     */
    UIImagePickerController* mImagePicker;

    /**
     * Store image type handles and data content of taken photos.
     * Contains pairs of:
     * - NSNumber type objects(the key generated with maCreatePlaceholder()).
     * - NSData type objects.
     */
    NSMutableDictionary* mImageHandleArray;

    /**
     * Store recorded videos.
     * Contains pairs of:
     * - NSString type objects(the object).
     * - NSNumber type objects(the key generated with maCreatePlaceholder()).
     */
    NSMutableDictionary* mVideoDictionary;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(Capture*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * @brief Sets the properties to the Native Image Picker.
 * @param property A string representing which property to set.
 *  One of the MA_CAPTURE_ constants.
 * @param value The value that will be assigned to the property.
 * @return
 *  - MA_CAPTURE_RES_OK
 *  - MA_CAPTURE_RES_INVALID_PROPERTY
 *  - MA_CAPTURE_RES_INVALID_PROPERTY_VALUE
 */
-(int) setProperty:(const char*) property
         withValue:(const char*) value;

/**
 * @brief Retrieves the properties from the Native Image Picker.
 * @param property A string representing which property to get.
 *  One of the MA_CAPTURE_ constants.
 * @param value A buffer that will hold the value of the property, represented as a string.
 * @param bufSize Size of the buffer.
 * @return One of the next constants:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_INVALID_PROPERTY if the property name is not valid.
 * - MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
 */
-(int) getProperty:(const char*) property
             value:(char*) value
           maxSize:(const int) bufSize;

/**
 * @brief Perform an action on the image picker.
 * @param action One of the MA_CAPTURE_ACTION_ constants.
 * @return One of the next constants:
 *  - MA_CAPTURE_RES_OK if no error occurred.
 *  - MA_CAPTURE_RES_INVALID_ACTION if the given action is invalid.
 *  - MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE if the camera is not available.
 *  - MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED if video recording is not supported.
 *  - MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED if camera picture mode is not supported.
 */
-(int) action:(const int) action;

/**
 * @brief Save a image data object to a file.
 * @param handle Handle to a image data object.
 * @param fullPath A buffer containing the a full path where the file will be created.
 * @param fullPathBufSize The size of the fullPath buffer.
 * @return One of the next constants:
 *  - MA_CAPTURE_RES_OK if no error occurred.
 *  - MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
 *  - MA_CAPTURE_RES_FILE_INVALID_NAME if the fullPath param is invalid.
 *  - MA_CAPTURE_RES_FILE_ALREADY_EXISTS if the file already exists.
 */
-(int) writeImage:(MAHandle) handle
             withPath:(const char*) fullPath
              maxSize:(const int) fullPathBufSize;

/**
 * Get full path to a recorded video.
 * @param handle Handle to a video data object.
 * @param buffer Will contain the full path to the video file.
 * @param maxSize Maximum size of the buffer.
 * @return One of the next constants:
 *  - MA_CAPTURE_RES_OK if no error occurred.
 *  - MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
 *  - MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
 */
-(int) getVideoPath:(MAHandle) handle
             buffer:(char*) buffer
            maxSize:(const int) maxSize;
/**
 * @brief Destroys a image/video data object.
 * @param handle Handle to a image/video data object.
 *
 * @return One of the next constants:
 *  - MA_CAPTURE_RES_OK if no error occurred.
 *  - MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
 */
-(int) destroyData:(MAHandle) handle;

/**
 * Starts video capture.
 * @return One of the next constans:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE if the camera is not available.
 * - MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED if video recording is not supported.
 */
-(int) recordVideo;

/**
 * Starts camera in image mode.
 * @return One of the next constans:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE if the camera is not available.
 * - MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED if camera picture mode is not supported.
 */
-(int) takePicture;

/**
 * Display the image picker.
 */
-(void) showImagePicker;

/**
 * Hide the current shown image picker.
 */
-(void) hideImagePicker;

/**
 * Set the video quality.
 * @param value A string containing one of the MA_CAPTURE_VIDEO_QUALITY constants.
 * @return One of the next constants:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_INVALID_PROPERTY_VALUE if value param is invalid.
 */
-(int) setVideoQuality:(NSString*) value;

/**
 * Get the video quality.
 * @return One of the the MA_CAPTURE_VIDEO_QUALITY constants.
 */
-(int) getVideoQuality;

/**
 * Store taken photo and send event.
 * Becasue this method is time consuming, call it on a new thread.
 * @param cameraDictionary Dictionary received from the camera.
 */
-(void) handleTakenPhoto:(NSDictionary *) cameraDictionary;

/**
 * Store recorded video and send event.
 * @param cameraDictionary Dictionary received from the camera.
 */
-(void) handleRecordedVideo:(NSDictionary *) cameraDictionary;

@end

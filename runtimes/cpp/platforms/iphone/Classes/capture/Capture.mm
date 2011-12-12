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
 * @file Capture.mm
 * @author Bogdan Iusco
 * @date 6 Dec 2011
 *
 * @brief Used for taking photos and recording videos.
 */

#import <MobileCoreServices/UTCoreTypes.h>

#import "Capture.h"
#import "MoSyncUI.h"
#import "MoSyncUISyscalls.h"
#import "ScreenWidget.h"
#import "NSStringExpanded.h"
#import "helpers/cpp_defs.h"

@implementation Capture

static Capture *sSharedInstance = nil;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(Capture*) getInstance
{
    if (!sSharedInstance)
    {
        sSharedInstance = [[Capture alloc] init];
    }

    return sSharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sSharedInstance release];
}

/**
 * Init function.
 */
- (id)init
{
    mImagePicker = [[UIImagePickerController alloc] init];
    mImagePicker.delegate = self;
    mImagePicker.allowsEditing = YES;

    mImageHandleArray = [[NSMutableDictionary alloc] init];
    mVideoDictionary = [[NSMutableDictionary alloc] init];

    return [super init];
}

/**
 * Called if the user cancelled the pick operation.
 * @param picker The controller object managing the image picker interface.
 */
-(void) imagePickerControllerDidCancel:(UIImagePickerController *) picker
{
    NSLog(@"Capture::imagePickerControllerDidCancel");

    // Send event.
    MAEvent event;
    event.type = EVENT_TYPE_CAPTURE;
    MACaptureEventData eventData;
    eventData.type = MA_CAPTURE_EVENT_TYPE_CANCEL;
    event.captureData = eventData;
    Base::gEventQueue.put(event);

    [self hideImagePicker];
}

/**
 * Called if the user picked a still image or movie.
 * @param picker The controller object managing the image picker interface.
 * @param info A dictionary containing the original image and the edited image,
 * if an image was picked; or a filesystem URL for the movie, if a movie was picked.
 */
-(void) imagePickerController:(UIImagePickerController *) picker didFinishPickingMediaWithInfo:(NSDictionary *) info
{
    NSLog(@"Capture::didFinishPickingMediaWithInfo %@", [info description]);

    NSString* mediaType = [info objectForKey: UIImagePickerControllerMediaType];

    // Check if an image has been taken.
    if ([mediaType isEqualToString: (NSString*) kUTTypeImage])
    {
        // Perform selector on a new thread because creating a PNG/JPEG image takes time.
        [NSThread detachNewThreadSelector:@selector(handleTakenPhoto:)
                                 toTarget:self
                               withObject:info];
    }
    // Check if a video has been recorded.
    else if ([mediaType isEqualToString: (NSString*) kUTTypeMovie])
    {
        [self handleRecordedVideo:info];
    }

    [self hideImagePicker];
}

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
         withValue:(const char*) value
{
    int returnValue = MA_CAPTURE_RES_OK;
    NSString* propertyString = [[NSString alloc] initWithUTF8String:property];
    NSString* valueString = [[NSString alloc] initWithUTF8String:value];

    if ([propertyString isEqualToString:@MA_CAPTURE_MAX_DURATION])
    {
        if (![valueString canParseNumber] ||
            0 > [valueString intValue])
        {
            returnValue = MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
        }
        else
        {
            mImagePicker.videoMaximumDuration = [valueString intValue];
        }
    }
    else if ([propertyString isEqualToString:@MA_CAPTURE_VIDEO_QUALITY])
    {
        returnValue = [self setVideoQuality:valueString];
    }
    else
    {
        returnValue = MA_CAPTURE_RES_INVALID_PROPERTY;
    }

    [propertyString release];
    [valueString release];

    return returnValue;
}

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
           maxSize:(const int) bufSize
{
    NSString* propertyString = [[NSString alloc] initWithUTF8String:property];
    NSString* valueString = nil;
    int returnedValue = MA_CAPTURE_RES_OK;

    if ([propertyString isEqualToString:@MA_CAPTURE_MAX_DURATION])
    {
        int maxDuration = mImagePicker.videoMaximumDuration;
        valueString = [[NSString alloc] initWithFormat:@"%d", maxDuration];
    }
    else if ([propertyString isEqualToString:@MA_CAPTURE_VIDEO_QUALITY])
    {
        int videoQuality = [self getVideoQuality];
        valueString = [[NSString alloc] initWithFormat:@"%d", videoQuality];
    }
    else
    {
        returnedValue = MA_CAPTURE_RES_INVALID_PROPERTY;
    }

    // Write value into buffer
    if (returnedValue == MA_CAPTURE_RES_OK)
    {
        int realLength = [valueString length];
        if(realLength > bufSize)
        {
            NSLog(@"Capture::getProperty invalid buffer size");
            returnedValue =  MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
        }
        else
        {
            [valueString getCString:value maxLength:bufSize encoding:NSASCIIStringEncoding];
        }
    }

    [propertyString release];
    [valueString release];

    return returnedValue;
}

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
-(int) action:(const int) action
{
    int returnedValue = MA_CAPTURE_RES_OK;
    switch (action)
    {
        case MA_CAPTURE_ACTION_RECORD_VIDEO:
            returnedValue = [self recordVideo];
            break;
        case MA_CAPTURE_ACTION_STOP_RECORDING:
            [mImagePicker stopVideoCapture];
            returnedValue = MA_CAPTURE_RES_OK;
            break;
        case MA_CAPTURE_ACTION_TAKE_PICTURE:
            returnedValue = [self takePicture];
            break;
        default:
            returnedValue = MA_CAPTURE_RES_INVALID_ACTION;
    }

    return returnedValue;
}

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
          maxSize:(const int) fullPathBufSize
{
    int returnValue = MA_CAPTURE_RES_OK;
    NSNumber* key = [[NSNumber alloc] initWithInt:handle];
    NSData* data = [mImageHandleArray objectForKey:key];
    if (data)
    {
        NSString* address = [[NSString alloc] initWithUTF8String:fullPath];
        if (!address)
        {
            returnValue = MA_CAPTURE_RES_FILE_INVALID_NAME;
        }
        else
        {
            [data writeToFile:address atomically:YES];
        }
        [address release];
    }
    else
    {
        returnValue = MA_CAPTURE_RES_INVALID_HANDLE;
    }

    [key release];
    return returnValue;
}

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
            maxSize:(const int) maxSize
{
    int result = MA_CAPTURE_RES_OK;
    NSNumber* key = [[NSNumber alloc] initWithInt:handle];
    NSString* path = [mVideoDictionary objectForKey:key];
    if (path)
    {
        int length = [path length];
        if (length > maxSize)
        {
            result = MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
        }
        else
        {
            [path getCString:buffer maxLength:maxSize encoding:NSASCIIStringEncoding];
        }
    }
    else
    {
        result = MA_CAPTURE_RES_INVALID_HANDLE;
    }

    return result;
}

/**
 * @brief Destroys a image/video data object.
 * @param handle Handle to a image/video data object.
 *
 * @return One of the next constants:
 *  - MA_CAPTURE_RES_OK if no error occurred.
 *  - MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
 */
-(int) destroyData:(MAHandle) handle
{
    // Check if handle is related to a image.
    NSNumber* handleKey = [NSNumber numberWithInt:handle];
    NSData* data = [mImageHandleArray objectForKey:handleKey];
    if (data)
    {
        maDestroyObject(handle);
        [mImageHandleArray removeObjectForKey:handleKey];
        [data release];
        return MA_CAPTURE_RES_OK;
    }

    // Check if handle is related to a video.
    NSString* videoPath = [mVideoDictionary objectForKey:handleKey];
    if (videoPath)
    {
        [mVideoDictionary removeObjectForKey:handleKey];
        [videoPath release];
        return MA_CAPTURE_RES_OK;
    }

    return MA_CAPTURE_RES_INVALID_HANDLE;
}

/**
 * Starts video capture.
 * @return One of the next constans:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE if the camera is not available.
 * - MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED if video recording is not supported.
 */
-(int) recordVideo
{
    if ([UIImagePickerController isSourceTypeAvailable:
         UIImagePickerControllerSourceTypeCamera] == NO)
    {
        NSLog(@"Capture::recordVideo camera not available");
        return MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE;
    }

    mImagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;

    // Check if video recording is supported.
    NSArray* array = [UIImagePickerController availableMediaTypesForSourceType:
                      UIImagePickerControllerSourceTypeCamera];
    bool isVideoSupported = false;
    for (NSString* key in array)
    {
        if ([key isEqualToString:(NSString*) kUTTypeMovie])
        {
            isVideoSupported = true;
            break;
        }
    }

    if (isVideoSupported)
    {
        mImagePicker.mediaTypes = [NSArray arrayWithObject:(NSString*) kUTTypeMovie];
    }
    else
    {
        return MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED;
    }
    [self showImagePicker];

    return MA_CAPTURE_RES_OK;
}

/**
 * Starts camera in image mode.
 * @return One of the next constans:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE if the camera is not available.
 * - MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED if camera picture mode is not supported.
 */
-(int) takePicture
{
    if ([UIImagePickerController isSourceTypeAvailable:
         UIImagePickerControllerSourceTypeCamera] == NO)
    {
        NSLog(@"Capture::takePicture camera not available");
        return MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE;
    }

    mImagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;

    // Check if taking picture mode is supported.
    NSArray* array = [UIImagePickerController availableMediaTypesForSourceType:
                      UIImagePickerControllerSourceTypeCamera];
    bool isPictureSupported = false;
    for (NSString* key in array)
    {
        if ([key isEqualToString:(NSString*) kUTTypeImage])
        {
            isPictureSupported = true;
            break;
        }
    }

    if (isPictureSupported)
    {
        mImagePicker.mediaTypes = [NSArray arrayWithObject:(NSString*) kUTTypeImage];
    }
    else
    {
        return MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED;
    }

    [self showImagePicker];

    return MA_CAPTURE_RES_OK;
}

/**
 * Display the image picker.
 */
-(void) showImagePicker
{
    MoSyncUI* mosyncUI = getMoSyncUI();
    [mosyncUI showModal:mImagePicker];
}

/**
 * Hide the current shown image picker.
 */
-(void) hideImagePicker
{
    MoSyncUI* mosyncUI = getMoSyncUI();
    [mosyncUI hideModal];
}

/**
 * Set the video quality.
 * @param value A string containing one of the MA_CAPTURE_VIDEO_QUALITY constants.
 * @return One of the next constants:
 * - MA_CAPTURE_RES_OK if no error occurred.
 * - MA_CAPTURE_RES_INVALID_PROPERTY_VALUE if value param is invalid.
 */
-(int) setVideoQuality:(NSString*) value
{
    if (![value canParseNumber])
    {
        return MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
    }

    int returnValue = MA_CAPTURE_RES_OK;
    int qualityInt = [value intValue];
    UIImagePickerControllerQualityType videoQuality;
    switch (qualityInt)
    {
        case MA_CAPTURE_VIDEO_QUALITY_HIGH:
            videoQuality = UIImagePickerControllerQualityTypeHigh;
            break;
        case MA_CAPTURE_VIDEO_QUALITY_MEDIUM:
            videoQuality = UIImagePickerControllerQualityTypeMedium;
            break;
        case MA_CAPTURE_VIDEO_QUALITY_LOW:
            videoQuality = UIImagePickerControllerQualityTypeLow;
            break;
        default:
            returnValue = MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
    }

    if (returnValue == MA_CAPTURE_RES_OK)
    {
        mImagePicker.videoQuality = videoQuality;
    }

    return returnValue;
}

/**
 * Get the video quality.
 * @return One of the the MA_CAPTURE_VIDEO_QUALITY constants.
 */
-(int) getVideoQuality
{
    int quality = MA_CAPTURE_VIDEO_QUALITY_LOW;
    UIImagePickerControllerQualityType qualityType = mImagePicker.videoQuality;
    switch (qualityType)
    {
        case UIImagePickerControllerQualityTypeHigh:
            quality = MA_CAPTURE_VIDEO_QUALITY_HIGH;
            break;
        case UIImagePickerControllerQualityTypeMedium:
            quality = MA_CAPTURE_VIDEO_QUALITY_MEDIUM;
            break;
        case UIImagePickerControllerQualityTypeLow:
            quality = MA_CAPTURE_VIDEO_QUALITY_LOW;
            break;
    }
    return quality;
}

/**
 * Store taken photo and send event.
 * Becasue this method is time consuming, call it on a new thread.
 * @param cameraDictionary Dictionary received from the camera.
 */
-(void) handleTakenPhoto:(NSDictionary *) cameraDictionary
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    UIImage* image = [cameraDictionary objectForKey:UIImagePickerControllerEditedImage];
    if (!image)
    {
        NSLog(@"Capture::handleTakenPhoto image is invalid");
        [pool release];
        return;
    }

    MAHandle dataPlaceholder = maCreatePlaceholder();
    NSData *imageData = [UIImagePNGRepresentation(image) retain];
    int size = [imageData length];
    const void* src = [imageData bytes];
    if (RES_OK == maCreateData(dataPlaceholder, size))
    {
        maWriteData(dataPlaceholder, src, 0, size);
        MAHandle imageHandle = maCreatePlaceholder();
        maCreateImageFromData(imageHandle, dataPlaceholder, 0, size);
        maDestroyObject(dataPlaceholder);

        NSNumber* handle = [[NSNumber alloc] initWithInt:imageHandle];
        [mImageHandleArray setObject:imageData forKey:handle];
        [handle release];

        // Send event.
        MAEvent event;
        event.type = EVENT_TYPE_CAPTURE;
        MACaptureEventData eventData;
        eventData.type = MA_CAPTURE_EVENT_TYPE_IMAGE;
        eventData.handle = imageHandle;
        event.captureData = eventData;
        Base::gEventQueue.put(event);
    }

    [pool release];
}

/**
 * Store recorded video and send event.
 * @param cameraDictionary Dictionary received from the camera.
 */
-(void) handleRecordedVideo:(NSDictionary *) cameraDictionary
{
    NSString *videoPath = [[cameraDictionary objectForKey:UIImagePickerControllerMediaURL] path];

//    if (UIVideoAtPathIsCompatibleWithSavedPhotosAlbum (videoPath))
//    {
//        UISaveVideoAtPathToSavedPhotosAlbum(videoPath, nil, nil, nil);
//    }

    if (!videoPath)
    {
        NSLog(@"Capture::handleRecordedVideo video is invalid");
        return;
    }

    MAHandle placeholder = maCreatePlaceholder();
    NSNumber* key = [[NSNumber alloc] initWithInt: placeholder];

    // Ownership of the videoPath object is passed to the dictionary object.
    [videoPath retain];
    [mVideoDictionary setObject:videoPath forKey:key];

    // Send event.
    MAEvent event;
    event.type = EVENT_TYPE_CAPTURE;
    MACaptureEventData eventData;
    eventData.type = MA_CAPTURE_EVENT_TYPE_VIDEO;
    eventData.handle = placeholder;
    event.captureData = eventData;
    Base::gEventQueue.put(event);

    [key release];
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [mImagePicker release];
    [mImageHandleArray release];
    [mVideoDictionary release];

    [super dealloc];
}

@end

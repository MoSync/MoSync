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

#import "ImageWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>

/**
 * Hidden functions/methods for ImageWidget class.
 */
@interface ImageWidget ()

/**
 * Used by the MAW_IMAGE_IMAGE property.
 */
@property(nonatomic, retain) NSNumber* imageHandle;

/**
 * Used by the MAW_IMAGE_PATH property.
 */
@property(nonatomic, retain) NSString* imagePath;

/**
 * Show an image from a given path.
 * Setter for MAW_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was shown.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyImagePath:(NSString*)path;

@end

@implementation ImageWidget

@synthesize imageHandle = _imageHandle;
@synthesize imagePath = _imagePath;

- (id)init
{
    self = [super init];
    if (self)
    {
        _imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)];
        self.view = _imageView;
        _leftCapWidth = 0;
        _topCapHeight = 0;
        self.autoSizeWidth = WidgetAutoSizeWrapContent;
        self.autoSizeHeight = WidgetAutoSizeWrapContent;
        self.view.contentMode = UIViewContentModeCenter;
    }
	return self;
}

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
    if([key isEqualToString:@MAW_IMAGE_IMAGE])
    {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
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
		if (_leftCapWidth != 0 || _topCapHeight != 0)
        {
			image = [image stretchableImageWithLeftCapWidth:_leftCapWidth topCapHeight:_topCapHeight];
		}
        _imageView.image = image;
		self.imageHandle = [NSNumber numberWithInt:imageHandle];
		self.imagePath = @"";
		[self layout];
	}
	else if([key isEqualToString:@"leftCapWidth"])
    {
		int newLeftCapWidth = [value intValue];
		if (_imageView != nil)
        {
			UIImage* image = [_imageView.image stretchableImageWithLeftCapWidth:newLeftCapWidth
                                                                   topCapHeight:_topCapHeight];
			_imageView.image = image;
		}
		_leftCapWidth = newLeftCapWidth;
	}
	else if ([key isEqualToString:@"topCapHeight"])
    {
		int newTopCapHeight = [value intValue];
		if (_imageView != nil)
        {
			UIImage* image = [_imageView.image stretchableImageWithLeftCapWidth:_leftCapWidth
                                                                   topCapHeight:newTopCapHeight];
			_imageView.image = image;
		}
		_topCapHeight = newTopCapHeight;
	}
    else if ([key isEqualToString:@MAW_IMAGE_SCALE_MODE])
    {
        // none
        // scaleXY
        // scalePreserveAspect

        // maybe have these later?
        // scaleX
        // scaleY

        if([value isEqualToString:@"none"]) self.view.contentMode = UIViewContentModeCenter;
        else if([value isEqualToString:@"scaleXY"]) self.view.contentMode = UIViewContentModeScaleToFill;
        else if([value isEqualToString:@"scalePreserveAspect"]) self.view.contentMode = UIViewContentModeScaleAspectFit;
    }
	else if([key isEqualToString:@MAW_IMAGE_PATH])
	{
		return [self setPropertyImagePath:value];
	}
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if ([key isEqualToString:@MAW_IMAGE_IMAGE])
	{
		return [[self.imageHandle stringValue] retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_PATH])
	{
		return [self.imagePath retain];
	}
	else
	{
		return [super getPropertyWithKey:key];
	}
}

/**
 * Show an image from a given path.
 * Setter for MAW_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was shown.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyImagePath:(NSString*)path
{
	UIImage* image = [[UIImage alloc] initWithContentsOfFile:path];
	if (!image)
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}
	self.imagePath = path;
	self.imageHandle = [NSNumber numberWithInt:0];
	_imageView.image = image;
	[image release];
	[self layout];
	return MAW_RES_OK;
}

/**
 * Getter implementation for imageNumber property.
 */
- (NSNumber*)imageHandle
{
	if (!_imageHandle)
	{
		_imageHandle = [[NSNumber alloc] initWithInt:0];
	}
	return _imageHandle;
}

/**
 * Getter implementation for imagePath property.
 */
- (NSString*)imagePath
{
	if (!_imagePath)
	{
		_imagePath = [[NSString alloc] init];
	}
	return _imagePath;
}

/**
 * Dealloc method
 */
-(void) dealloc
{
    [_imageView release];
	[_imageHandle release];
	[_imagePath release];
    [super dealloc];
}

@end

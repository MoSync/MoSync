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

#import "ImageButtonWidget.h"

/**
 * Hidden functions/methods for ImageButtonWidget class.
 */
@interface ImageButtonWidget ()

/**
 * Used by the MAW_IMAGE_BUTTON_IMAGE_PATH property.
 */
@property(nonatomic, retain) NSString* foregroundImagePath;

/**
 * Used by the MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH property.
 */
@property(nonatomic, retain) NSString* backgroundImagePath;

/**
 * Used by the MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH property.
 */
@property(nonatomic, retain) NSString* pressedImagePath;

/**
 * Used by the MAW_IMAGE_BUTTON_PRESSED_IMAGE property.
 */
@property(nonatomic, assign) MAHandle pressedImageHadle;

/**
 * Set the foreground image from a given file path.
 * Setter for MAW_IMAGE_BUTTON_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyForegroundImagePath:(NSString*)path;

/**
 * Set the background image from a given file path.
 * Setter for MAW_IMAGE_BUTTON_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyBackgroundImagePath:(NSString*)path;

/**
 * Set the pressed image from a handle.
 * Setter for MAW_IMAGE_BUTTON_PRESSED_IMAGE.
 * @param image Image handle as string.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the handle is invalid.
 */
- (int)setPropertyPressedImage:(NSString*)imageHandle;

/**
 * Set the pressed image from a given file path.
 * Setter for MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyPressedImagePath:(NSString*)path;

@end

@implementation ImageButtonWidget

@synthesize foregroundImagePath = _foregroundImagePath;
@synthesize backgroundImagePath = _backgroundImagePath;

- (id)init
{
    self = [super init];
    if (self)
    {
        self.view = [UIButton buttonWithType:UIButtonTypeCustom];
        [self setupWidget];
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
	if ([key isEqualToString:@MAW_IMAGE_BUTTON_IMAGE_PATH])
	{
		return [self setPropertyForegroundImagePath:value];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH])
	{
		return [self setPropertyBackgroundImagePath:value];
	}
	else if([key isEqualToString:@MAW_IMAGE_BUTTON_IMAGE])
	{
		int result = [super setPropertyWithKey:key toValue:value];
		if (result == MAW_RES_OK)
		{
			self.foregroundImagePath = @"";
		}
		return result;
	}
	else if([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE])
	{
		int result = [super setPropertyWithKey:key toValue:value];
		if (result == MAW_RES_OK)
		{
			self.backgroundImagePath = @"";
		}
		return result;
	}
	else if([key isEqualToString:@MAW_IMAGE_BUTTON_PRESSED_IMAGE])
	{
		return [self setPropertyPressedImage:value];
	}
	else if([key isEqualToString:@MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH])
	{
		return [self setPropertyPressedImagePath:value];
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
	if ([key isEqualToString:@MAW_IMAGE_BUTTON_IMAGE_PATH])
	{
		return [self.foregroundImagePath retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH])
	{
		return [self.backgroundImagePath retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_PRESSED_IMAGE])
	{
		return [[NSString alloc] initWithFormat:@"%d", self.pressedImageHadle];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH])
	{
		return [self.pressedImagePath retain];
	}
	else
	{
		return [super getPropertyWithKey:key];
	}
}

/**
 * Set the foreground image from a given file path.
 * Setter for MAW_IMAGE_BUTTON_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyForegroundImagePath:(NSString*)path
{
	UIImage* imageSource = [[UIImage alloc] initWithContentsOfFile:path];
	if (!imageSource)
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}
	self.foregroundImagePath = path;
	UIButton* button = (UIButton*)self.view;
	[button setImage:imageSource forState:UIControlStateNormal];
	[imageSource release];
	[self layout];
	return MAW_RES_OK;
}

/**
 * Set the background image from a given file path.
 * Setter for MAW_IMAGE_BUTTON_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyBackgroundImagePath:(NSString*)path
{
	UIImage* imageSource = [[UIImage alloc] initWithContentsOfFile:path];
	if (!imageSource)
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}
	self.backgroundImagePath = path;
	UIButton* button = (UIButton*)self.view;
	[button setBackgroundImage:imageSource forState:UIControlStateNormal];
	[imageSource release];
	[self layout];
	return MAW_RES_OK;
}

/**
 * Set the pressed image from a handle.
 * Setter for MAW_IMAGE_BUTTON_PRESSED_IMAGE.
 * @param image Image handle as string.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the handle is invalid.
 */
- (int)setPropertyPressedImage:(NSString*)imageHandle
{
	int handle = [imageHandle intValue];
	if(handle <= 0)
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}

	UIButton* button = (UIButton*) self.view;
	Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(handle);
	image = [UIImage imageWithCGImage:imageResource->image];
	[button setBackgroundImage:image forState:UIControlStateHighlighted];

	self.pressedImageHadle = handle;
	self.pressedImagePath = @"";

	return MAW_RES_OK;
}

/**
 * Set the pressed image from a given file path.
 * Setter for MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH.
 * @param path Image file path.
 * @return One of the following result codes:
 * - MAW_RES_OK if the image was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the path is invalid.
 */
- (int)setPropertyPressedImagePath:(NSString*)path
{
	UIImage* imageSource = [[UIImage alloc] initWithContentsOfFile:path];
	if (!imageSource)
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}

	UIButton* button = (UIButton*)self.view;
	[button setBackgroundImage:imageSource forState:UIControlStateHighlighted];
	[imageSource release];

	self.pressedImagePath = path;
	self.pressedImageHadle = 0;

	return MAW_RES_OK;
}

/**
 * Getter implementation for foregroundImagePath property.
 */
- (NSString*)foregroundImagePath
{
	if (!_foregroundImagePath)
	{
		_foregroundImagePath = [[NSString alloc] init];
	}
	return _foregroundImagePath;
}

/**
 * Getter implementation for backgroundImagePath property.
 */
- (NSString*)backgroundImagePath
{
	if (!_backgroundImagePath)
	{
		_backgroundImagePath = [[NSString alloc] init];
	}
	return _backgroundImagePath;
}

/**
 * Getter implementation for pressedImagePath property.
 */
- (NSString*)pressedImagePath {
	if (!_pressedImagePath)
	{
		_pressedImagePath = [[NSString alloc] init];
	}
	return _pressedImagePath;
}

- (void)dealloc
{
	[_foregroundImagePath release];
	[_backgroundImagePath release];
	[_pressedImagePath release];

	[super dealloc];
}

@end

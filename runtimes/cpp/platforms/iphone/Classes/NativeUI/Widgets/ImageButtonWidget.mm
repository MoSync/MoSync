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

#import "ImageButtonWidget.h"

/**
 * Hidden functions/methods for ImageButtonWidget class.
 */
@interface ImageButtonWidget ()

/**
 * Used by the MAW_IMAGE_BUTTON_IMAGE property.
 */
@property(nonatomic, retain) NSNumber* foregroundImageHandle;

/**
 * Used by the MAW_IMAGE_BUTTON_IMAGE_PATH property.
 */
@property(nonatomic, retain) NSString* foregroundImagePath;

/**
 * Used by the MAW_IMAGE_BUTTON_BACKGROUND_IMAGE property.
 */
@property(nonatomic, retain) NSNumber* backgroundImageHandle;

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
@property(nonatomic, assign) MAHandle pressedImageHandle;

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

@synthesize foregroundImageHandle = _foregroundImageHandle;
@synthesize foregroundImagePath = _foregroundImagePath;
@synthesize backgroundImageHandle = _backgroundImageHandle;
@synthesize backgroundImagePath = _backgroundImagePath;
@synthesize pressedImagePath = _pressedImagePath;
@synthesize pressedImageHandle;

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
			self.foregroundImageHandle = [NSNumber numberWithInt:[value intValue]];
		}
		return result;
	}
	else if([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE])
	{
		int result = [super setPropertyWithKey:key toValue:value];
		if (result == MAW_RES_OK)
		{
			self.backgroundImageHandle = [NSNumber numberWithInt:[value intValue]];
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
	if ([key isEqualToString:@MAW_IMAGE_BUTTON_IMAGE])
	{
		return [[self.foregroundImageHandle stringValue] retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_IMAGE_PATH])
	{
		return [self.foregroundImagePath retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE])
	{
		return [[self.backgroundImageHandle stringValue] retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH])
	{
		return [self.backgroundImagePath retain];
	}
	else if ([key isEqualToString:@MAW_IMAGE_BUTTON_PRESSED_IMAGE])
	{
		return [[NSString alloc] initWithFormat:@"%d", self.pressedImageHandle];
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
	self.foregroundImageHandle = [NSNumber numberWithInt:0];
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
	self.backgroundImageHandle = [NSNumber numberWithInt:0];
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

	self.pressedImageHandle = handle;
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
	self.pressedImageHandle = 0;

	return MAW_RES_OK;
}

/**
 * Getter implementation for foregroundImageHandle property.
 */
- (NSNumber*)foregroundImageHandle
{
	if (!_foregroundImageHandle)
	{
		_foregroundImageHandle = [[NSNumber alloc] initWithInt:0];
	}
	return _foregroundImageHandle;
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
 * Getter implementation for backgroundImageHandle property.
 */
- (NSNumber*)backgroundImageHandle
{
	if (!_backgroundImageHandle)
	{
		_backgroundImageHandle = [[NSNumber alloc] initWithInt:0];
	}
	return _backgroundImageHandle;
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
	[_foregroundImageHandle release];
	[_foregroundImagePath release];
	[_backgroundImageHandle release];
	[_backgroundImagePath release];
	[_pressedImagePath release];

	[super dealloc];
}

@end

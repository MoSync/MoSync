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

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

#include "Platform.h"
#import "StackScreenWidget.h"
#import "StackScreenWidgetController.h"
#import "UINavigationControllerExpanded.h"

@implementation StackScreenWidget

/**
 * Init function.
 */
- (id)init
{
	UINavigationController* navigationController = [[[StackScreenWidgetController alloc] init] autorelease];
    self = [super initWithController:navigationController];
    if (self)
    {
        stack = [[NSMutableArray alloc] init];
        scaleMode = nil;
        navigationController.viewControllers = [NSArray array];
        navigationController.delegate = self;
    }
	return self;
}

/**
 * Pop the top view controller from the navigation stack.
 * Called by the UINavigationController object.
 * For more info see UINavigationControllerExpanded file.
 */
- (void)viewControllerWillBePoped
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_STACK_SCREEN_POPPED;
	eventData->widgetHandle = self.handle;
    int fromScreenHandle = -1;
    int toScreenHandle = -1;

    if ([stack count] > 0)
    {
        int fromScreenIndex = [stack count] - 1;
        IWidget* fromScreen = [stack objectAtIndex:fromScreenIndex];
        fromScreenHandle = fromScreen.handle;
		fromScreen.parent = nil;
    }
    if ([stack count] > 1)
    {
        int toScreenIndex = [stack count] - 2;
        IWidget* toScreen = [stack objectAtIndex:toScreenIndex];
        toScreenHandle = toScreen.handle;
    }

    eventData->fromScreen = fromScreenHandle;
    eventData->toScreen = toScreenHandle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);

    [stack removeLastObject];
}

/**
 * Push a view controller on the navigation stack.
 * @param child Widget containing a view controller(e.g. ScreenWidget).
 */
- (void)push:(IWidget*)child
{
	UINavigationController* navigationController = (UINavigationController*)_controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	[navigationController pushViewController:[screen getController] animated:YES];
	[stack addObject:child];
	float navBarHeight = navigationController.toolbar.bounds.size.height;
	float viewHeight = self.height - navBarHeight;
    child.size = CGSizeMake(self.width, viewHeight);
    [child layout];
	[child show];
	child.parent = self;
}

/**
 * Pop the top screen from the stack.
 */
- (void)pop
{
	UINavigationController* navigationController = (UINavigationController*)_controller;
	[navigationController popViewControllerAnimated:YES];
}

/**
 * Recalculate its and children size.
 */
- (void)layout
{
	UINavigationController* navigationController = (UINavigationController*)_controller;
	float navBarHeight = navigationController.toolbar.bounds.size.height;
    float viewWidth = self.width;
	float viewHeight = self.height - navBarHeight;
	[self.view setNeedsLayout];
	for (IWidget *child in stack)
    {
        child.size = CGSizeMake(viewWidth, viewHeight);
        [child layout];
	}
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
- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value
{
	if([key isEqualToString:@MAW_SCREEN_TITLE])
    {
		_controller.title = value;
	}
	else if([key isEqualToString:@MAW_STACK_SCREEN_BACK_BUTTON_ENABLED])
    {
		UINavigationController* navigationController = (UINavigationController*)_controller;
		navigationController.navigationBar.backItem.hidesBackButton = [value boolValue];
	}
    else if ([key isEqualToString:@MAW_STACK_SCREEN_TITLE_FONT_HANDLE])
    {
        UIFont* font = Base::getUIFontObject([value intValue]);
        if (font)
        {
            // Customize the title text for ALL UINavigationBars
            [[UINavigationBar appearance] setTitleTextAttributes:
             [NSDictionary dictionaryWithObjectsAndKeys:
              [UIColor colorWithRed:255.0/255.0 green:255.0/255.0 blue:255.0/255.0 alpha:1.0],
              UITextAttributeTextColor,
              [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.8],
              UITextAttributeTextShadowColor,
              [NSValue valueWithUIOffset:UIOffsetMake(0, -1)],
              UITextAttributeTextShadowOffset,
              font,
              UITextAttributeFont,
              nil]];
        }
    }
    else if ([key isEqualToString:@MAW_STACK_SCREEN_TITLE_BACKGROUND_IMAGE_HANDLE])
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

        CGFloat imageScale = 1.0;

        if (scaleMode)
        {
            if ([scaleMode isEqualToString:@"scaleAndRepeatXY"])
            {
                CGFloat screenScale = [[UIScreen mainScreen] scale];
                imageScale = imageResource->height / (screenScale * 44.0);
            }
        }

        UIImage* image = [UIImage imageWithCGImage:imageResource->image scale:imageScale orientation:orientation];

        [[UINavigationBar appearance] setBackgroundImage:image
                                           forBarMetrics:UIBarMetricsDefault];
        [[UINavigationBar appearance] setBackgroundImage:image
                                           forBarMetrics:UIBarMetricsLandscapePhone];
    }
    else if ([key isEqualToString:@MAW_STACK_SCREEN_TITLE_BACKGROUND_SCALE_MODE])
    {
        if (scaleMode != value)
        {
            if (scaleMode)
                [scaleMode release];

            scaleMode = [[NSString alloc] initWithString: value];
        }
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
	if ([key isEqualToString:@MAW_STACK_SCREEN_IS_SHOWN])
	{
		return [[super isShownProperty] retain];
	}
	else
	{
		return [super getPropertyWithKey:key];
	}
}

/**
 * Check if a given child screen is shown inside this stack screen.
 * @param childScreen Screen to check.
 * @return YES if the childScreen is currently shown, NO otherwise.
 */
- (BOOL)isChildScreenShown:(ScreenWidget*)childScreen
{
	if ([stack count] == 0)
	{
		return NO;
	}
	ScreenWidget* shownScreen = [stack lastObject];
	return ([shownScreen isEqual:childScreen]) ? YES : NO;
}

/**
 * Dealloc method.
 */
- (void)dealloc
{
    if (scaleMode)
        [scaleMode release];

    [stack release];
    [super dealloc];
}

@end

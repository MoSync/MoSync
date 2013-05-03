/*
 The MIT License (MIT)

 Copyright (c) 2013 Gregor Gullwi

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#import "NavBarButtonWidget.h"
#import "UIColorExpanded.h"

@implementation NavBarButtonWidget

- (UIBarButtonItem*) barButtonItem
{
    return barButtonItem;
}

- (id)init
{
    self = [super init];
    if (self)
    {
        [self createAsTextButton];
        self.view = nil;
    }
    return self;
}

- (id)initWithObject: (NSObject*)param
{
    self = [super init];
    if (self)
    {
        NSNumber* buttonTypeNumber = (NSNumber*)param;
        UIBarButtonSystemItem buttonType = (UIBarButtonSystemItem)[buttonTypeNumber intValue];

        if (buttonType < 0xFFFFFF00) // System button
        {
            barButtonItem = [[UIBarButtonItem alloc]
                              initWithBarButtonSystemItem:(UIBarButtonSystemItem)buttonType
                              target:self
                              action:@selector(onTitleBarButtonClicked:)];
        }
        else if (buttonType == 0xFFFFFFFE)  // Widget button
        {
            barButtonItem = nil;
        }
        else
        {
            [self createAsTextButton];
        }

        self.view = nil;
    }
    return self;
}

- (void)createAsTextButton
{
    barButtonItem = [[UIBarButtonItem alloc]
                      initWithTitle:@"Click me!"
                      style:UIBarButtonItemStylePlain
                      target:self
                      action:@selector(onTitleBarButtonClicked:)];
}

- (void)onTitleBarButtonClicked:(id)selector
{
    [super sendEvent:MAW_EVENT_POINTER_PRESSED];
    [super sendEvent:MAW_EVENT_POINTER_RELEASED];
    [super sendEvent:MAW_EVENT_CLICKED];
}

- (int)addChild:(IWidget*)child
{
    if ([_children count] > 0)
    {
        return MAW_RES_ERROR;
    }

    child.isMainWidget = TRUE;

    [super addChild:child toSubview:NO];

    barButtonItem = [[UIBarButtonItem alloc] initWithCustomView:child.view];

    return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value
{
    if([key isEqualToString:@MAW_BUTTON_TEXT])
    {
        barButtonItem.title = value;
    }
    else if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR])
    {
        UIColor* color = [UIColor colorWithHexString:value];
        if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
        [barButtonItem setTintColor:color];
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }
    return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey:(NSString*)key
{
	if ([key isEqualToString:@MAW_BUTTON_TEXT])
	{
		if (barButtonItem != nil)
			return [barButtonItem title];

		return @"";
	}
	else
	{
		return [super getPropertyWithKey:key];
	}
}

- (void)dealloc
{
    [barButtonItem release];
    [super dealloc];
}

@end

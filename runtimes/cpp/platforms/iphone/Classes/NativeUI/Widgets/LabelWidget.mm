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

#import "LabelWidget.h"
#import "UIColorExpanded.h"
#include "Platform.h"

typedef enum VerticalAlignment
{
    VerticalAlignmentTop,
    VerticalAlignmentMiddle,
    VerticalAlignmentBottom,
} VerticalAlignment;

@interface UILabelWithVerticalAlignment : UILabel {
@private
    VerticalAlignment verticalAlignment_;
}

@property (nonatomic, assign) VerticalAlignment verticalAlignment;

@end

@implementation UILabelWithVerticalAlignment

@synthesize verticalAlignment = verticalAlignment_;

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        self.verticalAlignment = VerticalAlignmentMiddle;
    }
    return self;
}

- (void)setVerticalAlignment:(VerticalAlignment)verticalAlignment {
    verticalAlignment_ = verticalAlignment;
    [self setNeedsDisplay];
}

- (CGRect)textRectForBounds:(CGRect)bounds limitedToNumberOfLines:(NSInteger)numberOfLines {
    CGRect textRect = [super textRectForBounds:bounds limitedToNumberOfLines:numberOfLines];
    switch (self.verticalAlignment) {
        case VerticalAlignmentTop:
            textRect.origin.y = bounds.origin.y;
            break;
        case VerticalAlignmentBottom:
            textRect.origin.y = bounds.origin.y + bounds.size.height - textRect.size.height;
            break;
        case VerticalAlignmentMiddle:
            // Fall through.
        default:
            textRect.origin.y = bounds.origin.y + (bounds.size.height - textRect.size.height) / 2.0;
    }
    return textRect;
}

-(CGSize)sizeThatFits:(CGSize)_size {
	CGFloat frameWidth = self.frame.size.width;
    NSString* _text = self.text;
	CGSize size = [_text sizeWithFont:self.font constrainedToSize:CGSizeMake(frameWidth,FLT_MAX) lineBreakMode:self.lineBreakMode ];
	return size;
}

-(void)drawTextInRect:(CGRect)requestedRect {
    CGRect actualRect = [self textRectForBounds:requestedRect limitedToNumberOfLines:self.numberOfLines];
    [super drawTextInRect:actualRect];
}
@end


@implementation LabelWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UILabel* label = [[[UILabelWithVerticalAlignment alloc] initWithFrame:CGRectMake(0, 0, 200, 60)] autorelease];
        label.backgroundColor = [UIColor clearColor];
        self.view = label;

        self.autoSizeWidth = WidgetAutoSizeWrapContent;
        self.autoSizeHeight = WidgetAutoSizeWrapContent;
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
	if([key isEqualToString:@MAW_LABEL_TEXT])
    {
		UILabel* label = (UILabel*) self.view;
		[label setText: value];
		[self layout];
	}
	else if([key isEqualToString:@MAW_LABEL_MAX_NUMBER_OF_LINES])
    {
        TEST_FOR_NEGATIVE_VALUE([value intValue]);
		UILabel* label = (UILabel*) self.view;
		label.numberOfLines = [value intValue];
		[self layout];
	}
	else if([key isEqualToString:@MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT])
    {
		UILabel* label = (UILabel*) self.view;
		if([value isEqualToString:@"left"]) {
			label.textAlignment = UITextAlignmentLeft;
		}
		else if([value isEqualToString:@"center"]) {
			label.textAlignment = UITextAlignmentCenter;
		}
		else if([value isEqualToString:@"right"]) {
			label.textAlignment = UITextAlignmentRight;
		}
        else {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
	}
	else if([key isEqualToString:@MAW_LABEL_TEXT_VERTICAL_ALIGNMENT])
    {
		UILabelWithVerticalAlignment* label = (UILabelWithVerticalAlignment*) self.view;
		if([value isEqualToString:@"top"]) {
			[label setVerticalAlignment:VerticalAlignmentTop];
		}
		else if([value isEqualToString:@"center"]) {
			[label setVerticalAlignment:VerticalAlignmentMiddle];
		}
		else if([value isEqualToString:@"bottom"]) {
			[label setVerticalAlignment:VerticalAlignmentBottom];
		}
        else {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
	}
	else if([key isEqualToString:@MAW_LABEL_FONT_COLOR])
    {
		UILabel* label = (UILabel*) self.view;
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		label.textColor = color;
	}
	else if([key isEqualToString:@MAW_LABEL_FONT_SIZE])
    {
		UILabel* label = (UILabel*) self.view;
		float fontSize = [value floatValue];
        UIFont* currentFont = label.font;
        NSString* currentFontName = [currentFont fontName];
        UIFont* newFont = [UIFont fontWithName:currentFontName size:fontSize];
        [label setFont:newFont];
		[self layout];
	}
    else if([key isEqualToString:@MAW_LABEL_FONT_HANDLE])
    {
		UILabel* label = (UILabel*) self.view;
		UIFont* font = Base::getUIFontObject([value intValue]);
        if (!font)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }

        [label setFont:font];
        [self layout];
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
	if([key isEqualToString:@MAW_LABEL_TEXT])
    {
		UILabel* label = (UILabel*) self.view;
		return [label.text retain];
	}
    else if([key isEqualToString:@MAW_LABEL_MAX_NUMBER_OF_LINES])
    {
		UILabel* label = (UILabel*) self.view;
        return [[[NSNumber numberWithInt: label.numberOfLines] stringValue] retain];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Asks the label to calculate and return the size that best fits itself.
 * @return Wrap size of the label.
 */
- (CGSize)sizeThatFitsForWidget
{
    UILabel* label = (UILabel*) self.view;
    CGSize constrainedSize = self.size;

    // If one of the autosize values is fill or wrap set the according
    // size to maximum.
    if (self.autoSizeHeight != WidgetAutoSizeFixed)
    {
        constrainedSize.height = FLT_MAX;
    }
    if (self.autoSizeWidth != WidgetAutoSizeFixed)
    {
        constrainedSize.width = FLT_MAX;
    }

    // Calculate label size that best fits itself.
	CGSize size = [label.text sizeWithFont:label.font constrainedToSize:constrainedSize
                             lineBreakMode:label.lineBreakMode ];
    return size;
}

@end

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

#import "LabelWidget.h"

typedef enum VerticalAlignment {
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
	CGSize size = [self.text sizeWithFont:self.font constrainedToSize:CGSizeMake(self.frame.size.width,FLT_MAX) lineBreakMode:UILineBreakModeWordWrap ];
	return size;
}

-(void)drawTextInRect:(CGRect)requestedRect {
    CGRect actualRect = [self textRectForBounds:requestedRect limitedToNumberOfLines:self.numberOfLines];
    [super drawTextInRect:actualRect];
}
@end


@implementation LabelWidget

- (id)init {	
	UILabel* label = [[[UILabelWithVerticalAlignment alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	label.opaque = NO;
	view = label;
	label.numberOfLines = 0;
	return [super init];	
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UILabel* label = (UILabel*) view;
		[label setText: value];
		[self layout];
		//[label sizeToFit];
	} 
	else if([key isEqualToString:@"numberOfLines"]) {
		UILabel* label = (UILabel*) view;
		label.numberOfLines = [value intValue];
	} 
	else if([key isEqualToString:@"textHorizontalAlignment"]) {
		UILabel* label = (UILabel*) view;
		if([value isEqualToString:@"left"]) {
			label.textAlignment = UITextAlignmentLeft;
		}
		else if([value isEqualToString:@"center"]) {
			label.textAlignment = UITextAlignmentCenter;
		}
		else if([value isEqualToString:@"right"]) {
			label.textAlignment = UITextAlignmentRight;
		}		
	}
<<<<<<< HEAD
	else if([key isEqualToString:@"verticalAlignment"]) {
		UILabelWithVerticalAlignment* label = (UILabelWithVerticalAlignment*) view;
		if([value isEqualToString:@"top"]) {
			[label setVerticalAlignment:VerticalAlignmentTop];
		}
		else if([value isEqualToString:@"center"]) {
			[label setVerticalAlignment:VerticalAlignmentMiddle];
		}
		else if([value isEqualToString:@"bottom"]) {
			[label setVerticalAlignment:VerticalAlignmentBottom];
		}		
=======
	else if([key isEqualToString:@"textVerticalAlignment"]) {
		// This isn't trivial on iphone.
		
		//UILabel* label = (UILabel*) view;
		//if([value isEqualToString:@"center"]) {
		//}
>>>>>>> 7dc2153ccd8cd8580b7bec8ad4fe08765b2be6f6
	}	
	else if([key isEqualToString:@"fontColor"]) {
		UILabel* label = (UILabel*) view;	
		label.textColor = [UIColor colorWithHexString:value];
	}	
	else if([key isEqualToString:@"fontSize"]) {
		UILabel* label = (UILabel*) view;	
		float fontSize = [value floatValue];
		label.font = [UIFont boldSystemFontOfSize:fontSize];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end

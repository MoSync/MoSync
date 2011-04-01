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

#import "LayoutWidgets.h"
#import "LayoutManagers.h"
#include <helpers/CPP_IX_WIDGET.h>

@implementation AbstractLayoutView (AbstractLayoutViewExpanded)

- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va {
	vAlignment = va;
	[self setNeedsLayout];	
}

- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha {
	hAlignment = ha;
	[self setNeedsLayout];	
}

- (void) setSpacing:(int)_spacing {
	spacing = _spacing;
	[self setNeedsLayout];	
}

- (void) setLeftMargin:(int)_margin {
	leftMargin = _margin;
	[self setNeedsLayout];
}

- (void) setRightMargin:(int)_margin {
	rightMargin = _margin;
	[self setNeedsLayout];
}

- (void) setTopMargin:(int)_margin {
	topMargin = _margin;
	[self setNeedsLayout];
}

- (void) setBottomMargin:(int)_margin {
	bottomMargin = _margin;
	[self setNeedsLayout];
}
@end

MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, VLayoutView)
MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, HLayoutView)

@implementation LinearLayoutBase

- (id)init:(LinearLayoutOrientation)ori {	
	orientation = ori;
	
	if(ori == OrientationVertical) {
		view = [[MoSyncVLayoutView alloc]  initWithFrame:CGRectZero spacing:0
									leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
									hAlignment:UIControlContentHorizontalAlignmentLeft
									vAlignment:UIControlContentVerticalAlignmentTop];
	} else {
		view = [[MoSyncHLayoutView alloc]  initWithFrame:CGRectZero spacing:0
											  leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
											  hAlignment:UIControlContentHorizontalAlignmentLeft
											  vAlignment:UIControlContentVerticalAlignmentTop];		
	}
	
	[view setWidget:self];
	
	id ret = [super init];
	[self setAutoSizeParamX:FILL_PARENT andY:FILL_PARENT];
	return ret;
}

- (void)layoutSubviews:(UIView*)_view {
	if(orientation == OrientationVertical) {
		int numFillParent = 0;
		int heightRemaining = 0;
		for (IWidget *child in children)
		{
			UIView* childView = [child getView];
			
			int viewWidth = childView.frame.size.width; 
			if([child getAutoSizeParamX] == FILL_PARENT) {
				viewWidth = view.frame.size.width;
			}
			else if([child getAutoSizeParamX] == WRAP_CONTENT) {
				viewWidth = [childView sizeThatFits:CGSizeZero].width;
			}
			
			int viewHeight = childView.frame.size.height; 
			if([child getAutoSizeParamY] == FILL_PARENT) {
				numFillParent++;
			}
			else if([child getAutoSizeParamY] == WRAP_CONTENT) {
				viewHeight = [childView sizeThatFits:CGSizeZero].height;
				heightRemaining+=viewHeight;
			} else {
				heightRemaining+=viewHeight;
			}
						
			[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, viewWidth, viewHeight)];
		}
		
		if(numFillParent != 0) {
			int fillParentHeight = (view.frame.size.height - heightRemaining)/numFillParent;
			for (IWidget *child in children)
			{
				if([child getAutoSizeParamY] == FILL_PARENT) {
					UIView* childView = [child getView];
					[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, childView.frame.size.width, fillParentHeight)];
				}
			}
			
		}
			
		[_view superLayoutSubviews];		
		//[super layoutSubviews];
	} else {
		int numFillParent = 0;
		int widthRemaining = 0;
		for (IWidget *child in children)
		{
			UIView* childView = [child getView];
			
			int viewHeight = childView.frame.size.height; 
			if([child getAutoSizeParamY] == FILL_PARENT) {
				viewHeight = view.frame.size.height;
			}
			else if([child getAutoSizeParamY] == WRAP_CONTENT) {
				viewHeight = [childView sizeThatFits:CGSizeZero].height;
			}
			
			int viewWidth = childView.frame.size.width; 
			if([child getAutoSizeParamX] == FILL_PARENT) {
				numFillParent++;
			}
			else if([child getAutoSizeParamX] == WRAP_CONTENT) {
				viewWidth = [childView sizeThatFits:CGSizeZero].width;
				widthRemaining+=viewWidth;
			} else {
				widthRemaining+=viewWidth;
			}
			
			[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, viewWidth, viewHeight)];
		}
		
		if(numFillParent != 0) {
			int fillParentWidth = (view.frame.size.width - widthRemaining)/numFillParent;
			for (IWidget *child in children)
			{
				if([child getAutoSizeParamX] == FILL_PARENT) {
					UIView* childView = [child getView];
					[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, fillParentWidth, childView.frame.size.height)];
				}
			}
			
		}
		
		[_view superLayoutSubviews];
		//[super layoutSubviews];
	}
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"childVerticalAlignment"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		if([value isEqualToString:@"top"])
			[alv setVerticalAlignment:UIControlContentVerticalAlignmentTop];
		else if([value isEqualToString:@"center"])
			[alv setVerticalAlignment:UIControlContentVerticalAlignmentCenter];
		else if([value isEqualToString:@"bottom"])
			[alv setVerticalAlignment:UIControlContentVerticalAlignmentBottom];		
	} else if([key isEqualToString:@"childHorizontalAlignment"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		if([value isEqualToString:@"left"])
			[alv setHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
		else if([value isEqualToString:@"center"])
			[alv setHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
		else if([value isEqualToString:@"right"])
			[alv setHorizontalAlignment:UIControlContentHorizontalAlignmentRight];		
	} else if([key isEqualToString:@"leftMargin"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		[alv setLeftMargin:[value intValue]];
	} else if([key isEqualToString:@"rightMargin"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		[alv setRightMargin:[value intValue]];
	} else if([key isEqualToString:@"topMargin"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		[alv setTopMargin:[value intValue]];
	} else if([key isEqualToString:@"bottomMargin"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		[alv setBottomMargin:[value intValue]];
	} else if([key isEqualToString:@"spacing"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		[alv setSpacing:[value intValue]];
	}
	else if([key isEqualToString:@"isScrollable"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)view;
		alv.scrollEnabled = [value boolValue];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end



@implementation HorizontalLayoutWidget
- (id)init {
	return [super init:OrientationHorizontal];
}
@end

@implementation VerticalLayoutWidget
- (id)init {
	return [super init:OrientationVertical];
}
@end
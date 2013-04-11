/* Copyright (C) 2013 MoSync AB

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

/**
 * @file CustomPickerWidget.mm
 * @author Bogdan Iusco
 *
 * @brief NativeUI CustomPicker widget.
 * The widget is implementat as a UIPickerView.
 */

#import "CustomPickerWidget.h"
#import "HorizontalLayoutWidget.h"
#import "RelativeLayoutWidget.h"
#import "VerticalLayoutWidget.h"

/**
 * Private methods/functions for CustomPickerWidget.
 */
@interface CustomPickerWidget ()

/**
 * Getter and setter for CustomPicker item height.
 */
@property(nonatomic, assign) CGFloat rowHeight;

/**
 * Getter and setter for CustomPicker item width.
 */
@property(nonatomic, assign) CGFloat rowWidth;

@end

@implementation CustomPickerWidget

@synthesize rowHeight;
@synthesize rowWidth;

/**
 * Init function.
 */
- (id)init
{
	self = [super init];
	if (self)
	{
		UIPickerView* pickerView = [[UIPickerView alloc] init];
		pickerView.delegate = self;
		pickerView.dataSource = self;

		self.view = pickerView;
		self.rowWidth = pickerView.frame.size.width;
		self.rowHeight = pickerView.frame.size.height;

		[pickerView release];
		pickerView = nil;
	}
	return self;
}

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added. Its type must be HorizontalLayout,
 * RelativeLayout or VerticalLayout.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not as expected.
 */
- (int)addChild:(IWidget*)child
{
	if ([child class] != [HorizontalLayoutWidget class] &&
		[child class] != [HorizontalLayoutWidget class] &&
		[child class] != [HorizontalLayoutWidget class])
	{
		return MAW_RES_INVALID_LAYOUT;
	}
	[super addChild:child toSubview:NO];
	return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added. Its type must be HorizontalLayout,
 * RelativeLayout or VerticalLayout.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT If its type is not as expected.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index
{
	if ([child class] != [HorizontalLayoutWidget class] &&
		[child class] != [HorizontalLayoutWidget class] &&
		[child class] != [HorizontalLayoutWidget class])
	{
		return MAW_RES_INVALID_LAYOUT;
	}
	return [super insertChild:child atIndex:index toSubview:NO];
}

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild:(IWidget*)child
{
	[super removeChild:child fromSuperview:NO];
}

#pragma mark UIPickerViewDataSource

/**
 * Called by the picker view when it needs the number of components.
 * @param pickerView Object requesting data.
 */
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	return 1;
}

/**
 * Called by the picker view when it needs the number of rows for a specified component.
 * @param pickerView Object requesting data.
 * @param component A number identifying a component.
 */
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
	return [_children count];
}

#pragma mark UIPickerViewDelegate

/**
 * Called by the picker view when it needs the row height to use for drawing row content.
 * @param pickerView Object requesting data.
 * @param component A number identifying a component.
 * @return A float value indicating the height of the row in points.
 */
- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
	return self.rowHeight;
}

/**
 * Called by the picker view when it needs the row width to use for drawing row content.
 * @param pickerView Object requesting data.
 * @param component A number identifying a component.
 * @return A float value indicating the width of the row in points.
 */
- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
	return self.rowWidth;
}

/**
 * Called by the picker view when it needs the view to use for a given row in a given component.
 * @param pickerView Object requesting data.
 * @param component A number identifying a component.
 * @param view A view object that was previously used for this row, but is now hidden and cached by the picker view.
 * @return A view object to use as the content of row.
 */
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view
{
	IWidget* widget = [_children objectAtIndex:row];
	return widget.view;
}

@end

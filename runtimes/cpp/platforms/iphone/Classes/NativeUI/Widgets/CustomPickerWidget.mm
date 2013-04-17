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
#import "NSStringExpanded.h"

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

/**
 * Set the row height of its items.
 * Setter for MAW_CUSTOM_PICKER_ROW_HEIGHT property.
 * @param rowHeightString Height to set in pixels.
 * @return One of the following values:
 * - MAW_RES_OK if the height was set, MAW_RES_INVALID_PROPERTY_VALUE if the given
 * height is invalid.
 */
-(int)setRowHeightProperty:(NSString*)rowHeightString;

/**
 * Set the row width of its items.
 * Setter for MAW_CUSTOM_PICKER_ROW_WIDTH property.
 * @param rowWidthString Width to set in pixels.
 * @return One of the following values:
 * - MAW_RES_OK if the width was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the given width is invalid.
 */
-(int)setRowWidthProperty:(NSString*)rowWidthString;

/**
 * Show / hide the selection indicator.
 * Setter for MAW_CUSTOM_PICKER_SELECTION_INDICATOR property.
 * @param value "true" in order to show it, "false" otherwise.
 * @return One of the following values:
 * - MAW_RES_OK for success.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the given value is invalid.
 */
-(int)showSelectionIndicatorProperty:(NSString*)value;

/**
 * Check if the selection indicator is shown.
 * Getter for MAW_CUSTOM_PICKER_SELECTION_INDICATOR property.
 * @return "true" if it is, "false" otherwise.
 */
-(NSString*)isSelectionIndicatorShown;

/**
 * Set the selected item by index.
 * Setter for MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX.
 * @param indexString The index of the item to select.
 * @return One of the following values:
 * - MAW_RES_OK for success.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the given indexString is invalid.
 */
-(int)setSelectedItemIndexProperty:(NSString*)indexString;

/**
 * Get the index of the selected item.
 * Getter for MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX.
 * @return The index of the item to select.
 * If the widget has no children, #MAW_RES_ERROR will be returned.
 */
-(NSString*)selectedItemIndexProperty;

/**
 * Get the widget view.
 * @return View for this widget.
 */
-(UIPickerView*)pickerView;

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
		[child class] != [VerticalLayoutWidget class] &&
		[child class] != [RelativeLayoutWidget class])
	{
		return MAW_RES_INVALID_LAYOUT;
	}
	[super addChild:child toSubview:NO];
	[[self pickerView] reloadAllComponents];
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
		[child class] != [RelativeLayoutWidget class] &&
		[child class] != [RelativeLayoutWidget class])
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
	int returnValue = MAW_RES_OK;
	if ([key isEqualToString:@MAW_CUSTOM_PICKER_ROW_HEIGHT])
	{
		returnValue = [self setRowHeightProperty:value];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_ROW_WIDTH])
	{
		returnValue = [self setRowWidthProperty:value];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_RELOAD_DATA])
	{
		[[self pickerView] reloadAllComponents];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_SELECTION_INDICATOR])
	{
		returnValue = [self showSelectionIndicatorProperty:value];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX])
	{
		returnValue = [self setSelectedItemIndexProperty:value];
	}
	else
	{
		returnValue = [super setPropertyWithKey:key toValue:value];
	}
	return returnValue;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if ([key isEqualToString:@MAW_CUSTOM_PICKER_ROW_HEIGHT])
	{
		int rowHeightValue = self.rowHeight * getScreenScale();
		return [[NSString alloc] initWithFormat:@"%d", rowHeightValue];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_ROW_WIDTH])
	{
		int rowWidthValue = self.rowWidth * getScreenScale();
		return [[NSString alloc] initWithFormat:@"%d", rowWidthValue];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_SELECTION_INDICATOR])
	{
		return [[self isSelectionIndicatorShown] retain];
	}
	else if ([key isEqualToString:@MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX])
	{
		return [[self selectedItemIndexProperty] retain];
	}
	else
	{
		return [super getPropertyWithKey:key];
	}
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
 * @param view A view object that was previously used for this row, but is now
 * hidden and cached by the picker view.
 * @return A view object to use as the content of row.
 */
- (UIView *)pickerView:(UIPickerView *)pickerView
			viewForRow:(NSInteger)row
		  forComponent:(NSInteger)component
		   reusingView:(UIView *)view
{
	if ([_children count] == 0)
	{
		return nil;
	}
	IWidget* widget = [_children objectAtIndex:row];
	return widget.view;
}

/**
 * Called by the picker view when the user selects a row in a component.
 * @param pickerView Object requesting data.
 * @param row A number identifying a row of component.
 * @param component A number identifying a component.
 */
- (void)pickerView:(UIPickerView *)pickerView
	  didSelectRow:(NSInteger)row
	   inComponent:(NSInteger)component
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CUSTOM_PICKER_ITEM_SELECTED;
	eventData->widgetHandle = self.handle;
	eventData->customPickerItemIndex = row;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

/**
 * Set the row height of its items.
 * Setter for MAW_CUSTOM_PICKER_ROW_HEIGHT property.
 * @param rowHeightString Height to set in pixels.
 * @return One of the following values:
 * - MAW_RES_OK if the height was set, MAW_RES_INVALID_PROPERTY_VALUE if the given
 * height is invalid.
 */
-(int)setRowHeightProperty:(NSString*)rowHeightString
{
	if (![rowHeightString canParseNumber])
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}

	self.rowHeight = [rowHeightString intValue] / getScreenScale();
	[[self pickerView] reloadAllComponents];
	return MAW_RES_OK;
}

/**
 * Set the row width of its items.
 * Setter for MAW_CUSTOM_PICKER_ROW_WIDTH property.
 * @param rowWidthString Width to set in pixels.
 * @return One of the following values:
 * - MAW_RES_OK if the width was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the given width is invalid.
 */
-(int)setRowWidthProperty:(NSString*)rowWidthString
{
	if (![rowWidthString canParseNumber])
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}

	self.rowWidth = [rowWidthString intValue] / getScreenScale();
	[[self pickerView] reloadAllComponents];
	return MAW_RES_OK;
}

/**
 * Show / hide the selection indicator.
 * Setter for MAW_CUSTOM_PICKER_SELECTION_INDICATOR property.
 * @param value "true" in order to show it, "false" otherwise.
 * @return One of the following values:
 * - MAW_RES_OK for success.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the given value is invalid.
 */
-(int)showSelectionIndicatorProperty:(NSString*)value
{
	if ([value isEqualToString:kWidgetTrueValue])
	{
		[self pickerView].showsSelectionIndicator = YES;
	}
	else if ([value isEqualToString:kWidgetFalseValue])
	{
		[self pickerView].showsSelectionIndicator = NO;
	}
	else
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}
	return MAW_RES_OK;
}

/**
 * Check if the selection indicator is shown.
 * Getter for MAW_CUSTOM_PICKER_SELECTION_INDICATOR property.
 * @return "true" if it is, "false" otherwise.
 */
-(NSString*)isSelectionIndicatorShown
{
	BOOL isShown = [[self pickerView] showsSelectionIndicator];
	return isShown ? kWidgetTrueValue : kWidgetFalseValue;
}

/**
 * Set the selected item by index.
 * Setter for MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX.
 * @param indexString The index of the item to select.
 * @return One of the following values:
 * - MAW_RES_OK for success.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the given indexString is invalid.
 */
-(int)setSelectedItemIndexProperty:(NSString*)indexString
{
	if (![indexString canParseNumber])
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}
	int index = [indexString intValue];
	if (index < 0 || index >= [_children count])
	{
		return MAW_RES_INVALID_PROPERTY_VALUE;
	}

	[[self pickerView] selectRow:index inComponent:0 animated:YES];
	return MAW_RES_OK;
}

/**
 * Get the index of the selected item.
 * Getter for MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX.
 * @return The index of the item to select.
 * If the widget has no children, #MAW_RES_ERROR will be returned.
 */
-(NSString*)selectedItemIndexProperty
{
	int selectedRow = [[self pickerView] selectedRowInComponent:0];
	// Check if no row is selected.
	if (selectedRow == -1)
	{
		selectedRow = MAW_RES_ERROR;
	}
	return [NSString stringWithFormat:@"%d", selectedRow];
}

/**
 * Get the widget view.
 * @return View for this widget.
 */
-(UIPickerView*)pickerView
{
	return (UIPickerView*)self.view;
}

@end

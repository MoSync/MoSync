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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "WidgetUtils.h"

/**
 * Base class for all Native UI widgets.
 */
@interface IWidget : NSObject
{
    @protected
    /**
     * Contains view's subviews.
     */
	NSMutableArray* _children;

    @private
    /**
     * Widget's represention on iOS platform.
     */
    UIView* _view;

    /**
     * Widget's parent.
     * If the widget doesn't have an parent this value will be nil.
     * A widget can have only one parent.
     */
    IWidget* _parent;

    /**
     * Widget's handle.
     * It must be set after creating the widget.
     * After that its value should not be modified.
     */
    MAHandle _handle;

    /**
     * Auto size enum for width.
     */
    WidgetAutoSizeType _autoSizeWidth;

    /**
     * Auto size enum for height.
     */
    WidgetAutoSizeType _autoSizeHeight;
}

/**
 * Setter and getter for widget's view.
 */
@property(nonatomic, retain) UIView* view;

/**
 * Setter and getter for widget's parent.
 */
@property(nonatomic, assign) IWidget* parent;

/**
 * Setter and getter for widget's handle.
 */
@property(nonatomic, assign) MAHandle handle;

/**
 * Setter and getter for widget's main widget property.
 */
@property(nonatomic, assign) BOOL isMainWidget;

/**
 * Setter and getter for widget's width.
 */
@property(nonatomic, setter = setWidth:, getter = width) CGFloat width;

/**
 * Setter and getter for widget's height.
 */
@property(nonatomic, setter = setHeight:, getter = height) CGFloat height;

/**
 * Setter and getter for widget's view.
 */
@property(nonatomic, setter = setSize:, getter = size) CGSize size;

/**
 * Setter and getter for widget's auto size width value.
 * Will trigger a layout.
 */
@property(nonatomic, assign) WidgetAutoSizeType autoSizeWidth;

/**
 * Setter and getter for widget's auto size height value.
 * Will trigger a layout.
 */
@property(nonatomic, assign) WidgetAutoSizeType autoSizeHeight;

/**
 * Setter and getter for widget's x coordinate.
 * This value will be used only if its parent is an RelativeLayoutWidget.
 */
@property(nonatomic, setter = setOriginX:, getter = originX) CGFloat originX;

/**
 * Setter and getter for widget's y coordinate.
 * This value will be used only if its parent is an RelativeLayoutWidget.
 */
@property(nonatomic, setter = setOriginY:, getter = originY) CGFloat originY;

/**
 * Set widget's auto size width and height values.
 * Will trigger a layout.
 * @param autoSizeWidth Width auto size value to set.
 * @param autoSizeHeight Height auto size value to set.
 */
- (void) setAutoSizeWidth:(WidgetAutoSizeType)autoSizeWidth
                  height:(WidgetAutoSizeType)autoSizeHeight;

/**
 * Adds an widget to the end of the children list.
 * Override this method to provide functionality.
 * The overrided method should verify if the widget can have children or not.
 * @param child Widget to be added.
 * @return MAW_RES_INVALID_LAYOUT.
 */
- (int)addChild:(IWidget*)child;

/**
 * Adds an widget to the end of the children list.
 * It does not check if the widget can have children.
 * The child will be layouted.
 * @param child Widget to be added.
 * @param toSubview If true child's view will be added as subview, otherwise
 * it will only be added to a list.
 */
- (void)addChild: (IWidget*)child toSubview:(BOOL)toSubview;

/**
 * Insert a widget to a given index.
 * The child will be layouted.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @param toSubview If true child's view will be added as subview, otherwise
 * it will only be added to a list.
 * @return MAW_RES_OK if the child was added, MAW_RES_INVALID_INDEX if the index param
 * was invalid.
 */
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index toSubview:(BOOL)toSubview;

/**
 * Insert a widget to a given index.
 * Override this method to provide functionality.
 * The overrided method should verify if the widget can have children or not.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return MAW_RES_INVALID_LAYOUT.
 */
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index;

/**
 * Remove a widget from its parent(remove _view from its superview).
 * @return MAW_RES_OK.
 */
- (int)remove;

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child;


/**
 * Remove a child from the widget.
 * @param child Widget to remove.
 * @param fromSuperview If true the child's view will be removed its superview,
 * otherwise it will only be removed from the children list.
 */
- (void)removeChild: (IWidget*)child fromSuperview:(bool)fromSuperview;

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

/**
 * Recalculate its size and other widget sizes, if needed.
 */
- (void)layout;

/**
 * Layout its children size.
 * This method should be called by the class generated by widget layouting macro.
 * @param view The view that requested the operation.
 */
-(void) layoutSubviews:(UIView*) view;

/**
 * Asks the widget to calculate and return the size that best fits its subviews.
 * An IWidget object should not have children so the returned value will be the size
 * that fits itself.
 * Override this method if your widget can have children and calculate their size.
 * @return The size that best fits itself.
 */
- (CGSize)sizeThatFitsForWidget;

/**
 * Send a widget event type.
 * @param eventDataType One of the MAW_EVENT constants.
 */
- (void)sendEvent:(const int)eventDataType;

/**
 * When a root screen is shown, this will be called recursively for all widgets.
 */
- (void)show;

/**
 * Setter for MAW_WIDGET_WIDTH.
 * @param value MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_WRAP_CONTENT or
 * an int value greater or equal to zero.
 * @return MAW_RES_OK if the width was set, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setWidthProperty:(NSString*) value;

/**
 * Setter for MAW_WIDGET_HEIGHT.
 * @param value MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_WRAP_CONTENT or
 * an int value greater or equal to zero.
 * @return MAW_RES_OK if the height was set, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setHeightProperty:(NSString*) value;

/**
 * Get the index of a child from the children list.
 * @param child The child object whose index should be returned.
 * @return The index of the child, or NSNotFound if the child param is not
 * an child of the current widget.
 */
- (NSUInteger)indexOfChild:(IWidget*)child;

@end

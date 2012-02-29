/*
 Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

/**
 * @file SegmentedListViewItemWidget.mm
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListViewItemWidget widget implementation file.
 * A SegmentedListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in SegmentedListViewWidget objects.
 */


#import "SegmentedListViewItemWidget.h"

@implementation SegmentedListViewItemWidget

@synthesize cell = _cell;
@synthesize height = _height;

static NSString* kReuseIdentifier = @"Cell";

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier:kReuseIdentifier];
        _height = _cell.bounds.size.height;
        view = _cell;
    }

    return self;
}

/**
 * Sets a property.
 * @param key The property of the widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    return [super setPropertyWithKey:key toValue:value];
}

/**
 * Returns a property value of the widget.
 * @param key The property of the widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    return [super getPropertyWithKey:key];
}

/**
 * Release all member data.
 */
-(void) dealloc
{
    // Do NOT release _cell object here. It will be released by the super class.

    [super dealloc];
}

@end

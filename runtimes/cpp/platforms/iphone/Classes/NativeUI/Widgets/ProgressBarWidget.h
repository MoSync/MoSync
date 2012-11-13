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
#import "IWidget.h"

/**
 * Displays a progress bar to the user.
 */
@interface ProgressBarWidget: IWidget
{
    /**
     * The maximum value of the progress bar.
     */
    float _maxValue;

    /**
     * The progress value(how much of the task has been completed).
     */
    float _progressValue;
}

/**
 * Sets a progress bar property.
 * @param key The property of the progress bar widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;

/**
 * Returns a property value of the progress bar widget.
 * @param key The property of the progress bar widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

@end

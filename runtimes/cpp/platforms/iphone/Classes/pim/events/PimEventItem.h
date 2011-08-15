/* Copyright (C) 2011 Mobile Sorcery AB
 
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
#import <EventKit/EventKit.h>

#import "PimUtil.h"
#import "PimFieldItem.h"
#import "PimItem.h"

#include "helpers/cpp_defs.h"
#include "helpers/cpp_ix_pim.h"

@interface PimEventItem :  PimItem {
    
    /**
     * Stores the event's data.
     */
    EKEvent* mEvent;
}

/**
 * Init function.
 */
-(id) init;

/**
 * Init function with Calendar event.
 * @param event Calendar event.
 */
-(id) initWithEvent:(EKEvent*) event;

/**
 * Writes all the fields into record.
 * Deletes all item's fields.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) close;

@end

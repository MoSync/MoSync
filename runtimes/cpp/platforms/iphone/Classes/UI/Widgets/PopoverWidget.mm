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

#import "PopoverWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>
#include "MoSyncUISyscalls.h"


@implementation PopoverWidget

- (id)init {
	id res = [super init];
	popoverController = [[UIPopoverController alloc] initWithContentViewController:controller];
	return res;
}

- (int)show {
	[self layout];

	IWidget* shownScreen = [getMoSyncUI() getCurrentlyShownScreen];

	[popoverController presentPopoverFromRect:[shownScreen getView].frame
									   inView:[shownScreen getView]
					 permittedArrowDirections:UIPopoverArrowDirectionAny
									 animated:YES];
	return MAW_RES_OK;
}

- (int)hide {
	[popoverController dismissPopoverAnimated:YES];
	return MAW_RES_OK;
}


- (void)dealloc {
	[popoverController release];
	[super dealloc];
}

@end

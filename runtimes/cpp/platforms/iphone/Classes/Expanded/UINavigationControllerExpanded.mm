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

#import "UINavigationControllerExpanded.h"

@implementation UINavigationController (UINavigationControllerExpanded)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-protocol-method-implementation"

- (UIViewController *)popViewControllerAnimated:(BOOL)animated
{
	NSArray *vcs = self.viewControllers;
	int count = [vcs count];
	UIViewController *newViewController = count>=2?[vcs objectAtIndex:count-2]:nil;
	UIViewController *oldViewController = count>=1?[vcs objectAtIndex:count-1]:nil;
	if(newViewController && oldViewController)
    {
		if ([self.delegate respondsToSelector:@selector(viewControllerWillBePoped)])
        {
			[self.delegate performSelector:@selector(viewControllerWillBePoped)];
		}

		[self popToViewController:newViewController animated:YES];
	}
	return oldViewController;
}

#pragma clang diagnostic pop

@end

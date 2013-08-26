/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

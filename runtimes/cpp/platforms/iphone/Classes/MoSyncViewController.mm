/* Copyright (C) 2010 MoSync AB

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

#import "MoSyncViewController.h"

/**
 * Forward declaration.
 * Return a boolean value indicating whether the view controller supports the specified orientation.
 * Deprecated in iOS 6.0.
 * @param interfaceOrientation The orientation of the app’s user interface after the rotation.
 * @return YES if the view controller auto-rotates its view to the specified orientation, otherwise NO.
 */
BOOL MoSync_IsInterfaceOrientationSupported(UIInterfaceOrientation interfaceOrientation);

/**
 * Forward declaration.
 * Returns all of the interface orientations that the view controller supports.
 * Available in iOS 6.0 and later.
 * @return A mask with supported orientations.
 */
NSUInteger MoSync_SupportedInterfaceOrientations();

/**
 * Forward declaration.
 * Check if the current screen size has changed. If so send EVENT_TYPE_SCREEN_CHANGED event.
 * It's send only for non NativeUI applications. Once the NativeUI module is used
 * this event is not sent.
 * Usually the screen size changes when rotating device from portrait to landscape
 * and the other way around.
 * @param fromOrientation The old orientation of the user interface.
 */
void MoSync_OrientationChanged(UIInterfaceOrientation fromOrientation);

@interface MoSyncView : UIView <UITextFieldDelegate>
@end

@implementation MoSyncViewController

-(id)init
{
	self = [super init];
	if (self)
	{
		mosyncView = [[MoSyncView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
		self.view = mosyncView;
	}
	return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	[super loadView];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (UIView*) getMoSyncView {
	return mosyncView;
}

/**
 * Return a boolean value indicating whether the view controller supports the specified orientation.
 * Deprecated in iOS 6.0.
 * @param interfaceOrientation The orientation of the app’s user interface after the rotation.
 * @return YES if the view controller auto-rotates its view to the specified orientation, otherwise NO.
 */
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return MoSync_IsInterfaceOrientationSupported(interfaceOrientation);
}

/**
 * Returns whether the view controller’s contents should auto rotate.
 * Available in iOS 6.0 and later.
 * @return YES.
 */
-(BOOL)shouldAutorotate
{
	return YES;
}

/**
 * Returns all of the interface orientations that the view controller supports.
 * Available in iOS 6.0 and later.
 * @return A mask with supported orientations.
 */
- (NSUInteger)supportedInterfaceOrientations
{
	NSUInteger orientations = MoSync_SupportedInterfaceOrientations();
    return orientations;
}

/**
 * Called after the user interface rotates.
 * @param fromInterfaceOrientation The old orientation of the user interface.
 */
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	MoSync_OrientationChanged(fromInterfaceOrientation);
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (void)dealloc {
    [super dealloc];
}

@end

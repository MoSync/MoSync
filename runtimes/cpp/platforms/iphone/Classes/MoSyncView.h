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

#import <UIKit/UIKit.h>
#import "MoSyncCLController.h"
#include <helpers/cpp_defs.h>
#include "TouchHelper.h"
#include "MoSyncSensor.h"

// todo: remove all the functions that don't have to be here

@interface MoSyncView : UIView <UITextFieldDelegate> {
	CGImageRef mosyncView;
    MoSyncCLController *locationController;
	TouchHelper* touchHelper;
    /**
     * Used for handling sensors.
     */
    MoSyncSensor* moSyncSensor;

}

-(void) updateMoSyncView: (CGImageRef)ref;
-(void) showMessageBox:(NSString*)msg withTitle:(NSString*)title shouldKill:(bool)kill;
-(void) showAlert:(NSString*)msg withTitle:(NSString*)title
	 button1Title:(NSString*)buton1
	 button2Title:(NSString*)buton2
	 button3Title:(NSString*)buton3;
-(void) showTextBox:(NSString*)title
			withInText:(NSString*)inText
			outText:(wchar*)outText
			maxSize:(int)maxSize
			andConstraints:(int)constraints;
-(void) startUpdatingLocation;
-(void) stopUpdatingLocation;
- (void)deviceOrientationChanged:(NSNotification *)notification;

/**
 * Returns a pointer to the MoSyncSensor object.
 */
-(MoSyncSensor*) getMoSyncSensor;

@end

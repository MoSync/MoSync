//
//  MoSyncUI.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IWidget.h"

typedef int MAHandle;

@interface MoSyncUI : NSObject {

}

- (id)init;
- (void)close;

- (MAHandle)createWidget: (NSString*)name;
- (IWidget*)getWidget: (MAHandle) handle;
- (void)removeWidget: (MAHandle) handle;

@end

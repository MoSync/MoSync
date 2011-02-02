//
//  NSObjectExpanded.h
//  MoSync
//
//  Created by Niklas Nummelin on 12/10/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSObject (NSObjectExpanded)

- (void) performSelectorOnMainThread:(SEL)selector
						  withTarget:(id)target
						 withObjects:(NSArray*)objects
					   waitUntilDone:(BOOL)waitUntilDone;
@end

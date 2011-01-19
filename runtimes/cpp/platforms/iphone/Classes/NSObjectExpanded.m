//
//  NSObjectExpanded.m
//  MoSync
//
//  Created by Niklas Nummelin on 12/10/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NSObjectExpanded.h"

@interface FIXCATEGORYBUG_NSObjectExpanded
@end
@implementation FIXCATEGORYBUG_NSObjectExpanded
@end


@implementation NSObject (NSObjectExpanded)

- (void) performSelectorOnMainThread:(SEL)selector
						  withTarget:(id)target
						 withObjects:(NSArray*)objects
					   waitUntilDone:(BOOL)waitUntilDone {
	
    if(target && [target respondsToSelector:selector]) {
        NSMethodSignature* signature
		= [target methodSignatureForSelector:selector];
        if(signature) {
			NSInvocation* invocation
			= [NSInvocation invocationWithMethodSignature:signature];
			
            @try {
                [invocation setTarget:target];
				[invocation setSelector:selector];
				
                if(objects) {
					NSInteger objectsCount	= [objects count];
					
					for(NSInteger i=0; i < objectsCount; i++) {
						NSObject* obj = [objects objectAtIndex:i];
						[invocation setArgument:&obj atIndex:i+2];
					}
				}
				
				[invocation retainArguments];
				[invocation performSelectorOnMainThread:@selector(invoke)
												 withObject:nil
											  waitUntilDone:YES];
			}
			@catch (NSException * e) {
				//LOGEXCEPTION(e)
			}
			@finally {
				//[signature release];
				//[invocation release];
			}
        }
    }
}

@end

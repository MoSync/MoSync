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
					   waitUntilDone:(BOOL)waitUntilDone
                      andReturnValue:(void*)retLoc {

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
				//[invocation retainArguments];
				[invocation performSelectorOnMainThread:@selector(invoke)
												 withObject:nil
											  waitUntilDone:waitUntilDone];
				if(retLoc!=nil)
					[invocation getReturnValue:retLoc];
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

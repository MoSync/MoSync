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

#import <Foundation/Foundation.h>

@interface MoSyncPanic :  NSObject {
    /**
     * If set throws a panic when calling error function.
     */
    bool mThrowPanic;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(MoSyncPanic*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Set the flag for throwing panics.
 * @param value If true a panic will be raised when calling error function.
 */
-(void) setThowPanic:(bool) value;

/**
 * Throws a panic if the panic flag is set. Otherwise returns the error code.
 * @param errorCode The error code value.
 * @param panicCode The panic code that will be raised.
 * @param panicText The panic text that will be raised.
 * @return The error code if the panic was not raised.
 */
-(int) error:(int) errorCode
withPanicCode:(int) panicCode
withPanicText:(NSString*) panicText;

@end

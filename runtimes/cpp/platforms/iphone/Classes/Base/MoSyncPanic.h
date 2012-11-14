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

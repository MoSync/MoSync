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

/**
 * @file NSStringExpanded.h
 * @author Bogdan Iusco
 * @date 22 Nov 2011
 *
 * @brief Add new methods to NSString class.
 */

#import "NSStringExpanded.h"

@implementation NSString (NSStringExpanded)

/**
 * Checks if string can be converted into number.
 * Only [0-9] and "." characters are allowed.
 * @return True if it can be converted, false otherwise.
 */
-(bool) canParseNumber
{
    NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
    [formatter setNumberStyle:NSNumberFormatterDecimalStyle];
    NSNumber* number = [formatter numberFromString:self];
    [formatter release];

    if(number)
    {
        return true;
    }

    return false;
}

/**
 * Returns a string created by copying the data from a given C array of UTF16-encoded bytes.
 * @param bytes A NULL-terminated C array of bytes in UTF16 encoding.
 * @return A string created by copying the data from bytes.
 */
+(NSString*)stringWithUTF16String:(const wchar*) bytes
{
    void* address = (void*) bytes;
    int wcharSize = sizeof(wchar);
    NSString* returnedValue = @"";
    while (true)
    {
        NSString* utf16Char = [[NSString alloc] initWithBytes:address
                                                      length:wcharSize
                                                    encoding:NSUTF16LittleEndianStringEncoding];

        if ([utf16Char isEqualToString:@"\0"])
        {
            break;
        }
        address = (wchar*)address + 1;
        returnedValue = [returnedValue stringByAppendingString:utf16Char];
        [utf16Char release];
        utf16Char = nil;
    }

    return returnedValue;
}

@end

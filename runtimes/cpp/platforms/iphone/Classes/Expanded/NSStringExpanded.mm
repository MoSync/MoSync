/*
 Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
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

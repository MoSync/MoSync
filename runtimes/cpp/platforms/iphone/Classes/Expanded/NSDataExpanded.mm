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
 * @file NSDataExpanded.mm
 * @author Bogdan Iusco
 * @date 10 Apr 2011
 *
 * @brief Add new methods to NSData class.
 */
#import "NSDataExpanded.h"

@implementation NSData (Base64Encoding)

- (NSString*)base64Encode
{
    static char table [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

    NSInteger length = [self length];
    NSMutableData* data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
    uint8_t* input = (uint8_t*)[self bytes];
    uint8_t* output = (uint8_t*)data.mutableBytes;

    for (NSInteger i = 0; i < length; i += 3)
    {
        NSInteger value = 0;
        for (NSInteger j = i; j < (i + 3); ++j)
        {
            value <<= 8;

            if (j < length)
            {
                value |= (0xff & input[j]);
            }
        }

        NSInteger index = (i / 3) * 4;
        output[index + 0] =                    table[(value >> 18) & 0x3f];
        output[index + 1] =                    table[(value >> 12) & 0x3f];
        output[index + 2] = (i + 1) < length ? table[(value >> 6) & 0x3f] : '=';
        output[index + 3] = (i + 2) < length ? table[(value >> 0) & 0x3f] : '=';
    }

    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}

@end

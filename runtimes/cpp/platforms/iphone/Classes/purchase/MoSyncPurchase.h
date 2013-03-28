/*
 Copyright (C) 2012 MoSync AB

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

#import <Foundation/Foundation.h>

#include <helpers/cpp_defs.h>
#include "Syscall.h"

SYSCALL(int, maPurchaseSupported());
SYSCALL(void, maPurchaseCreate(MAHandle productHandle, const char* productID));
SYSCALL(int, maPurchaseDestroy(MAHandle productHandle));
SYSCALL(void, maPurchaseRequest(MAHandle productHandle, const int quantity));
SYSCALL(int, maPurchaseGetName(MAHandle productHandle, char* buffer, const int bufferSize));
SYSCALL(void, maPurchaseSetStoreURL(const char* url));
SYSCALL(void, maPurchaseVerifyReceipt(MAHandle productHandle));
SYSCALL(int, maPurchaseGetField(MAHandle productHandle, const char* fieldName,
								char* buffer, const int bufferSize));
SYSCALL(void, maPurchaseRestoreTransactions());
SYSCALL(void, maPurchaseVerifyReceipt(MAHandle productHandle));

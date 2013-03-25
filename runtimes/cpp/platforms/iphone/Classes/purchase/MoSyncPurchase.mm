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

#import "MoSyncPurchase.h"
#import "PurchaseManager.h"

SYSCALL(int, maPurchaseSupported())
{
	return [[PurchaseManager getInstance] isPurchaseSupported];
}

SYSCALL(void, maPurchaseCreate(MAHandle productHandle, const char* productID))
{
	[[PurchaseManager getInstance] createProduct:productHandle productID:productID];
}

SYSCALL(int, maPurchaseDestroy(MAHandle productHandle))
{
	return [[PurchaseManager getInstance] destroyProduct:productHandle];
}

SYSCALL(void, maPurchaseRequest(MAHandle productHandle, const int quantity))
{
	[[PurchaseManager getInstance] requestProduct:productHandle quantity:quantity];
}

SYSCALL(int, maPurchaseGetName(MAHandle productHandle, char* buffer, const int bufferSize))
{
	return [[PurchaseManager getInstance] productName:productHandle
											   buffer:buffer
										   bufferSize:bufferSize];
}

SYSCALL(void, maPurchaseSetStoreURL(const char* url))
{
	[[PurchaseManager getInstance] setStoreURL:url];
}

SYSCALL(void, maPurchaseVerifyReceipt(MAHandle productHandle))
{
	[[PurchaseManager getInstance] verifyReceipt:productHandle];
}

SYSCALL(int, maPurchaseGetField(MAHandle productHandle, const char* fieldName,
								char* buffer, const int bufferSize))
{
	return [[PurchaseManager getInstance] getReceiptField:productHandle
												fieldName:fieldName
												   buffer:buffer
											   bufferSize:bufferSize];
}

SYSCALL(void, maPurchaseRestoreTransactions())
{
	[[PurchaseManager getInstance] restoreTransactions];
}

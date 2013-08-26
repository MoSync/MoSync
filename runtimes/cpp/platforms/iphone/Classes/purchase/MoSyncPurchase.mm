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

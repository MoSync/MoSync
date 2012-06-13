/* Copyright (C) 2012 MoSync AB

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
package com.mosync.internal.android.billing;

/**
 * This class notifies the PurchaseManager.
 * The callbacks are sent from the BillingService.
 * @author emma
 */
public abstract class BillingListener
{

    public static void onTransactionInformationReceived(PurchaseInformation purchase){};

    public static void onPurchaseRestored(PurchaseInformation purchase){};

    public static void onPurchaseRefunded(PurchaseInformation purchase){};

    public static void onPurchaseStateChanged(int state, int handle, int error){};

    public static void onReceiptEvent(int[] event, int handle){};
}
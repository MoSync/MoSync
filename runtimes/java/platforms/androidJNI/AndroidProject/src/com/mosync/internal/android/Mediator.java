/* Copyright (C) 2011 MoSync AB

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

package com.mosync.internal.android;

import android.os.Handler;
import android.os.Message;

/**
 * This class acts like a mediator (see Mediator design pattern).
 * The idea is that this class is a hub for communication between objects.
 * TODO: Move other relevant communication here.
 * @author Mikael Kindborg
 */
public class Mediator
{
	/**
	 * Constant for the request code for enabling Bluetooth.
	 */
	public static final int REQUEST_ENABLE_BLUETOOTH = 1;
	
	/**
	 * Holder of singleton instance.
	 */
	private static Mediator sTheBlackboard = null;
	
	/** 
	 * Bluetooth enable handler.
	 */
	public Handler mBluetoothEnableMessageHandler = null;
	
	/**
	 * Get singleton instance.
	 * Instance is created lazily.
	 * @return The single instance of this class.
	 */
	public static Mediator getInstance()
	{
		if (null == sTheBlackboard)
		{
			sTheBlackboard = new Mediator();
		}
		
		return sTheBlackboard;
	}
	
	/**
	 * Post a message to the Bluetooth enable handler.
	 */
	public void postBluetoothDialogClosedMessage()
	{
		if (null != mBluetoothEnableMessageHandler)
		{
			Message message = new Message();
			message.arg1 = REQUEST_ENABLE_BLUETOOTH;
			mBluetoothEnableMessageHandler.sendMessage(message);
		}
	}
}

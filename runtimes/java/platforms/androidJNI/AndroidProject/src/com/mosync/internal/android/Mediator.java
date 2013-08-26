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

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

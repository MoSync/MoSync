/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.CONNERR_GENERIC;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_SMS;
import static com.mosync.internal.generated.MAAPI_consts.MA_SMS_RESULT_DELIVERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_SMS_RESULT_NOT_DELIVERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_SMS_RESULT_NOT_SENT;
import static com.mosync.internal.generated.MAAPI_consts.MA_SMS_RESULT_SENT;

import java.util.List;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.telephony.SmsManager;
import android.util.Log;

public class MoSyncSMS
{
	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	public MoSyncSMS(MoSyncThread moSyncThread)
	{
		mMoSyncThread = moSyncThread;
	}

	/**
	 * Send an SMS (text message).
	 *
	 * @param phoneNo The destination phone number.
	 * @param message The text message.
	 * @return 0 on success, <0 on error.
	 *
	 * Thanks to Wei-Meng Lee at weimenglee@learn2develop.net for
	 * this useful article:
	 * http://mobiforge.com/developing/story/sms-messaging-android
	 */
	public int maSendTextSMS(String phoneNo, String message)
	{
		String SMS_SENT = "SMS_SENT";
		String SMS_DELIVERED = "SMS_DELIVERED";

		final Activity activity = mMoSyncThread.getActivity();

		PendingIntent sentIntent = PendingIntent.getBroadcast(
			activity,
			0,
			new Intent(SMS_SENT),
			0);

		PendingIntent deliveredIntent = PendingIntent.getBroadcast(
			activity,
			0,
			new Intent(SMS_DELIVERED),
			0);

		// Called to communicate SMS send status.
		// TODO: Post EVENT_TYPE_SMS
		activity.registerReceiver(new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context arg0, Intent arg1)
			{
				if (Activity.RESULT_OK == getResultCode())
				{
					//Log.i("@@@ MoSync", "SMS sent");
					postSMSEvent(MA_SMS_RESULT_SENT);
				}
				else
				{
					//Log.i("@@@ MoSync", "SMS not sent");
					postSMSEvent(MA_SMS_RESULT_NOT_SENT);
				}

				// Display a log message with more detailed info.
				// TODO: Remove when debugging is done.
				switch (getResultCode())
				{
					case Activity.RESULT_OK:
						//Log.i("@@@ MoSync", "SMS sent RESULT_OK");
						break;
					case SmsManager.RESULT_ERROR_GENERIC_FAILURE:
						//Log.i("@@@ MoSync", "SMS sent RESULT_ERROR_GENERIC_FAILURE");
						break;
					case SmsManager.RESULT_ERROR_NO_SERVICE:
						//Log.i("@@@ MoSync", "SMS sent RESULT_ERROR_NO_SERVICE");
						break;
					case SmsManager.RESULT_ERROR_NULL_PDU:
						//Log.i("@@@ MoSync", "SMS sent RESULT_ERROR_NULL_PDU");
						break;
					case SmsManager.RESULT_ERROR_RADIO_OFF:
						//Log.i("@@@ MoSync", "SMS sent RESULT_ERROR_RADIO_OFF");
						break;
				}
			}
		},
		new IntentFilter(SMS_SENT));

		// Called to communicate SMS delivery status.
		// TODO: Post EVENT_TYPE_SMS
		activity.registerReceiver(new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context arg0, Intent arg1)
			{
				if (Activity.RESULT_OK == getResultCode())
				{
					//Log.i("@@@ MoSync", "SMS delivered");
					postSMSEvent(MA_SMS_RESULT_DELIVERED);
				}
				else
				{
					//Log.i("@@@ MoSync", "SMS not delivered");
					postSMSEvent(MA_SMS_RESULT_NOT_DELIVERED);
				}
			}
		},
		new IntentFilter(SMS_DELIVERED));

		try
		{
			SmsManager smsManager = SmsManager.getDefault();
			List<String> messages = smsManager.divideMessage(message);
			for (String messagePart : messages)
			{
				smsManager.sendTextMessage(
					phoneNo,
					null,
					messagePart,
					sentIntent,
					deliveredIntent);
			}

			// Return success.
			return 0;

		}
		catch (Throwable error)
		{
			return CONNERR_GENERIC;
		}
	}

	/**
	 * Send event.
	 * @param status The SMS status constant.
	 */
	private void postSMSEvent(int status)
	{
		int[] event = new int[2];
		event[0] = EVENT_TYPE_SMS;
		event[1] = status;
		mMoSyncThread.postEvent(event);
	}
}

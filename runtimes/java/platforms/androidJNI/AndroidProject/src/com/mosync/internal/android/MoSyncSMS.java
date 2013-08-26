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

//package com.google.android.c2dm;
//
//import com.google.android.c2dm.C2DMBaseReceiver;
//import com.mosync.java.android.MoSync;
//
//import android.app.Notification;
//import android.app.NotificationManager;
//import android.app.PendingIntent;
//import android.content.Context;
//import android.content.Intent;
//import android.util.Log;
//
////import com.google.android.c2dm.C2DMBaseReceiver;
//
//public class C2DMReceiver extends C2DMBaseReceiver {
//	public C2DMReceiver() {
//		// Email address currently not used by the C2DM Messaging framework
//		super("dummy@google.com");
////		super("emma.tresanszki@mobilesorcery.com");
//	}
//
//	@Override
//	public void onRegistered(Context context, String registrationId)
//			throws java.io.IOException {
//		Log.e("C2DM", "Registration ID arrived: Fantastic!!!");
//		Log.e("C2DM", registrationId);
//	};
//
//	@Override
//	protected void onMessage(Context context, Intent intent) {
//		Log.e("C2DM", "Message: Fantastic!!!");
//
//		// create new PushNotificationObject that holds the payload.
//
//		// TODO raise event
//
//		// Launch local notification
//		final String message = intent.getStringExtra("payload");
//		createNotification(context, message);
//	}
//
//	@Override
//	public void onError(Context context, String errorId) {
//		Log.e("C2DM", "Error occured!!!");
//	}
//
//	public void createNotification(Context context, String message)
//	{
//		NotificationManager notificationManager = (NotificationManager) context
//				.getSystemService(Context.NOTIFICATION_SERVICE);
//		int icon = getResources().getIdentifier(
//				"icon",
//				"drawable",
//				getPackageName());
//		Notification notification = new Notification(icon,
//				"MoSync Notification", System.currentTimeMillis());
//		// Hide the notification after its selected
//		notification.flags |= Notification.FLAG_AUTO_CANCEL;
//
//		Intent intent = new Intent(context, MoSync.class);
//		intent.putExtra("payload", message);
//		PendingIntent pendingIntent = PendingIntent.getActivity(context, 0,
//				intent, 0);
//		notification.setLatestEventInfo(context, "Message received",
//				message, pendingIntent);
//		notificationManager.notify(0, notification);
//	}
//
//}
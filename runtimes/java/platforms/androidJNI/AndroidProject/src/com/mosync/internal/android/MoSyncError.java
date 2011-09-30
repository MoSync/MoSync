package com.mosync.internal.android;

public class MoSyncError {

	private static MoSyncError sObject;
	private MoSyncThread mThread;
	boolean mFlag = false;

	private MoSyncError() {
	}

	public static MoSyncError getSingletonObject() {
		if (sObject == null) {
			sObject = new MoSyncError();
		}
		return sObject;
	}

	public void setThread(MoSyncThread thread) {
		mThread = thread;
	}

	public void setFlag(boolean flag) {
		mFlag = flag;
	}

	public int error(int errorCode, int panicCode, String panicText) {
		if (mFlag) {
			mThread.maPanic(panicCode, panicText);
		}

		return errorCode;
	}
}

package com.mosync.internal.android;

public class SingletonObject {

	private static SingletonObject singletonObject;
	private MoSyncThread mMoSyncThread;
	boolean mFlag;

	private SingletonObject() {
	}

	public static SingletonObject getSingletonObject() {
		if (singletonObject == null) {
			singletonObject = new SingletonObject();
		}
		return singletonObject;
	}

	public void setThread(MoSyncThread thread) {
		mMoSyncThread = thread;
	}

	public void setFlag(boolean flag) {
		mFlag = flag;
	}

	public int error(int errorCode, int panicCode, String panicText) {
		if (mFlag) {
			mMoSyncThread.maPanic(panicCode, panicText);
		}

		return errorCode;
	}
}

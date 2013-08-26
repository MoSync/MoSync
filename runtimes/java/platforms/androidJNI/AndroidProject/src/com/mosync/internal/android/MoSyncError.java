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

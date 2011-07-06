package com.mosync.internal.android;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import android.app.Activity;

public class MoSyncPIM {

	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}
    
	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncPIM(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}
	
	int maPimListOpen(int listType)
	{
		SYSLOG("Android: maPimListOpen");
		return 0;
	}
	
	int maPimListNext(int list)
	{
		return 0;
	}
	
	int maPimListClose(int list)
	{
		return 0;
	}
	
	int maPimItemCount(int item)
	{
		return 0;
	}
	
	int maPimItemGetField(int item, int n)
	{
		return 0;
	}
	
	int maPimItemFieldCount(int item, int field)
	{
		return 0;
	}
	
	int maPimItemGetAttributes(int item, int field, int index)
	{
		return 0;
	}
	
	int maPimItemSetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		return 0;
	}
	
	int maPimItemGetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		return 0;
	}
	
	int maPimFieldType(int list, int field)
	{
		return 0;
	}
	
	int maPimItemGetValue(int item, int field, int buffPointer, int buffSize, int index)
	{
		return 0;
	}
	
	int maPimItemSetValue(int item, int field, int buffPointer, int buffSize, int index, int attributes)
	{
		return 0;
	}
	
	int maPimItemAddValue(int item, int field, int buffPointer, int buffSize, int attributes)
	{
		return 0;
	}
	
	int maPimItemRemoveValue(int item, int field, int index)
	{
		return 0;
	}
	
	int maPimItemClose(int item)
	{
		return 0;
	}
	
	int maPimItemCreate(int list)
	{
		return 0;
	}
	
	int maPimItemRemove(int list, int item)
	{
		return 0;
	}

}

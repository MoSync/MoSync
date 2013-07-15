package com.mosync.api;

import android.content.Context;

public interface MoSyncContext {

	public Context getActivity();

	public MoSyncExtension getExtension(String name);

}

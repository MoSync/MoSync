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

package com.mosync.java.android;

import com.mosync.internal.android.MoSyncThread;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.widget.LinearLayout;

/**
 * Activity that displays the MoSync panic dialog.
 * @author Anders Malm
 */
public class MoSyncPanicDialog extends Activity
{
	public static String sPanicMessage;

    @Override
    protected void onCreate(Bundle savedInstanceState)
	{
        super.onCreate(savedInstanceState);

		LinearLayout ll = new LinearLayout(this);
		ll.setOrientation(LinearLayout.VERTICAL);
		setContentView(ll);

		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setCancelable(false);
		builder.setMessage(sPanicMessage);
		builder.setPositiveButton("Quit", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int which)
			{
				finish();
			}
		});
		AlertDialog alertDialog = builder.create();
		alertDialog.show();
    }

    @Override
    protected void onStop()
	{
		super.onStop();
		finish();
	}

	@Override
    protected void onRestart()
	{
		super.onRestart();
    }

	@Override
    protected void onResume()
	{
		super.onResume();
    }

	@Override
    protected void onPause()
	{
		super.onPause();
		finish();
    }

	@Override
    protected void onDestroy()
	{
		super.onDestroy();

		MoSyncThread mosyncThread = MoSyncThread.getInstance();
		mosyncThread.exitApplication();

		finish();
    }
}


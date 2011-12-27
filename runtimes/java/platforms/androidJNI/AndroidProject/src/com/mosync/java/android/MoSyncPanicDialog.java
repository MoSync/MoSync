/* Copyright (C) 2010 MoSync AB

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

package com.mosync.java.android;

import com.mosync.internal.android.MoSyncThread;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
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


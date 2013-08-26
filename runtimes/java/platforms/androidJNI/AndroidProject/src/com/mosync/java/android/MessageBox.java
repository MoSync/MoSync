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
public class MessageBox extends Activity
{	
    @Override
    protected void onCreate(Bundle savedInstanceState)
	{
        super.onCreate(savedInstanceState);
        Log.i("MessageBox","onCreate");
        
        // Get parameters from the parent activity
		Bundle bundle = this.getIntent().getExtras();
		String title = bundle.getString("TITLE");
		String text = bundle.getString("TEXT");
		
		LinearLayout linearLayout = new LinearLayout(this);
		linearLayout.setOrientation(LinearLayout.VERTICAL);
		setContentView(linearLayout);
		
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setCancelable(false);
		builder.setMessage(text);
		builder.setPositiveButton("Ok", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int which)
			{
				Log.i("dialog on click", "finish!!");
				finish();
			}
		}); 
		AlertDialog alertDialog = builder.create();
		
		alertDialog.setTitle(title);
		alertDialog.show();
    }
 
    @Override
    protected void onStop()
	{
		super.onStop();
	  	Log.i("MessageBox","onStop");
		finish();
	}

	@Override
    protected void onRestart()
	{
		super.onRestart();
	   	Log.i("MessageBox","onRestart");
    }
	
	@Override
    protected void onResume()
	{
		super.onResume();		
    	Log.i("MessageBox","onResume");
    }

	@Override
    protected void onPause()
	{
		super.onPause();		
    	Log.i("MessageBox","onPause");
		finish();
    }

	@Override
    protected void onDestroy()
	{
		super.onDestroy();
    	Log.i("MessageBox","onDestroy");
		finish();
    }
}


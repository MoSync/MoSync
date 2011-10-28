package com.mosync;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.widget.TextView;

public class MemBenchNativeAndroidActivity extends Activity {
	
	TextView tv;

	// load the library - name matches jni/Android.mk
	static {
		System.loadLibrary("membench");
	}

	// declare the native code function - must match stropbench.cpp
	private native String invokeNativeFunction();

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		tv = (TextView) findViewById(R.id.textview);
		
		//new ProgressDialog.Builder(this).setMessage("Benching, this may take some time!").show();

		// this is where we call the native code
		try{
			invokeNativeFunction();
		}catch(Exception e){
			new AlertDialog.Builder(this).setMessage(e.getMessage()).show();
		}
		new AlertDialog.Builder(this).setMessage("DONE!").show();
	}
	
	public void printMsg(String msg) {
//		new AlertDialog.Builder(this).setMessage(msg).show();
		tv.append(msg);
	}
}
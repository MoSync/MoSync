package com.java.linpack;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.widget.TextView;

public class JavaLinpackActivity extends Activity {

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		TextView tv = new TextView(this);
		setContentView(tv);
		Linpack lp = new Linpack(this);
		tv.setText(lp.bench());

		//System.out.println("DONE!");
		new AlertDialog.Builder(this).setMessage("DONE!").show();
	}


}
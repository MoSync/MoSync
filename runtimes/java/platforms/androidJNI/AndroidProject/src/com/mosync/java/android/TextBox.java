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

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_FOCUS_GAINED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_TEXTBOX;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_FLAG_INITIAL_CAPS_SENTENCE;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_FLAG_INITIAL_CAPS_WORD;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_FLAG_NON_PREDICTIVE;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_FLAG_PASSWORD;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_FLAG_SENSITIVE;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_FLAG_UNEDITABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_RES_CANCEL;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_TYPE_ANY;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_TYPE_DECIMAL;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_TYPE_EMAILADDR;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_TYPE_NUMERIC;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_TYPE_PHONENUMBER;
import static com.mosync.internal.generated.MAAPI_consts.MA_TB_TYPE_URL;

import java.nio.CharBuffer;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.text.InputType;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
* This class is an implementation of a text box dialog.
* It shows a fullscreen text fiels with OK and Cancel buttons.
*
* Clicking OK retrieves the text entered by the user and puts it into
* a buffer specified in the maTextBox IOCtl, pushes a MoSync event and
* then dismisses the dialog.
* Clicking cancel only dismisses the dialog.
*/
public class TextBox extends Activity implements OnClickListener {

	public Handler mHandler;
	private Button mOkButton;
	private Button mCancelButton;
	private EditText mEdit;
	private TextView mLabel;
	private int mOutputMemPtr;
	private int mConstraints;
	
	/**
	* Converts MoSync input constraints android ones.
	*
	* @param mosyncInputConstraints		The MoSync input type to be converted.
	*									See maapi.idl for a list of input types,
	*									section "maTextBox", "constraints".
	* @return							An integer corresponding to the relevant 
	*									android input type.
	*/
	private int convertInputConstraints(int mosyncInputConstraints) {
		int type = mosyncInputConstraints & 0x0000F;
		int flag = mosyncInputConstraints & 0xFF000;
		int androidInputConstraints = 0x00000;
		
		// Process the type
		switch(type) {
			case MA_TB_TYPE_ANY:
				androidInputConstraints = InputType.TYPE_CLASS_TEXT | 
										InputType.TYPE_TEXT_FLAG_MULTI_LINE;
				break;
			case MA_TB_TYPE_EMAILADDR:
				androidInputConstraints = InputType.TYPE_CLASS_TEXT |
										InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
				break;
			case MA_TB_TYPE_NUMERIC:
				androidInputConstraints = InputType.TYPE_CLASS_NUMBER |
										InputType.TYPE_NUMBER_FLAG_SIGNED;
				break;
			case MA_TB_TYPE_PHONENUMBER:
				androidInputConstraints = InputType.TYPE_CLASS_PHONE;
				break;
			case MA_TB_TYPE_URL:
				androidInputConstraints = InputType.TYPE_CLASS_TEXT | 
										InputType.TYPE_TEXT_VARIATION_URI;
				break;
			case MA_TB_TYPE_DECIMAL:
				androidInputConstraints = InputType.TYPE_CLASS_NUMBER |
										InputType.TYPE_NUMBER_FLAG_DECIMAL |
										InputType.TYPE_NUMBER_FLAG_SIGNED;
				break;
			// Default case is normal text with several lines
			default:
				androidInputConstraints = InputType.TYPE_CLASS_TEXT |
										InputType.TYPE_TEXT_FLAG_MULTI_LINE;
				break;
		}
		// Process the flag
		switch(flag) {
			case MA_TB_FLAG_PASSWORD:
				androidInputConstraints |= InputType.TYPE_TEXT_VARIATION_PASSWORD;
				break;
			case MA_TB_FLAG_UNEDITABLE:
				// Uneditable is not available on android
				break;
			case MA_TB_FLAG_SENSITIVE:
				// Sensitive is the same as no suggesions in that context
				// because we do not store the text entered in any kind of dictionary
				
				// OOPS! TYPE_TEXT_FLAG_NO_SUGGESTIONS Not available in android 1.5
				//androidInputConstraints |= InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
				break;
			case MA_TB_FLAG_NON_PREDICTIVE:
				// OOPS! TYPE_TEXT_FLAG_NO_SUGGESTIONS Not available in android 1.5
				//androidInputConstraints |= InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
				break;
			case MA_TB_FLAG_INITIAL_CAPS_WORD:
				androidInputConstraints |= InputType.TYPE_TEXT_FLAG_CAP_WORDS;
				break;
			case MA_TB_FLAG_INITIAL_CAPS_SENTENCE:
				androidInputConstraints |= InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
				break;
			// Default case is still normal text with several lines
			default:
				break;
		}
		
		return androidInputConstraints;
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		// Set a handler to this thread
		mHandler= new Handler();
		
		// Fullscreen mode
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
			WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		// Get screen dimansions
		Display display = getWindowManager().getDefaultDisplay(); 
		int width = display.getWidth();
		int height = display.getHeight();
		
		// Get parameters from the parent activity
		Bundle bundle = this.getIntent().getExtras();
		String title = bundle.getString("TITLE");
		String text = bundle.getString("TEXT");
		mOutputMemPtr = bundle.getInt("OUTPUT");
		mConstraints = bundle.getInt("CONSTRAINTS");
			
		// Initialize layout components
		mEdit = new EditText(this);
		mEdit.setWidth(width);
		mEdit.setHeight(height/3);
		// The line below creates a bug on HTC Desire.
		// mEdit.setGravity(Gravity.TOP);
		mEdit.setText(text);
		mEdit.setInputType( this.convertInputConstraints(mConstraints) );
		mCancelButton = new Button(this);
		mCancelButton.setText("Cancel");
		mCancelButton.setId(1000);
		mCancelButton.setOnClickListener(this);
		mOkButton = new Button(this);
		mOkButton.setText("OK");
		mOkButton.setId(1001);
		mOkButton.setOnClickListener(this);
		mLabel = new TextView(this);
		mLabel.setText(title);
		
		// Add buttons to a sub-layout
		LinearLayout horizontalLayout = new LinearLayout(this);
		horizontalLayout.addView(mCancelButton);
		horizontalLayout.addView(mOkButton);
		
		// Add components to the global layout
		LinearLayout verticalLayout = new LinearLayout(this);
		verticalLayout.setOrientation(LinearLayout.VERTICAL);
		verticalLayout.addView(mLabel);
		verticalLayout.addView(mEdit);
		verticalLayout.addView(horizontalLayout);

		// Show the global layout
		setContentView(verticalLayout);
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		// Send a focus gained event
		// when going back to MoSync view
		int[] event = new int[3];
		event[0] = EVENT_TYPE_FOCUS_GAINED;
		event[1] = 0;
		event[2] = 0;
		MoSync.mMoSyncThread.postEvent(event);
	}
	
	@Override
	protected void onRestart() {
		super.onRestart();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		super.onPause();
	}
	
	@Override
	protected void onStart() {
		super.onStart();
	}

	@Override
	public void onClick(View v) {
		if( v.getId() == mOkButton.getId() ) {
			Log.i("InputBox", "OK clicked.");
			String output = mEdit.getText().toString();
			
			// Write text directly to the MoSync memory
			char[] ca = output.toCharArray();
			MoSync.mMoSyncThread.mMemDataSection.position(mOutputMemPtr);
			
			CharBuffer cb = MoSync.mMoSyncThread.mMemDataSection.asCharBuffer();
			cb.put(ca);
			cb.put((char)0);
			
			// Notice that data is available
			int[] event = new int[3];
			event[0] = EVENT_TYPE_TEXTBOX;
			event[1] = MA_TB_RES_OK;
			event[2] = output.length();
			MoSync.mMoSyncThread.postEvent(event);
			
			Log.i("InputBox", "event" + output.length());
			Log.i("InputBox", "event" + event[1]);
	
			finish();
		}
		if( v.getId() == mCancelButton.getId() ) {
			Log.i("InputBox", "Cancel clicked.");
			String output = mEdit.getText().toString();
			
			// Write text directly to the MoSync memory
			byte[] ba = output.getBytes();
			MoSync.mMoSyncThread.mMemDataSection.position(mOutputMemPtr);
			MoSync.mMoSyncThread.mMemDataSection.put(ba);
			MoSync.mMoSyncThread.mMemDataSection.put((byte)0);
			
			// Notice that the user clicked cancel
			int[] event = new int[3];
			event[0] = EVENT_TYPE_TEXTBOX;
			event[1] = MA_TB_RES_CANCEL;
			event[2] = 0;
			MoSync.mMoSyncThread.postEvent(event);
			
			finish();
		}
	}
}

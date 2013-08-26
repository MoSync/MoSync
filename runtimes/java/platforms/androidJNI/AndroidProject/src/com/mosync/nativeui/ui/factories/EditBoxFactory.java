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

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnFocusChangeListener;
import android.view.View.OnKeyListener;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.EditBoxWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates an editbox that sends a click event
 * when the enter key is pressed.
 * @author fmattias
 */
public class EditBoxFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, final int handle)
	{
		final EditText editBox = new EditText( activity );

		editBox.addTextChangedListener(new TextWatcher() {

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				// Do not send event when edit box receives focus and text is empty.
				if ( count == 0 && before == 0 )
				{
					return;
				}
				EventQueue.getDefault().postWidgetEvent(
						IX_WIDGET.MAW_EVENT_EDIT_BOX_TEXT_CHANGED, handle);
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
			}

			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub
			}
		});

		editBox.setOnFocusChangeListener(new OnFocusChangeListener() {

			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				if (hasFocus)
				{
					EventQueue.getDefault( ).postWidgetEvent(
							IX_WIDGET.MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN, handle );
				}
				else

					EventQueue.getDefault( ).postWidgetEvent(
							IX_WIDGET.MAW_EVENT_EDIT_BOX_EDITING_DID_END, handle );
			}
		});

		editBox.setImeOptions(EditorInfo.IME_ACTION_DONE);
		editBox.setImeOptions(EditorInfo.IME_ACTION_NEXT);

		editBox.setOnEditorActionListener( new OnEditorActionListener() {

			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				if ( editBox == v )
				{
					if ( actionId == EditorInfo.IME_ACTION_DONE ||
						actionId == EditorInfo.IME_ACTION_NEXT)
					{
						EventQueue.getDefault( ).postWidgetEvent(
								IX_WIDGET.MAW_EVENT_EDIT_BOX_RETURN, handle );
					}
				}
				return false;
			}
		});

		// Because HTC implemented their own soft  keyboard which ignores the imeOptions,
		// listen for key events instead.
		editBox.setOnKeyListener( new OnKeyListener( ) {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event)
			{
				if (event.getAction( ) == KeyEvent.ACTION_DOWN
						&& keyCode == KeyEvent.KEYCODE_ENTER )
				{
					EventQueue.getDefault( ).postWidgetEvent(
							IX_WIDGET.MAW_EVENT_EDIT_BOX_RETURN, handle );
				}
				return false;
			}
		});

		editBox.setSingleLine( );

		return new EditBoxWidget( handle, editBox );
	}
}

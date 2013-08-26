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
import android.widget.RatingBar;
import android.widget.RatingBar.OnRatingBarChangeListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.RatingBarWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates an extension of SeekBar and ProgressBar that shows a rating in stars.
 *
 * @author emma
 */
public class RatingBarFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		RatingBar ratingBar = new RatingBar( activity );
		ratingBar.setOnRatingBarChangeListener( new OnRatingBarChangeListener() {

			@Override
			public void onRatingChanged(RatingBar arg0, float rating, boolean fromUser) {
				int intVal = (fromUser) ? 1 : 0;
				EventQueue.getDefault( ).postRatingBarChanged(handle, rating, intVal);
			}
		});

		return new RatingBarWidget( handle, ratingBar);
	}

}
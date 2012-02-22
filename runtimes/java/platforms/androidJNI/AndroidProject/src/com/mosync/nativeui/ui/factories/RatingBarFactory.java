/* Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
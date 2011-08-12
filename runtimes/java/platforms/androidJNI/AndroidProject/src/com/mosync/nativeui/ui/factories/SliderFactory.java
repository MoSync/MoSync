package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.SliderWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates slider and sets up the default
 * behavior for sending events when clicked.
 * 
 * @author emma
 */
public class SliderFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, final int handle)
	{
		SeekBar slider = new SeekBar( activity);
		slider.setOnSeekBarChangeListener( new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				
					EventQueue.getDefault( ).postSeekBarValueChangedEvent(handle, progress);
			}
		});
		
		return new SliderWidget( handle, slider );
	}
}

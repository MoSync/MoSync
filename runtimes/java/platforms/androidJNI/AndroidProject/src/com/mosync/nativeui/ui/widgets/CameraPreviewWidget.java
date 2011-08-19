
package com.mosync.nativeui.ui.widgets;


import android.widget.FrameLayout;

/**
 * Wraps behavior of the camera preview widget.
 * 
 * @author Ali Sarrafi
 *
 */
public class CameraPreviewWidget extends Widget {
	
	private FrameLayout preview;
	
	/**
	 * @param handle Integer handle corresponding to this instance.
	 * @param view A framelayout that is connected to the preview and wrapped by this class.
	 */
	public CameraPreviewWidget(int handle, FrameLayout view) 
	{
		super(handle, view);
		preview = view;
	}

}

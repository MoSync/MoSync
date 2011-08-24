package com.mosync.nativeui.ui.factories;

import android.app.Activity;

import android.widget.FrameLayout;

import com.mosync.nativeui.ui.widgets.CameraPreviewWidget;
import com.mosync.nativeui.ui.widgets.MoSyncCameraPreview;
import com.mosync.nativeui.ui.widgets.Widget;

public class CameraPreviewFactory implements AbstractViewFactory
{

	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		/*
		 * we use an android framelayout to hold the actual 
		 * surface view and prevent it from overlapping with others
		 */
		MoSyncCameraPreview view = new MoSyncCameraPreview(activity);
		FrameLayout frameView = new FrameLayout(activity);
		frameView.addView(view);
		CameraPreviewWidget previewWidget = new CameraPreviewWidget(handle, frameView);
		
		
		return previewWidget;
	}
	
}

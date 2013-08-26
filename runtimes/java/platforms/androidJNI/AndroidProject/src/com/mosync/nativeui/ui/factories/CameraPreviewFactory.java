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
import android.content.pm.PackageManager;

import android.widget.FrameLayout;

import com.mosync.internal.android.MoSyncThread;
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
		MoSyncThread mosyncThread = MoSyncThread.getInstance();

		Boolean isCameraAccessGranted =
				(PackageManager.PERMISSION_GRANTED ==
				mosyncThread.getActivity().checkCallingOrSelfPermission("android.permission.CAMERA"));
		if ( !isCameraAccessGranted )
		{
			// TODO When maPanic works from here discuss if is more suited than maAlert.
			mosyncThread.maAlert("Camera", "Camera permission is not set in the project", "OK", "", "");
		}

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

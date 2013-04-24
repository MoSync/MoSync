/* Copyright (C) 2011 MoSync AB

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

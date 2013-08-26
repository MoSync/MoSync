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

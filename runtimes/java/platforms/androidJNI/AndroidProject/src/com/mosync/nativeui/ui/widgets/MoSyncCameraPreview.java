package com.mosync.nativeui.ui.widgets;

import static com.mosync.internal.android.MoSyncHelpers.DebugIsOn;
import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.android.MoSyncHelpers.MYASSERT;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.*;

import java.io.IOException;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/*
 * An implementation of SurfaceView that is strcitly used for camera purposes
 */
public class MoSyncCameraPreview extends SurfaceView implements SurfaceHolder.Callback {
	
	
    SurfaceHolder mHolder;
    /*
     * An instance of camera Hardware 
     */
    public Camera mCamera;
    
    /**
     * Constructor
     * 
     * @param context
     * @param camera 
     */
    MoSyncCameraPreview(Context context) {
        super(context);
        
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    /**
     * Activates everything when the surface is created
     */
	 public void surfaceCreated(SurfaceHolder holder) 
	 {
	     // The Surface has been created, acquire the camera and tell it where
	     // to draw.
		 mCamera.open();
	        try {
				mCamera.setPreviewDisplay(holder);
				mCamera.setPreviewCallback(new PreviewCallback() {
					public void onPreviewFrame(byte[] data, Camera arg1) {
							MoSyncCameraPreview.this.invalidate();
					}
				});
			} catch (IOException e) {
				e.printStackTrace();
			}
	 }

	 public void surfaceDestroyed(SurfaceHolder holder) 
	 {
	        mCamera.stopPreview();
	        mCamera.release();
	        mCamera = null;
	 }
	 
	 public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) 
	 {
	        // Now that the size is known, set up the camera parameters and begin
	        // the preview.
	        Camera.Parameters parameters = mCamera.getParameters();
	        parameters.setPreviewSize(w, h);
	        try {
	            mCamera.setParameters(parameters);
	        } catch (Exception e)
	        {
	        
	        } finally {
	        }
	        mCamera.startPreview();
    }
}

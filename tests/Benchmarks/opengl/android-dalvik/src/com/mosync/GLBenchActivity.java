package com.mosync;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class GLBenchActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLSurfaceView = new GLSurfaceView(this);
        
        // Set an OpenGL ES 1.x-compatible renderer. 
        mGLSurfaceView.setRenderer(new GLBenchRenderer(this));
        setContentView(mGLSurfaceView);
        
    }
    
    private GLSurfaceView mGLSurfaceView;
}
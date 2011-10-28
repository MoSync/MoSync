package com.mosync;


import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.os.SystemClock;

public class GLBenchRenderer implements GLSurfaceView.Renderer {

	public GLBenchRenderer(GLBenchActivity activity) {
		mNumPolygons = 100;
		mNumPlanes = 2;
		mActivity = activity;
		mResultsActivity = new ResultsActivity();
		mContext = mActivity.getBaseContext();
		mCurrTest = 1; //the test to start with
		mStartTime = currTime(); //set the starting time
		mData = Data.getInstance(mContext);
	}

	/*
	 * Print benchmark statistics to the screen
	 */
	void showStats() {
		mData.addResult("frames: " + mFrameCounter + "\n");
		mData.addResult("msecs: " + mTimeSlot + "\n");
		mData.addResult("FPS: " + mFrameCounter * 1000 / mTimeSlot + "\n");
	}

	/*
	 * Create and bind texture
	 */
	void createTexture(GL10 gl) {
		Matrix flip = new Matrix();
		flip.postScale(1f, -1f);

		// This will tell the BitmapFactory to not scale based on the device's pixel density:
		// (Thanks to Matthew Marshall for this bit)
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inScaled = false;

		//our bitmap created from box.png
		Bitmap bmp = BitmapFactory.decodeResource(mContext.getResources(),
				R.drawable.box, opts); /* pass in opts */

		//array with the number of textures
		mTextures = new int[1];
		gl.glGenTextures(1, mTextures, 0);
		//bind texture
		gl.glBindTexture(GL10.GL_TEXTURE_2D, mTextures[0]);
		GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bmp, 0);

		// Set texture parameters.
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
		gl.glTexParameterx(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		gl.glViewport(0, 0, width, height);

		/*
		 * Set our projection matrix. This doesn't have to be done
		 * each time we draw, but usually a new projection needs to
		 * be set when the viewport is resized.
		 */

		float ratio = (float) width / height;
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		this.gluPerspective(45.0f, ratio, 0.1f, 100.0f, gl);
		//gl.glFrustumf(-ratio, ratio, -1, 1, 1, 10);
	}

	/**
	 * Standard OpenGL utility function for setting up the
	 * perspective projection matrix.
	 */
	void gluPerspective(
			float fovy,
			float aspect,
			float zNear,
			float zFar,
			GL10 gl)
	{
		final float M_PI = 3.14159f;

		float ymax = zNear * (float) Math.tan(fovy * M_PI / 360.0);
		float ymin = -ymax;
		float xmin = ymin * aspect;
		float xmax = ymax * aspect;

		gl.glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	}

	/*
	 * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(javax.microedition.khronos.opengles.GL10)
	 * Gets called on every frame
	 */
	@Override
	public void onDrawFrame(GL10 gl) {

		//draw the current test here
		if(mCurrTest != -1){ //if not all tests are finished
			doTest(mCurrTest, gl);
		}


	}

	/*
	 * Do one frame of test @test
	 * Call this from within onDrawFrame()
	 */
	private void doTest(int test, GL10 gl) {

		switch(test){

		case 1: //fillrate test (without texture)
			gl.glEnable(GL10.GL_BLEND);
			gl.glDisable(GL10.GL_TEXTURE_2D);
			gl.glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored

			drawTextFillTest(1, gl); //the 1 is for NO texture
			mFrameCounter += 1;
			if(((float) mFrameCounter * 1000 / (float) mTimeSlot) > 30.0f){ //if FPS > 24
				mNumPlanes += 4; //increase the number of planes
			}
			// Update rotation parameters.
			if(mYRotation > 2.99f) {
				mSwitchDir = true;
			}else if(mYRotation < -2.99f) {
				mSwitchDir = false;
			}
			if(mSwitchDir)
				mYRotation -= 0.6f/1000.0f * (currTime() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
			else
				mYRotation += 0.6f/1000.0f * (currTime() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
			if((mTimeSlot = currTime() - mStartTime) > 20000) {
				mData.addResult("-----------------------------\n");
				mData.addResult("Test #1: Fillrate (No texture)\n");
				showStats();
				mData.addResult("#planes (fill rate tests): " + mNumPlanes);
				mData.addResult("\n#planes x FPS: " + mNumPlanes*(mFrameCounter * 1000.0f / mTimeSlot) + "\n");
				mCurrTest = 2; //do next test
				mStartTime = currTime(); //reset the timer for the next test
				mTimeSlot = 0; //reset timer
				mFrameCounter = 0; //reset the FPS counter for the next test
				mNumPlanes = 2; //reset the number of planes for the next test
				mYRotation = 0.0f; //reset starting position for next test
				mCurrTest = 2;
			}
			break;
		case 2: //fillrate test with texture
			gl.glEnable(GL10.GL_TEXTURE_2D);
			gl.glEnable(GL10.GL_BLEND);
			gl.glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored

			drawTextFillTest(0, gl); //the 0 is for texture
			mFrameCounter += 1;
			if(((float) mFrameCounter * 1000 / (float) mTimeSlot) > 30.0f){ //if FPS > 24
				mNumPlanes += 4; //increase the number of planes
			}
			// Update rotation parameters.
			if(mYRotation > 2.99f) {
				mSwitchDir = true;
			}else if(mYRotation < -2.99f) {
				mSwitchDir = false;
			}
			if(mSwitchDir)
				mYRotation -= 0.6f/1000.0f * (currTime() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
			else
				mYRotation += 0.6f/1000.0f * (currTime() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
			if((mTimeSlot = currTime() - mStartTime) > 20000) {
				mData.addResult("-----------------------------\n");
				mData.addResult("Test #1: Fillrate (No texture)\n");
				showStats();
				mData.addResult("#planes (fill rate tests): " + mNumPlanes);
				mData.addResult("\n#planes x FPS: " + mNumPlanes*(mFrameCounter * 1000.0f / mTimeSlot) + "\n");
				mCurrTest = 2; //do next test
				mStartTime = currTime(); //reset the timer for the next test
				mTimeSlot = 0; //reset timer
				mFrameCounter = 0; //reset the FPS counter for the next test
				mNumPlanes = 2; //reset the number of planes for the next test
				mYRotation = 0.0f; //reset starting position for next test
				mCurrTest = 3;
			}
			break;
		case 3:
			//Dynamic Object test
			gl.glDisable(GL10.GL_TEXTURE_2D);
			gl.glDisable(GL10.GL_BLEND);	
			this.drawDOTest(gl);	
			mFrameCounter += 1;
			if(((float) mFrameCounter * 1000 / (float) mTimeSlot) > 30.0f){ //if FPS > 30
				mNumPolygons += 100; //increase the number of vertices
			}
			// Update rotation parameters.
			mXRotation += .3f;
			mYRotation += .24f;
			mZRotation += .18f;
			if((mTimeSlot = currTime() - mStartTime) > 20000) {
				mData.addResult("-----------------------------\n");
				mData.addResult("Test #3: Dynamic Object test\n");
				showStats();
				mData.addResult("#Vertices (dynamic object test): " + mNumPolygons);
				mData.addResult("\ndrawing time (msecs): " + mGLTimeSlot + "\nobject coordinates calc time (msecs): " + mMoTimeSlot);
				mData.addResult("\ndrawing time/object coord calc time ratio: " + (float) mGLTimeSlot/ (float) mMoTimeSlot + "\n");
				mCurrTest = 4; //do next test
				mStartTime = currTime(); //reset the timer for the next test
				mFrameCounter = 0; //reset the FPS counter for the next test
			}

			break;
		case 4: //textured box test
			gl.glEnable(GL10.GL_TEXTURE_2D);
			gl.glDisable(GL10.GL_BLEND);
			gl.glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
			draw(gl); 
			mFrameCounter += 1;
			// Update rotation parameters.
			mXRotation += 1.0f;
			mYRotation += 0.8f;
			mZRotation += 0.6f;
			if((mTimeSlot = currTime() - mStartTime) > 20000) {  
				mData.addResult("-----------------------------\n");
				mData.addResult("Test #4: Rotating textured box\n");
				showStats();

				//when all tests have finished we want to show the results screen
				Intent myIntent = new Intent(mContext, ResultsActivity.class);
				mActivity.startActivityForResult(myIntent, 0);
				mCurrTest = -1;
			}
			break;
		default: 
			break;
		}

	}

	/*
	 * Draw lots of planes with different opacity (and with or without texture depending on the type param)
	 * on top of each other to test the fill rate of the GPU.
	 * */
	void drawTextFillTest(int type, GL10 gl) {

		// Array used for object coordinates.
		//vertex and color arrays (needs to be direct, meaning that they are placed on the native heap where the GC can't touch them)
		ByteBuffer vcoords = ByteBuffer.allocateDirect(mNumPlanes*3*4*(Float.SIZE/8));
		ByteBuffer tcoords = ByteBuffer.allocateDirect(mNumPlanes*2*4*(Float.SIZE/8));
		ByteBuffer colors = ByteBuffer.allocateDirect(mNumPlanes*4*4*(Float.SIZE/8));
		//Multi-byte data types need to have native byte order (floats, int, short etc.)
		vcoords.order(ByteOrder.nativeOrder());
		tcoords.order(ByteOrder.nativeOrder());
		colors.order(ByteOrder.nativeOrder());
		//convert to float buffers 
		mVCoords = vcoords.asFloatBuffer();
		mTCoords = tcoords.asFloatBuffer();
		mColors = colors.asFloatBuffer();
		mColors.position(0);

		int j = 0; //used to simulate second array index

		if(type == 0){ //texture test
			for(int i = 0; i < mNumPlanes*4; ++i) { //fill the color array with mColors
				mColors.put(j, 1.0f); // Go from maximum red to none
				mColors.put(j+1, 1.0f);
				mColors.put(j+2, 1.0f); // Go from no blue to maximum
				mColors.put(j+3,1.0f - i*(0.25f/mNumPlanes)); // Alpha value (from full opacity to almost none)
				j += 4;
			}
		}else{ //no texture
			for(int i = 0; i < mNumPlanes*4; ++i) { //fill the color array with mColors
				mColors.put(j, 1.0f - i*(0.5f/mNumPlanes)); // Go from maximum red to none
				mColors.put(j+1, 0.0f);
				mColors.put(j+2, 0.0f + i*(0.5f/mNumPlanes)); // Go from no blue to maximum
				mColors.put(j+3, 1.0f - i*(0.25f/mNumPlanes)); // Alpha value (from full opacity to almost none)
				j += 4;
			}
		}

		// Array used to convert from QUAD to TRIANGLE_STRIP.
		// QUAD is not available on the OpenGL implementation
		// we are using.
		//GLubyte indices[4] = {0, 1, 3, 2};

		// Set the background color to be used when clearing the screen.
		gl.glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		// Clear the screen and the depth buffer.
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);

		// Use the model matrix.
		gl.glMatrixMode(GL10.GL_MODELVIEW);

		// Reset the model matrix.
		gl.glLoadIdentity();

		// Move into the screen 5 units.
		gl.glTranslatef(0.0f, 0.0f, -10.0f);

		// Select the texture to use when rendering the box.
		gl.glBindTexture(GL10.GL_TEXTURE_2D, mTextures[0]);

		// Set pointers to vertex coordinates.
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, mVCoords);
		gl.glColorPointer(4, GL10.GL_FLOAT, 0, mColors); //COLORS MUST BE DEFINED WITH RGBA IN THIS GL-IMPLEMENTATION HENCE THE 4.
		gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, mTCoords);

		// Enable vertex coord arrays. 
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
		gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);


		// Set the current color
		//glDisable(GL_DEPTH_TEST);

		// Define the front face of the box
		//GLfloat tdiff = 0.001;

		// Specify rotation
		gl.glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
		j = 0;
		int k = 0;
		for(int i = 0; i < mNumPlanes; ++i) { // fill the vertex coord array
			//x									y								z
			mVCoords.put(j, -0.5f); mVCoords.put(j+1, -1.0f); mVCoords.put(j+2, -1.0f + i*(20.0f/mNumPlanes)); //go from inside the screen and outwards
			mVCoords.put(j+3, 0.5f); mVCoords.put(j+4, -1.0f); mVCoords.put(j+5, -1.0f + i*(20.0f/mNumPlanes));
			mVCoords.put(j+9, 0.5f); mVCoords.put(j+10, 1.0f); mVCoords.put(j+11, -1.0f + i*(20.0f/mNumPlanes));
			mVCoords.put(j+6, -0.5f); mVCoords.put(j+7, 1.0f); mVCoords.put(j+8, -1.0f + i*(20.0f/mNumPlanes));
			mTCoords.put(k, -0.5f); mTCoords.put(k+1, -1.0f);
			mTCoords.put(k+2, 0.5f); mTCoords.put(k+3, -1.0f);
			mTCoords.put(k+4, 0.5f); mTCoords.put(k+5, 1.0f);
			mTCoords.put(k+6, -0.5f); mTCoords.put(k+7, 1.0f);
			gl.glRotatef(mYRotation*0.1f, 0.0f, 1.0f, 0.0f); //turn each plane sideways

			j+=12;
			k+=8;

			// Do the drawing
			gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 4*i, 4); //draws the arrays enabled with glEnableClientState()
		}


		// Disable texture and vertex arrays.
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glDisableClientState(GL10.GL_COLOR_ARRAY);
		gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

		// Wait (blocks) until all GL drawing commands to finish.
		gl.glFinish();

		//clear buffers to reset their positions (so glDrawArrays do not read outside of them)
		// vcoords.clear();
		mColors.clear();


	}

	private void drawDOTest(GL10 gl) {

		//vertex and color arrays (needs to be direct, meaning that they are placed on the native heap where the GC can't touch them)
		ByteBuffer vcoords = ByteBuffer.allocateDirect(mNumPolygons*3*(Float.SIZE/8));
		ByteBuffer colors = ByteBuffer.allocateDirect(mNumPolygons*4*(Float.SIZE/8));
		//Multi-byte data types need to have native byte order (floats, int, short etc.)
		vcoords.order(ByteOrder.nativeOrder());
		colors.order(ByteOrder.nativeOrder());
		//convert to float buffers 
		mVCoords = vcoords.asFloatBuffer();
		mColors = colors.asFloatBuffer();
		mColors.position(0);

		for(int i = 0; i < mNumPolygons; ++i){

			//mColors.put(4*i+0, 1.0f); //R
			//mColors.put(4*i+1, 0.0f); //G
			//mColors.put(4*i+2, 0.0f); //B
			//mColors.put(4*i+3, 1.0f); //A

			if(i%3 != 0){
				mColors.put(4*i+0, 1.0f); //R
				mColors.put(4*i+1, 0.0f); //G
				mColors.put(4*i+2, 0.0f); //B
				mColors.put(4*i+3, 1.0f); //A
			}else if(i%2 != 0){
				mColors.put(4*i+0, 0.0f); //R
				mColors.put(4*i+1, 1.0f); //G
				mColors.put(4*i+2, 0.0f); //B
				mColors.put(4*i+3, 1.0f); //A
			}else{
				mColors.put(4*i+0, 0.0f); //R
				mColors.put(4*i+1, 0.0f); //G
				mColors.put(4*i+2, 1.0f); //B
				mColors.put(4*i+3, 1.0f); //A
			}

		}

		// Set the background color to be used when clearing the screen.
		gl.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// Clear screen
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);

		// Use the model matrix.
		gl.glMatrixMode(GL10.GL_MODELVIEW);

		// Reset the model matrix.
		gl.glLoadIdentity();

		// Move into the screen 5 units.
		gl.glTranslatef(0.0f, 0.0f, -3.0f);

		// Specify rotation along the X, Y, and Z axes.
		gl.glRotatef(mXRotation, 1.0f, 0.0f, 0.0f);
		gl.glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
		gl.glRotatef(mZRotation, 0.0f, 0.0f, 1.0f);

		// Set pointers to vertex coordinates and colors.
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, mVCoords);
		gl.glColorPointer(4, GL10.GL_FLOAT, 0, mColors);

		// Enable vertex coord array and color array.
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL10.GL_COLOR_ARRAY);

		// Set the current color
		//glDisable(GL_DEPTH_TEST);

		// Define the front face of the box
		float tdiff = 0.2f;
		float phi, thet;

		//set the color
		//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

		mT+=tdiff;

		if(mT > 1.0f){
			mT = 0.0f;
		}

		//start timing
		long tmpTime = currTime();

		for(int i = 0; i < mNumPolygons; ++i) {
			phi = i*2*3.14159f/mNumPolygons;
			thet = phi/2.0f;
			mVCoords.put((float) Math.sin(thet)* (float) Math.cos(phi)); mVCoords.put((float)Math.sin(thet)*(float)Math.sin(phi)); mVCoords.put((float)Math.cos(thet));
		}

		mMoTimeSlot += currTime() - tmpTime;
		tmpTime = currTime();

		// Draw the plane.
		gl.glDrawArrays(GL10.GL_TRIANGLE_FAN, 0, mNumPolygons);

		// Disable texture and vertex arrays.
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glDisableClientState(GL10.GL_COLOR_ARRAY);

		// Wait (blocks) until all GL drawing commands to finish.
		gl.glFinish();
		mGLTimeSlot += currTime() - tmpTime;

		//clear buffers to reset their positions (so glDrawArrays do not read outside of them)
		// vcoords.clear();
		mColors.clear();
	}

	/**
	 * Render the 3D model.
	 */
	void draw(GL10 gl)
	{

		// Array used for object coordinates.
		ByteBuffer vcoords = ByteBuffer.allocateDirect(4*3*4*(Float.SIZE/8));
		ByteBuffer tcoords = ByteBuffer.allocateDirect(4*2*4*(Float.SIZE/8));
		ByteBuffer indices = ByteBuffer.allocateDirect(4*(Float.SIZE/8));
		//Multi-byte data types need to have native byte order (floats, int, short etc.)
		vcoords.order(ByteOrder.nativeOrder());
		tcoords.order(ByteOrder.nativeOrder());
		indices.order(ByteOrder.nativeOrder());
		//convert to float buffers 
		mVCoords = vcoords.asFloatBuffer();
		mTCoords = tcoords.asFloatBuffer();
		indices.position(0);


		// Array used to convert from QUAD to TRIANGLE_STRIP.
		// QUAD is not available on the OpenGL implementation
		// we are using.
		indices.put(0, (byte) 0); indices.put(1, (byte) 1); indices.put(2, (byte) 3); indices.put(3, (byte) 2);

		// Set the background color to be used when clearing the screen.
		gl.glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		// Clear the screen and the depth buffer.
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);

		// Use the model matrix.
		gl.glMatrixMode(GL10.GL_MODELVIEW);

		// Reset the model matrix.

		gl.glPushMatrix();
		gl.glLoadIdentity();

		//GLfloat position[4] = { -2.0f, -2.0f, 2.0f, 1.0f };
		//glLightfv(GL10.GL_LIGHT0, GL10.GL_POSITION, position);

		// Specify rotation along the X, Y, and Z axes.
		gl.glRotatef(0, 1.0f, 0.0f, 0.0f);
		gl.glRotatef(0, 0.0f, 1.0f, 0.0f);
		gl.glRotatef(0, 0.0f, 0.0f, 1.0f);
		gl.glPopMatrix();

		gl.glPushMatrix();
		gl.glLoadIdentity();
		// Move into the screen 5 units.
		gl.glTranslatef(0.0f, 0.0f, -5.0f);

		// Specify rotation along the X, Y, and Z axes.
		gl.glRotatef(mXRotation, 1.0f, 0.0f, 0.0f);
		gl.glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
		gl.glRotatef(mZRotation, 0.0f, 0.0f, 1.0f);

		// Select the texture to use when rendering the box.
		gl.glBindTexture(GL10.GL_TEXTURE_2D, mTextures[0]);

		// Set pointers to vertex coordinates and texture coordinates.
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, mVCoords);
		gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, mTCoords);

		// Enable vertex and texture coord arrays.
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

		// Define the front face of the box.
		mTCoords.put(0, 1.0f);  mTCoords.put(1, 0.0f);
		mVCoords.put(0, -1.0f); mVCoords.put(1, -1.0f); mVCoords.put(2, 1.0f);
		mTCoords.put(2, 0.0f);  mTCoords.put(3, 0.0f);
		mVCoords.put(3, 1.0f);  mVCoords.put(4, -1.0f); mVCoords.put(5, 1.0f);
		mTCoords.put(4, 0.0f);  mTCoords.put(5, 1.0f);
		mVCoords.put(6, 1.0f);  mVCoords.put(7, 1.0f);  mVCoords.put(8, 1.0f);
		mTCoords.put(6, 1.0f);  mTCoords.put(7, 1.0f);
		mVCoords.put(9, -1.0f); mVCoords.put(10, 1.0f);  mVCoords.put(11, 1.0f);

		// This draws one textured plane using a strip of two triangles.
		gl.glDrawElements(GL10.GL_TRIANGLE_STRIP, 4, GL10.GL_UNSIGNED_BYTE, indices);

		// Define the back face of the box.
		mTCoords.put(0, 0.0f);  mTCoords.put(1, 0.0f);
		mVCoords.put(0, -1.0f); mVCoords.put(1, -1.0f); mVCoords.put(2, -1.0f);
		mTCoords.put(2, 0.0f);  mTCoords.put(3, 1.0f);
		mVCoords.put(3, -1.0f); mVCoords.put(4, 1.0f);  mVCoords.put(5, -1.0f);
		mTCoords.put(4, 1.0f);  mTCoords.put(5, 1.0f);
		mVCoords.put(6, 1.0f);  mVCoords.put(7, 1.0f);  mVCoords.put(8, -1.0f);
		mTCoords.put(6, 1.0f);  mTCoords.put(7, 0.0f);
		mVCoords.put(9, 1.0f); mVCoords.put(10, -1.0f); mVCoords.put(11, -1.0f);

		// Draw the plane.
		gl.glDrawElements(GL10.GL_TRIANGLE_STRIP, 4, GL10.GL_UNSIGNED_BYTE, indices);

		// Define the top face of the box.
		mTCoords.put(0, 1.0f);  mTCoords.put(1, 1.0f);
		mVCoords.put(0, -1.0f); mVCoords.put(1, 1.0f); mVCoords.put(2, -1.0f);
		mTCoords.put(2, 1.0f);  mTCoords.put(3, 0.0f);
		mVCoords.put(3, -1.0f); mVCoords.put(4, 1.0f);  mVCoords.put(5, 1.0f);
		mTCoords.put(4, 0.0f);  mTCoords.put(5, 0.0f);
		mVCoords.put(6, 1.0f);  mVCoords.put(7, 1.0f);  mVCoords.put(8, 1.0f);
		mTCoords.put(6, 0.0f);  mTCoords.put(7, 1.0f);
		mVCoords.put(9, 1.0f); mVCoords.put(10, 1.0f); mVCoords.put(11, -1.0f);

		// Draw the plane.
		gl.glDrawElements(GL10.GL_TRIANGLE_STRIP, 4, GL10.GL_UNSIGNED_BYTE, indices);

		// Define the bottom face of the box.
		mTCoords.put(0, 1.0f);  mTCoords.put(1, 1.0f);
		mVCoords.put(0, -1.0f); mVCoords.put(1, -1.0f); mVCoords.put(2, -1.0f);
		mTCoords.put(2, 1.0f);  mTCoords.put(3, 0.0f);
		mVCoords.put(3, -1.0f); mVCoords.put(4, -1.0f);  mVCoords.put(5, 1.0f);
		mTCoords.put(4, 0.0f);  mTCoords.put(5, 0.0f);
		mVCoords.put(6, 1.0f);  mVCoords.put(7, -1.0f);  mVCoords.put(8, 1.0f);
		mTCoords.put(6, 0.0f);  mTCoords.put(7, 1.0f);
		mVCoords.put(9, 1.0f); mVCoords.put(10, -1.0f); mVCoords.put(11, -1.0f);

		// Draw the plane.
		gl.glDrawElements(GL10.GL_TRIANGLE_STRIP, 4, GL10.GL_UNSIGNED_BYTE, indices);

		// Define the right face of the box.
		mTCoords.put(0, 0.0f);  mTCoords.put(1, 0.0f);
		mVCoords.put(0, 1.0f); mVCoords.put(1, -1.0f); mVCoords.put(2, -1.0f);
		mTCoords.put(2, 0.0f);  mTCoords.put(3, 1.0f);
		mVCoords.put(3, 1.0f); mVCoords.put(4, 1.0f);  mVCoords.put(5, -1.0f);
		mTCoords.put(4, 1.0f);  mTCoords.put(5, 1.0f);
		mVCoords.put(6, 1.0f);  mVCoords.put(7, 1.0f);  mVCoords.put(8, 1.0f);
		mTCoords.put(6, 1.0f);  mTCoords.put(7, 0.0f);
		mVCoords.put(9, 1.0f); mVCoords.put(10, -1.0f); mVCoords.put(11, 1.0f);

		// Draw the plane.
		gl.glDrawElements(GL10.GL_TRIANGLE_STRIP, 4, GL10.GL_UNSIGNED_BYTE, indices);

		// Define the left face of the box.
		mTCoords.put(0, 1.0f);  mTCoords.put(1, 0.0f);
		mVCoords.put(0, -1.0f); mVCoords.put(1, -1.0f); mVCoords.put(2, -1.0f);
		mTCoords.put(2, 0.0f);  mTCoords.put(3, 0.0f);
		mVCoords.put(3, -1.0f); mVCoords.put(4, -1.0f);  mVCoords.put(5, 1.0f);
		mTCoords.put(4, 0.0f);  mTCoords.put(5, 1.0f);
		mVCoords.put(6, -1.0f);  mVCoords.put(7, 1.0f);  mVCoords.put(8, 1.0f);
		mTCoords.put(6, 1.0f);  mTCoords.put(7, 1.0f);
		mVCoords.put(9, -1.0f); mVCoords.put(10, 1.0f); mVCoords.put(11, -1.0f);

		// Draw the plane.
		gl.glDrawElements(GL10.GL_TRIANGLE_STRIP, 4, GL10.GL_UNSIGNED_BYTE, indices);

		// Disable texture and vertex arrays.
		gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);

		// Wait (blocks) until all GL drawing commands to finish.
		gl.glFinish();
		gl.glPopMatrix();

		indices.clear();

	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {

		this.initGL(gl); //init GL

	}

	/*
	 * Do OpenGL initialization
	 */
	private void initGL(GL10 gl) { 
		//create and bind texture
		this.createTexture(gl);

		// Enable texture mapping.
		gl.glEnable(GL10.GL_TEXTURE_2D);

		// Enable smooth shading.
		gl.glShadeModel(GL10.GL_SMOOTH);

		//enable blending (to make materials translucent)
		gl.glEnable (GL10.GL_BLEND);
		gl.glBlendFunc (GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);

		// Set the depth value used when clearing the depth buffer.
		gl.glClearDepthf(1.0f);

		// Enable depth testing.
		gl.glEnable(GL10.GL_DEPTH_TEST);

		// Set the type of depth test.
		gl.glDepthFunc(GL10.GL_LEQUAL);

		// Use the best perspective correction method.
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_NICEST);
	}

	/* 
	 * Returns the elapsed time (in msecs) since boot 
	 */
	private long currTime() {
		return SystemClock.elapsedRealtime();
	}

	/**************************************************************************
	 *  Instance variables 													  *
	 **************************************************************************/
	GLBenchActivity mActivity;
	ResultsActivity mResultsActivity;
	Context mContext;
	Data mData;

	float mT;
	boolean mSwitchDir = false;
	int mNumPolygons; //should really be named mNumVertices because that is what it is
	int mNumPlanes;
	FloatBuffer mVCoords;
	FloatBuffer mTCoords;
	FloatBuffer mColors;

	int[] mTextures; //texture pointers

	//Rotation params
	float mXRotation, mYRotation, mZRotation;

	//Timers & counters
	int mFrameCounter;
	long mStartTime;
	long mTimeSlot;
	long mMoTimeSlot;
	long mGLTimeSlot;

	//The current test
	int mCurrTest;

	//String to hold the test reults
	String mResultString = new String();

}
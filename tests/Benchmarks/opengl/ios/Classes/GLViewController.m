//
//  GLViewController.m
//  OpenGLES Benchmark for iOS
//
//  Created by Alexander Samuelsson July 2011.
//

#import "GLViewController.h"
#import "ConstantsAndMacros.h"
#import "OpenGLCommon.h"
@implementation GLViewController
- (void)drawView:(UIView *)theView
{    
    // Drawing code here
    
    // Do tests here!
    
    switch(mTest) {
            
        case 1: //fillrate test 
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
            
            [self drawTextFillTest:1]; //the 1 is for NO texture
            mFrameCounter += 1;
            if((mFrameCounter / mTimeSlot) > 30.0f){ //if FPS > 24
                mNumPlanes += 4; //increase the number of planes
            }
            // Update rotation parameters.
            if(mYRotation > 2.99f) {
                mSwitchDir = true;
            }else if(mYRotation < -2.99f) {
                mSwitchDir = false;
            }
            if(mSwitchDir)
                mYRotation -= 0.6f * ([self currTime] - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
            else
                mYRotation += 0.6f * ([self currTime] - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
            if((mTimeSlot = [self currTime] - mStartTime) > 20.0f) {
                mTextView.text = [mTextView.text stringByAppendingString:@"-----------------------------\n"];
                printf("-----------------------------\n");
                mTextView.text = [mTextView.text stringByAppendingString:@"Test #1: Fillrate (No texture)\n"];
                printf("Test #1: Fillrate (No texture)\n");
                [self showStats];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"#planes (fill rate tests): %d\n", mNumPlanes];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"#planes x FPS: %f\n", mNumPlanes*(mFrameCounter / mTimeSlot)];
                printf("#planes (fill rate tests): %d\n", mNumPlanes);
                printf("#planes x FPS: %f\n", mNumPlanes*(mFrameCounter / mTimeSlot));
                mTest = 2; //do next test
                mStartTime = [self currTime]; //reset the timer for the next test
                mTimeSlot = 0.0f; //reset timer
                mFrameCounter = 0; //reset the FPS counter for the next test
                mNumPlanes = 2; //reset the number of planes for the next test
                mYRotation = 0.0f; //reset starting position for next test
            }
            
            break;
            
        case 2: //fillrate test with texture
            glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
            
            [self drawTextFillTest:0]; //the 0 is for texture
            mFrameCounter += 1;
            if((mFrameCounter / mTimeSlot) > 30.0f){ //if FPS > 24
                mNumPlanes += 4; //increase the number of planes
            }
            // Update rotation parameters.
            if(mYRotation > 2.99f) {
                mSwitchDir = true;
            }else if(mYRotation < -2.99f) {
                mSwitchDir = false;
            }
            if(mSwitchDir)
                mYRotation -= 0.6f * ([self currTime] - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
            else
                mYRotation += 0.6f * ([self currTime] - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.6*20 units
            if((mTimeSlot = [self currTime] - mStartTime) > 20.0f) {
                mTextView.text = [mTextView.text stringByAppendingString:@"-----------------------------\n"];
                printf("-----------------------------\n");
                mTextView.text = [mTextView.text stringByAppendingString:@"Test #2: Fillrate (With texture)\n"];
                printf("Test #2: Fillrate (With texture)\n");
                [self showStats];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"#planes (fill rate tests): %d\n", mNumPlanes];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"#planes x FPS: %f\n", mNumPlanes*(mFrameCounter / mTimeSlot)];
                printf("#planes (fill rate tests): %d\n", mNumPlanes);
                printf("#planes x FPS: %f\n", mNumPlanes*(mFrameCounter / mTimeSlot));
                mTest = 3; //do next test
                mStartTime = [self currTime]; //reset the timer for the next test
                mFrameCounter = 0; //reset the FPS counter for the next test
                mYRotation = 0.0f; //reset starting position for next test
                mTimeSlot = 0.0f; //reset timer
            }
            
            break;
            
        case 3:   
            
            //Dynamic Object test
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
            
            [self drawDOTest]; 
            mFrameCounter += 1;
            if((mFrameCounter / mTimeSlot) > 30.0f){ //if FPS > 30
                mNumPolygons += 100; //increase the number of polygons
            }
            // Update rotation parameters.
            mXRotation += .3f;
            mYRotation += .24f;
            mZRotation += .18f;
            if((mTimeSlot = [self currTime] - mStartTime) > 20.0f) {
                mTextView.text = [mTextView.text stringByAppendingString:@"-----------------------------\n"];
                printf("-----------------------------\n");
                mTextView.text = [mTextView.text stringByAppendingString:@"Test #3: Dynamic Object test\n"];
                printf("Test #3: Dynamic Object test\n");
                [self showStats];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"#Vertices (dynamic object test): %d\n", mNumPolygons];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"drawing time (secs): %f\nobject coordinates calc time (secs): %f\n", mGLTimeSlot, mMoTimeSlot];
                mTextView.text = [mTextView.text stringByAppendingFormat:@"drawing time/object coord calc time ratio: %f\n", mGLTimeSlot/mMoTimeSlot];
                printf("#Vertices (dynamic object test): %d\n", mNumPolygons);
                printf("drawing time (secs): %f\nobject coordinates calc time (secs): %f\n", mGLTimeSlot, mMoTimeSlot);
                printf("drawing time/object coord calc time ratio: %f\n", mGLTimeSlot/mMoTimeSlot);
                mTest = 4; //do next test
                mStartTime = [self currTime]; //reset the timer for the next test
                mFrameCounter = 0; //reset the FPS counter for the next test
            }
            
            break;
            
        case 4:   
            
            //Rotating textured box test
            glEnable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
            
            [self draw]; 
            mFrameCounter += 1;
            // Update rotation parameters.
            mXRotation += 1.0f;
            mYRotation += 0.8f;
            mZRotation += 0.6f;
            if((mTimeSlot = [self currTime] - mStartTime) > 20.0f) {
                mTextView.text = [mTextView.text stringByAppendingString:@"-----------------------------\n"];
                printf("-----------------------------\n");
                mTextView.text = [mTextView.text stringByAppendingString:@"Test #4: Rotating textured box\n"];
                printf("Test #4: Rotating textured box\n");
                [self showStats];
                mTest = -1; //last test
                //[theView removeFromSuperview]; //remove the viev
                
                //set the label's text
                mTextView.text = [mTextView.text stringByAppendingString:@"Tests completed!"];
                
                //add the label to the view
                [self.view addSubview:mTextView]; //this message will make self.setupView to be called so we'll have to tell that we are done
                mDone = true; //we are done with the tests
                
                //release the label
                [mTextView release];
                
                
            }
            
            break;
            
            
            
            
    }
    
    
}
-(void)setupView:(GLView*)view
{
    
    printf("----------------setupView-----------------\n");
    //Do not show the status bar
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    
    //create a frame that sets the bounds of the view
    CGRect frame = CGRectMake(0, 0, 320, 480);
    
    //allocate the label
    mTextView = [[UITextView alloc] initWithFrame:frame];
    [mTextView setEditable:FALSE]; //the text should not be editable
    
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); 
    CGRect rect = view.bounds; 
    GLfloat ratio = rect.size.width/rect.size.height;
    [self gluPerspective:45.0f setAspect:ratio setZNear:0.1f setZFar:100.0f]; //call gluPerspective
    glViewport(0, 0, rect.size.width, rect.size.height);  
    
    //create texture
    [self createTexture];
    
    //initialize OpenGL
    [self initGL];
    mGLViewInitialized = true;
    
    if(!mDone)
        mTest = 1; //the starting test
    else
        mTest = -1; //we are done, no more tests
    mNumPlanes = 2;
    mNumPolygons = 100;
    mSwitchDir = false;
    
    //set the start time
    mStartTime = [self currTime];
    
    
}

/*
 * Standard OpenGL util function for setting up the perspective
 * projection matrix.
 **/
- (void)gluPerspective:(GLfloat)fovy setAspect:(GLfloat)aspect setZNear:(GLfloat)zNear setZFar:(GLfloat)zFar 
{
    const GLfloat pi = 3.14159;
    
    GLfloat ymax = zNear * tan(fovy * pi / 360.0);
    GLfloat ymin = -ymax;
    GLfloat xmin = ymin * aspect;
    GLfloat xmax = ymax * aspect;
    
    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

/*
 * Initiate OpenGL
 **/
- (void)initGL
{
    // Enable texture mapping.
    glEnable(GL_TEXTURE_2D);
    
    // Enable smooth shading.
    glShadeModel(GL_SMOOTH);
    
    //enable blending (to make materials translucent)
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set the depth value used when clearing the depth buffer.
    glClearDepthf(1.0f);
    
    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);
    
    // Set the type of depth test.
    glDepthFunc(GL_LEQUAL);
    
    // Use the best perspective correction method.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

- (void)drawDOTest
{
    // The GL_View must be initialized before we can do any drawing.
    if (!mGLViewInitialized)
    {
        return;
    }
    
    // Allocate space for colors and vertice arrays
    GLfloat *colors = (GLfloat*) malloc(4*mNumPolygons*4*sizeof(GLfloat));
    GLfloat *vcoords = (GLfloat*) malloc(4*mNumPolygons*3*sizeof(GLfloat));
    
    
    for(int i = 0; i < mNumPolygons; ++i){
        if(i%3){
            colors[4*i+0] = 1.0f;
            colors[4*i+1] = 0.0f;
            colors[4*i+2] = 0.0f;
            colors[4*i+3] = 1.0f;
        }else if(i%2){
            printf("OWL\n");
            colors[4*i+0] = 0.0f;
            colors[4*i+1] = 1.0f;
            colors[4*i+2] = 0.0f;
            colors[4*i+3] = 1.0f;
        }else{
            colors[4*i+0] = 0.0f;
            colors[4*i+1] = 0.0f;
            colors[4*i+2] = 1.0f;
            colors[4*i+3] = 1.0f;
        }
        
    }
    
    // Set the background color to be used when clearing the screen.
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // Clear the screen and the depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use the model matrix.
    glMatrixMode(GL_MODELVIEW);
    
    // Reset the model matrix.
    glLoadIdentity();
    
    // Move into the screen 5 units.
    glTranslatef(0.0f, 0.0f, -3.0f);
    
    // Specify rotation along the X, Y, and Z axes.
    glRotatef(mXRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
    glRotatef(mZRotation, 0.0f, 0.0f, 1.0f);
    
    // Set pointers to vertex coordinates and colors.
    glVertexPointer(3, GL_FLOAT, 0, vcoords);
    glColorPointer(4, GL_FLOAT, 0, colors);
    
    // Enable vertex coord array and color array.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    // Set the current color
    //glDisable(GL_DEPTH_TEST);
    
    // Define the front face of the box
    GLfloat tdiff = 0.2;
    GLfloat phi, thet;
    
    //set the color
    //glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    
    mT+=tdiff;
    
    if(mT > 1.0f){
        mT = 0.0f;
    }
    
    //start timing
    double tmpTime = [self currTime];
    
    for(int i = 0; i < mNumPolygons; ++i) {
        phi = i*2*3.14159/mNumPolygons;
        thet = phi/2.0f;
        vcoords[i*3+0] = sin(thet)*cos(phi); vcoords[i*3+1] = sin(thet)*sin(phi); vcoords[i*3+2] = cos(thet);
    }
    
    mMoTimeSlot += [self currTime] - tmpTime; 
    tmpTime = [self currTime];
    
    // Draw the plane.
    glDrawArrays(GL_TRIANGLE_FAN, 0, mNumPolygons);
    
    // Disable texture and vertex arrays.
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    // Wait (blocks) until all GL drawing commands to finish.
    glFinish();
    
    mGLTimeSlot += [self currTime] - tmpTime;
    
    // Update the GLView.
    //maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");
    // free the allocated memory
    free(colors);
    free(vcoords);
}


/*
 * Draw a rotating box with texture
 **/
- (void)draw
{
    // The GL_View must be initialized before we can do any drawing.
    if (!mGLViewInitialized)
    {
        return;
    }
    
    // Array used for object coordinates.
    GLfloat vcoords[4][3];
    
    // Array used for texture coordinates.
    GLfloat tcoords[4][2];
    
    // Array used to convert from QUAD to TRIANGLE_STRIP.
    // QUAD is not available on the OpenGL implementation
    // we are using.
    GLubyte indices[4] = {0, 1, 3, 2};
    
    // Set the background color to be used when clearing the screen.
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // Clear the screen and the depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use the model matrix.
    glMatrixMode(GL_MODELVIEW);
    
    // Reset the model matrix.
    
    glPushMatrix();
    glLoadIdentity();
    
    GLfloat position[4] = { -2.0f, -2.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    // Specify rotation along the X, Y, and Z axes.
    glRotatef(0, 1.0f, 0.0f, 0.0f);
    glRotatef(0, 0.0f, 1.0f, 0.0f);
    glRotatef(0, 0.0f, 0.0f, 1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glLoadIdentity();
    // Move into the screen 5 units.
    glTranslatef(0.0f, 0.0f, -5.0f);
    
    // Specify rotation along the X, Y, and Z axes.
    glRotatef(mXRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
    glRotatef(mZRotation, 0.0f, 0.0f, 1.0f);
    
    // Select the texture to use when rendering the box.
    //glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);
    
    // Set pointers to vertex coordinates and texture coordinates.
    glVertexPointer(3, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
    
    // Enable vertex and texture coord arrays.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    // Define the front face of the box.
    tcoords[0][0] = 1.0f;  tcoords[0][1] = 0.0f;
    vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = 1.0f;
    tcoords[1][0] = 0.0f;  tcoords[1][1] = 0.0f;
    vcoords[1][0] = 1.0f;  vcoords[1][1] = -1.0f; vcoords[1][2] = 1.0f;
    tcoords[2][0] = 0.0f;  tcoords[2][1] = 1.0f;
    vcoords[2][0] = 1.0f;  vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
    tcoords[3][0] = 1.0f;  tcoords[3][1] = 1.0f;
    vcoords[3][0] = -1.0f; vcoords[3][1] = 1.0f;  vcoords[3][2] = 1.0f;
    
    // This draws one textured plane using a strip of two triangles.
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
    
    // Define the back face of the box.
    tcoords[0][0] = 0.0f;  tcoords[0][1] = 0.0f;
    vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
    tcoords[1][0] = 0.0f;  tcoords[1][1] = 1.0f;
    vcoords[1][0] = -1.0f; vcoords[1][1] = 1.0f;  vcoords[1][2] = -1.0f;
    tcoords[2][0] = 1.0f;  tcoords[2][1] = 1.0f;
    vcoords[2][0] = 1.0f;  vcoords[2][1] = 1.0f;  vcoords[2][2] = -1.0f;
    tcoords[3][0] = 1.0f;  tcoords[3][1] = 0.0f;
    vcoords[3][0] = 1.0f;  vcoords[3][1] = -1.0f; vcoords[3][2] = -1.0f;
    
    // Draw the plane.
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
    
    // Define the top face of the box.
    tcoords[0][0] = 1.0f;  tcoords[0][1] = 1.0f;
    vcoords[0][0] = -1.0f; vcoords[0][1] = 1.0f; vcoords[0][2] = -1.0f;
    tcoords[1][0] = 1.0f;  tcoords[1][1] = 0.0f;
    vcoords[1][0] = -1.0f; vcoords[1][1] = 1.0f; vcoords[1][2] = 1.0f;
    tcoords[2][0] = 0.0f;  tcoords[2][1] = 0.0f;
    vcoords[2][0] = 1.0f;  vcoords[2][1] = 1.0f; vcoords[2][2] = 1.0f;
    tcoords[3][0] = 0.0f;  tcoords[3][1] = 1.0f;
    vcoords[3][0] = 1.0f;  vcoords[3][1] = 1.0f; vcoords[3][2] = -1.0f;
    
    // Draw the plane.
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
    
    // Define the bottom face of the box.
    tcoords[0][0] = 0.0f;  tcoords[0][1] = 1.0f;
    vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
    tcoords[1][0] = 1.0f;  tcoords[1][1] = 1.0f;
    vcoords[1][0] = 1.0f;  vcoords[1][1] = -1.0f; vcoords[1][2] = -1.0f;
    tcoords[2][0] = 1.0f;  tcoords[2][1] = 0.0f;
    vcoords[2][0] = 1.0f;  vcoords[2][1] = -1.0f; vcoords[2][2] = 1.0f;
    tcoords[3][0] = 0.0f;  tcoords[3][1] = 0.0f;
    vcoords[3][0] = -1.0f; vcoords[3][1] = -1.0f; vcoords[3][2] = 1.0f;
    
    // Draw the plane.
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
    
    // Define the right face of the box.
    tcoords[0][0] = 0.0f; tcoords[0][1] = 0.0f;
    vcoords[0][0] = 1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
    tcoords[1][0] = 0.0f; tcoords[1][1] = 1.0f;
    vcoords[1][0] = 1.0f; vcoords[1][1] = 1.0f;  vcoords[1][2] = -1.0f;
    tcoords[2][0] = 1.0f; tcoords[2][1] = 1.0f;
    vcoords[2][0] = 1.0f; vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
    tcoords[3][0] = 1.0f; tcoords[3][1] = 0.0f;
    vcoords[3][0] = 1.0f; vcoords[3][1] = -1.0f; vcoords[3][2] = 1.0f;
    
    // Draw the plane.
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
    
    // Define the left face of the box.
    tcoords[0][0] = 1.0f;  tcoords[0][1] = 0.0f;
    vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
    tcoords[1][0] = 0.0f;  tcoords[1][1] = 0.0f;
    vcoords[1][0] = -1.0f; vcoords[1][1] = -1.0f; vcoords[1][2] = 1.0f;
    tcoords[2][0] = 0.0f;  tcoords[2][1] = 1.0f;
    vcoords[2][0] = -1.0f; vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
    tcoords[3][0] = 1.0f;  tcoords[3][1] = 1.0f;
    vcoords[3][0] = -1.0f; vcoords[3][1] = 1.0f;  vcoords[3][2] = -1.0f;
    
    // Draw the plane.
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
    
    // Disable texture and vertex arrays.
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    // Wait (blocks) until all GL drawing commands to finish.
    glFinish();
    glPopMatrix();
}

/*
 * Print statistics to the console
 */
- (void)showStats
{
    mTextView.text = [mTextView.text stringByAppendingFormat:@"frames: %lu\n", mFrameCounter];
    mTextView.text = [mTextView.text stringByAppendingFormat:@"time (secs): %f\n", mTimeSlot];
    mTextView.text = [mTextView.text stringByAppendingFormat:@"FPS: %f\n", mFrameCounter / mTimeSlot];
    printf("Resolution: %d x %d\n", mViewWidth, mViewHeight);
    printf("frames: %lu\n", mFrameCounter);
    printf("time (secs): %f\n", mTimeSlot);
    printf("FPS: %f\n", mFrameCounter / mTimeSlot);
}


- (double)currTime
{
    return CFAbsoluteTimeGetCurrent(); 
}

- (void)createTexture
{
    //Generate texture name and bind texture
    glGenTextures(1, &mTextures[0]);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
    
    //Load image from Resources
    NSString *path = [[NSBundle mainBundle] pathForResource:@"Box" ofType:@"png"];
    NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
    UIImage *image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        NSLog(@"Do real error checking here");
    
    GLuint width = CGImageGetWidth(image.CGImage);
    GLuint height = CGImageGetHeight(image.CGImage);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void *imageData = malloc( height * width * 4 );
    CGContextRef context = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( context, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( context, 0, height - height );
    CGContextDrawImage( context, CGRectMake( 0, 0, width, height ), image.CGImage );
    
    //Pass the image data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    CGContextRelease(context);
    
    free(imageData);
    [image release];
    [texData release];
    
    // Set texture parameters.
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/* 
 * Fill rate test, draws semi-transparent planes to the screen to test the fill rate
 * of the GPU. Dynamically allocates and create more planes each time the frame rate
 * drops below 30 FPS. If type == 0 the planes has texture.
 **/
-(void)drawTextFillTest:(int)type
{
    // The GL_View must be initialized before we can do any drawing.
    if (!mGLViewInitialized){
        return;
    }
    
    // Array used for object coordinates.
    //GLfloat fill_vcoords[40][3];
    GLfloat *fill_vcoords = (GLfloat*) malloc(4*mNumPlanes*3*sizeof(GLfloat));
    // Array used for coloring vertices
    GLfloat *colors = (GLfloat*) malloc(4*mNumPlanes*4*sizeof(GLfloat));
    // Array used for texture coordinates.
    GLfloat *tcoords = (GLfloat*) malloc(4*mNumPlanes*2*sizeof(GLfloat));
    
    int j = 0; //used to simulate second array index
    
    if(type == 0){ //texture test
        for(int i = 0; i < 4*mNumPlanes; ++i) { //fill the color array with colors
            colors[j] = 1.0f; // Go from maximum red to none
            colors[j+1] = 1.0f;
            colors[j+2] = 1.0f; // Go from no blue to maximum
            colors[j+3] = 1.0f - i*(0.25f/mNumPlanes); // Alpha value (from full opacity to almost none)
            j += 4;
        }
    }else{ //no texture
        for(int i = 0; i < 4*mNumPlanes; ++i) { //fill the color array with colors
            colors[j] = 1.0f - i*(0.5f/mNumPlanes); // Go from maximum red to none
            colors[j+1] = 0.0f;
            colors[j+2] = 0.0f + i*(0.5f/mNumPlanes); // Go from no blue to maximum
            colors[j+3] = 1.0f - i*(0.25f/mNumPlanes); // Alpha value (from full opacity to almost none)
            j += 4;
        }
    }
    
    // Array used to convert from QUAD to TRIANGLE_STRIP.
    // QUAD is not available on the OpenGL implementation
    // we are using.
    //GLubyte indices[4] = {0, 1, 3, 2};
    
    // Set the background color to be used when clearing the screen.
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // Clear the screen and the depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use the model matrix.
    glMatrixMode(GL_MODELVIEW);
    
    // Reset the model matrix.
    glLoadIdentity();
    
    // Move into the screen 5 units.
    glTranslatef(0.0f, 0.0f, -10.0f);
    
    // Select the texture to use when rendering the box.
    //glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);
    
    // Set pointers to vertex coordinates.
    glVertexPointer(3, GL_FLOAT, 0, fill_vcoords);
    glColorPointer(4, GL_FLOAT, 0, colors); //COLORS MUST BE DEFINED WITH RGBA IN THIS GL-IMPLEMENTATION HENCE THE 4.
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
    
    // Enable vertex coord arrays. TODO enable color_array!
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    
    // Set the current color
    //glDisable(GL_DEPTH_TEST);
    
    // Define the front face of the box
    //GLfloat tdiff = 0.001;
    
    // Specify rotation
    glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
    j = 0;
    int k = 0;
    for(int i = 0; i < mNumPlanes; ++i) { // fill the vertex coord array
        //x									y								z
        fill_vcoords[j] = -0.5f; fill_vcoords[j+1] = -1.0f; fill_vcoords[j+2] = -1.0f + i*(20.0f/mNumPlanes); //go from inside the screen and outwards
        fill_vcoords[j+3] = 0.5f; fill_vcoords[j+4] = -1.0f; fill_vcoords[j+5] = -1.0f + i*(20.0f/mNumPlanes);
        fill_vcoords[j+9] = 0.5f; fill_vcoords[j+10] = 1.0f; fill_vcoords[j+11] = -1.0f + i*(20.0f/mNumPlanes);
        fill_vcoords[j+6] = -0.5f; fill_vcoords[j+7] = 1.0f; fill_vcoords[j+8] = -1.0f + i*(20.0f/mNumPlanes);
        tcoords[k] = -0.5f; tcoords[k+1] = -1.0f;
        tcoords[k+2] = 0.5f; tcoords[k+3] = -1.0f;
        tcoords[k+4] = 0.5f; tcoords[k+5] = 1.0f;
        tcoords[k+6] = -0.5f; tcoords[k+7] = 1.0f;
        glRotatef(mYRotation*0.1f, 0.0f, 1.0f, 0.0f); //turn each plane sideways
        
        j+=12;
        k+=8;
        
        // Do the drawing
        glDrawArrays(GL_TRIANGLE_STRIP, 4*i, 4); //draws the arrays enabled with glEnableClientState()
    }
    
    
    // Disable texture and vertex arrays.
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    // Wait (blocks) until all GL drawing commands to finish.
    glFinish();
    
    // free all heap mem
    free(fill_vcoords);
    free(colors);
    free(tcoords);
}

- (void)dealloc 
{
    [super dealloc];
}
@end

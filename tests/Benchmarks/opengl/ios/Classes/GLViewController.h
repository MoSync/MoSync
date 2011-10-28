//
//  GLViewController.h
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//  Copyright ___ORGANIZATIONNAME___ ___YEAR___. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLView.h"
#import <time.h>

@interface GLViewController : UIViewController <GLViewDelegate>
{
    /** GLView state (true = initialized and ready to be drawn). */
	bool mGLViewInitialized;
    
	/** Handle to the texture. */
	GLuint mBoxTextureHandle;
    
	/** X axis rotation. */
	GLfloat mXRotation;
    
	/** Y axis rotation. */
	GLfloat mYRotation;
    
	/** Z axis rotation. */
	GLfloat mZRotation;
    
	/** msec counter */
	double mStartTime;
    
	/** frame draw counter */
	long int mFrameCounter;
    
	double mTimeSlot;
	double mMoTimeSlot;
	double mGLTimeSlot;
    
	/** current color */
	GLfloat mR, mG, mB;
	GLfloat mT;
    
	/** Screen handle and screen resolution */
	int mViewWidth;
	int mViewHeight;
	int mScreen;
	int mWaveIndex;
    bool mSwitchDir;
    
	int mNumPlanes; //the number of planes to draw in the fill tests (this will vary to make sure that the device will have to sweat!)
	int mNumPolygons; //the number of polygons in the dynamic object test (this will vary to make sure that the device will have to sweat!)
    int mTest; //the current test case
    
    GLuint mTextures[1];
    
    //allocate the label
    UITextView *mTextView;
    
    bool mDone;
}

- (void)gluPerspective:(GLfloat)fovy setAspect:(GLfloat)aspect setZNear:(GLfloat)zNear setZFar:(GLfloat)zFar;
- (void)initGL;
- (void)draw;
- (void)drawDOTest;
- (void)drawTextFillTest:(int)type;
- (void)showStats;
- (double)currTime;
- (void)createTexture;

@end

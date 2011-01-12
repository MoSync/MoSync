//  EAGLView.h
//  OpenGL ES Tutorial - Part 2 by javacom

// To enable Debug NSLog, add GCC_PREPROCESSOR_DEFINITIONS DEBUGON in Project Settings for Debug Build Only and replace NSLog() with DEBUGLOG()
#ifdef DEBUGON
#define DEBUGLOG if (DEBUGON) NSLog
#else
#define DEBUGLOG
#endif

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

/*
 This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
 The view content is basically an EAGL surface you render your OpenGL scene into.
 Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
 */
@interface MoSyncGLView : UIView {
	/* The pixel dimensions of the backbuffer */
    GLint backingWidth;
    GLint backingHeight;
    
    EAGLContext *context;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;
    
    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;
    
    NSTimer *animationTimer;
    NSTimeInterval animationInterval;
	
	EAGLContext *WorkingContext;
}

@property NSTimeInterval animationInterval;

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;
- (void)setupView;

- (void) bindContext;
- (void) renderContext;

@end
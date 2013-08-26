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

//  EAGLView.h
//  OpenGL ES Tutorial - Part 2 by javacom

// To enable Debug NSLog, add GCC_PREPROCESSOR_DEFINITIONS DEBUGON in Project Settings for Debug Build Only and replace NSLog() with DEBUGLOG()
#ifdef DEBUGON
#define DEBUGLOG if (DEBUGON) NSLog
#else
#define DEBUGLOG(...)
#endif

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "TouchHelper.h"

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

	TouchHelper* touchHelper;
}

@property NSTimeInterval animationInterval;

- (id)initWithFrame:(CGRect)frame andApi:(EAGLRenderingAPI)api;
- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;
- (void)setupView;
- (void) bindContext;
- (void) renderContext;

@end

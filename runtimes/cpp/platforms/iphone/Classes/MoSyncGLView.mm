
//  EAGLView.m
//  OpenGL ES Tutorial - Part 2 by javacom
//
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "MoSyncGLView.h"
#include "iphone_helpers.h"

// hackety hack..
void MoSync_AddTouchPressedEvent(int x, int y, int touchId);
void MoSync_AddTouchMovedEvent(int x, int y, int touchId);
void MoSync_AddTouchReleasedEvent(int x, int y, int touchId);

#define USE_DEPTH_BUFFER 1 

// A class extension to declare private methods
@interface MoSyncGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation MoSyncGLView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;

// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithFrame:(CGRect)frame andApi:(EAGLRenderingAPI)api
{
    self = [super initWithFrame:frame];
		if(self) {
		
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        /* Retina display support. This needs to be handled correctly for everything native ui...
        int w, h;
        getScreenResolution(w, h);
        if (w == 640 && h == 960) // Retina display detected
        {
            // Set contentScale Factor to 2
            self.contentScaleFactor = 2.0;
            eaglLayer.contentsScale=2; //new line   
        }  
        */
            
        animationInterval = 1.0 / 60.0;
			
		WorkingContext = nil;	
		
		touchHelper = [[TouchHelper alloc] init];
		self.multipleTouchEnabled = YES;
			
		[self setupView];
    }
    return self;
}

- (void)setupView {  // new method for intialisation of variables and states		
	/*
	// setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Setup Orthographic Projection for the 320 x 480 of the iPhone screen
	glOrthof(0.0f, 320.0f, 480.0f, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	*/
	
}

- (void)drawView {
	
//	[EAGLContext setCurrentContext:context];
//glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
//	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
 //   [context presentRenderbuffer:GL_RENDERBUFFER_OES];	
}

/*
	[EAGLContext setCurrentContext:context];
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);

	// draw...
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}
 */

- (void)layoutSubviews {
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
	[self drawView];
	
}


- (BOOL)createFramebuffer {
    
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (USE_DEPTH_BUFFER) {
        glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        DEBUGLOG(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}

- (void) bindContext {
	
	/*
    if(!WorkingContext) {
		
		EAGLSharegroup* group = context.sharegroup;
		if (!group)
		{
			NSLog(@"Could not get sharegroup from the main context");
			return;
		}
		
		WorkingContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1 sharegroup:group];
	}
	
    if (!WorkingContext || ![EAGLContext setCurrentContext:WorkingContext]) {
        NSLog(@"Could not create WorkingContext");
    }
	
	
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	*/
	
	[EAGLContext setCurrentContext:context];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	
}

- (void) renderContext {
    /*
	if (!WorkingContext || [EAGLContext setCurrentContext:WorkingContext] == NO)
    {
        NSLog(@"SwapBuffers: [EAGLContext setCurrentContext:WorkingContext] failed");
        return;
    }
	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	
    if([WorkingContext presentRenderbuffer:GL_RENDERBUFFER_OES] == NO)
    {
        NSLog(@"SwapBuffers: [WorkingContext presentRenderbuffer:GL_RENDERBUFFER_OES] failed");
    }  
	*/
	
	[EAGLContext setCurrentContext:context];	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];	
}


- (void)destroyFramebuffer {
    
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}


- (void)startAnimation {
    //self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
    //self.animationTimer = nil;
}

/*
- (void)setAnimationTimer:(NSTimer *)newTimer {
   [animationTimer invalidate];
   animationTimer = newTimer;
}

- (void)setAnimationInterval:(NSTimeInterval)interval {    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}
*/

- (void)dealloc {
    
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];  
    [super dealloc];
}
/*
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	for (UITouch *touch in touches) 
	{
		if(touch.phase ==  UITouchPhaseBegan) {
			CGPoint point = [touch locationInView:self];
			int touchId = [touchHelper addTouch: touch];
			MoSync_AddTouchPressedEvent(point.x, point.y, touchId);
			//NSLog(@"%f, %f", point.x, point.y);			
		}
	}	
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	for (UITouch *touch in touches) 
	{
		if(touch.phase ==  UITouchPhaseMoved) {
			CGPoint point = [touch locationInView:self];
			int touchId = [touchHelper getTouchId: touch];
			MoSync_AddTouchMovedEvent(point.x, point.y, touchId);
			//NSLog(@"%f, %f", point.x, point.y);
		}
	}	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {	
    for (UITouch *touch in touches) 
	{
		if(touch.phase ==  UITouchPhaseEnded) {	
			CGPoint point = [touch locationInView:self];
			int touchId = [touchHelper getTouchId: touch];		
			MoSync_AddTouchReleasedEvent(point.x, point.y, touchId);
			[touchHelper removeTouch: touch];
		}
	}	
}
*/

@end
/* Copyright (C) 2009 Mobile Sorcery AB
 
 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import "MoSyncView.h"
#include "MosyncMain.h"

#include "iphone_helpers.h"

#include "Platform.h"



static MoSyncView *currentScreen;

@interface UIApplication(MyExtras) 
- (void)terminateWithSuccess; 
@end

@interface MessageBoxHandler : UIViewController <UIAlertViewDelegate> {
	BOOL kill;
	NSString *msg;
}
@property BOOL kill;
@property (copy, nonatomic) NSString* msg;
- (void)alertViewCancel:(UIAlertView *)alertView;
@end

@implementation MessageBoxHandler
@synthesize kill;
@synthesize msg;
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if(kill)
		Exit();
}
- (void)alertViewCancel:(UIAlertView *)alertView {
	// don't know if this is allowed...
	if(kill)
		Exit();
}
@end

// Used to pass parameters to the widgets, through performSelectorOnMainThread
@interface WidgetHandler : UIViewController <UIAlertViewDelegate> {
	NSString *msg;
	int x,y,l,h;
}
@property (copy, nonatomic) NSString* msg;
@property int x,y,l,h;
@end

@implementation WidgetHandler
@synthesize msg;
@synthesize x,y,l,h;
@end



@implementation MoSyncView

- (void)updateMoSyncView:(CGImageRef)ref {
	mosyncView = ref;
	[self performSelectorOnMainThread : @ selector(setNeedsDisplay) withObject:nil waitUntilDone:YES];
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)decoder {
    if (self = [super initWithCoder:decoder]) {
		[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
		self.frame.origin.y = 0;
		mosyncView = nil;
        // Initialization code
		MoSyncMain(self.frame.size.width, self.frame.size.height, self);
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;

    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, rect, mosyncView);	
	DoneUpdatingMoSyncView();
	 
}


- (void)dealloc {
    [super dealloc];
}

bool down = false;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];

	if(!down) {
		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_PRESSED);
		down = true;
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];
	
	if(down) {
		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_DRAGGED);
	}	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];
	
	if(down) {
		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_RELEASED);	
		down = false;
	}	
}

-(void) messageBox:(id) obj {
	MessageBoxHandler *mbh = (MessageBoxHandler*) obj;
	UIAlertView *alert = [[UIAlertView alloc] 
                          initWithTitle:nil
                          message:mbh.msg
                          delegate:mbh
                          cancelButtonTitle:@"OK"
                          otherButtonTitles:nil];
	
    [alert show];
    [alert release];
}
 
-(void) showMessageBox: (NSString*)msg shouldKill: (bool)kill {
	MessageBoxHandler *mbh = [MessageBoxHandler alloc];
	mbh.kill = kill;
	mbh.msg = msg;
	[self performSelectorOnMainThread: @ selector(messageBox:) withObject:(id)mbh waitUntilDone:NO];
}

-(void) addLabel:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	UILabel *myLabel = [[UILabel alloc] initWithFrame:CGRectMake(wh.x, wh.y, wh.l, wh.h)];
	myLabel.text = wh.msg;
	[self addSubview:myLabel];
	
	
}

-(void) showLabel: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	wh.x = x;
	wh.y = y;
	wh.l = l;
	wh.h = h;
	[self performSelectorOnMainThread: @ selector(addLabel:) withObject:(id)wh waitUntilDone:NO];
	
}

-(void) addButton:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	button.frame = CGRectMake(wh.x, wh.y, wh.l, wh.h);
	[button setTitle:wh.msg forState:UIControlStateNormal];
	[button addTarget:self action:@selector(myAction:) forControlEvents:UIControlEventTouchUpInside];
	[self addSubview:button];
	
}

-(void) showButton: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	wh.x = x;
	wh.y = y;
	wh.l = l;
	wh.h = h;
	[self performSelectorOnMainThread: @ selector(addButton:) withObject:(id)wh waitUntilDone:NO];
	
}

-(void) addTextField:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(wh.x, wh.y, wh.l, wh.h)];
	textField.placeholder = wh.msg;
	//[button setTitle:wh.msg forState:UIControlStateNormal];
	//[button addTarget:self action:@selector(myAction:) forControlEvents:UIControlEventTouchUpInside];
	[self addSubview:textField];
	
}

-(void) showTextField: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	wh.x = x;
	wh.y = y;
	wh.l = l;
	wh.h = h;
	[self performSelectorOnMainThread: @ selector(addTextField:) withObject:(id)wh waitUntilDone:NO];
	
}

-(void) addScreen:(id) obj {
	CGRect frame = CGRectMake(0, 0, 320, 480);
	MoSyncView *v = [[MoSyncView alloc] initWithFrame:frame];
	[self addSubview:v ];
	v.backgroundColor = [UIColor whiteColor];
	[[UIApplication sharedApplication] setStatusBarHidden:NO animated:NO];
	currentScreen = v;
	
}

-(MoSyncView *) showScreen {
	[self performSelectorOnMainThread: @ selector(addScreen:) withObject:(id)nil waitUntilDone:YES];
	return currentScreen;
	
}

-(void) passEvent:(id) obj {
	
}

@end

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
#include "Syscall.h"

// Used for Native UI, not the most elegant thing
// but the only one that I got working
static MoSyncView *currentScreen;
static NSMutableArray *listItems = nil;

@implementation MessageBoxHandler
@synthesize kill;
@synthesize msg;
@synthesize mutableArray;
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

// View Controller for TableView
@implementation MyTableViewController
@synthesize displayedObjects;
- (void)dealloc {
	[displayedObjects release];
    [super dealloc];
}

//  Lazily initializes array of displayed objects.
- (NSMutableArray *)displayedObjects {
    return displayedObjects;
}

//  By default, UITableViewController creates its own UITableView instance and
//  makes itself the table view's data source (and delegate as well),
//  so we can implement methods of the UITableViewDataSource protocol here,
//  and they'll be called automatically by the table view at runtime.
//  You can move these methods to another class if you prefer -- if you do, 
//  just be sure to send a -setDataSource: message to the table view.
//
#pragma mark -
#pragma mark UITableViewDataSource Protocol

//  Returns the number of rows in the current section. There's only
//  one section in a 'plain style' table view, so we can simply ignore the
//  'section' argument.
//
- (NSInteger)tableView:(UITableView *)tableView
 numberOfRowsInSection:(NSInteger)section
{
    return [[self displayedObjects] count];
}

// Returns a cell containing the text to display at the provided row index.
//
- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath 
{
    //  UITableView only needs enough cells to display the rows that are
    //  currently on screen. As the user scrolls, cells that disappear are
    //  placed on a queue so they can be reused to display the values of the
    //  new rows that appear.
    //
    //  The method we're calling here grabs the first available cell off of
    //  that queue. Since its possible for an app to have several table views 
    //  that use different kinds of cells, we provide a reuse identifier to
    //  specify the kind of cell we're looking for.
    //
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MyCell"];
    
    if (cell == nil)
    {
        //  If no cells have been dequeued yet, create a new instance. Use an
        //  identifier so later we can dequeue this kind of cell later (in
        //  case our app has more than one kind of cell table view cell).
        //
        //  The UITableViewCellStyleSubtitle is one of a small handful of 
        //  available styles. This style provides two text areas rendered
        //  on separate lines within the cell. The second line (referred to as
        //  the 'detail' text label) is rendered in a smaller font in a lighter
        //  shade of gray.
        //
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle
                                      reuseIdentifier:@"MyCell"];
        [cell autorelease];
    }
    
    //  Find out which row we're being asked for, and get the corresponding
    //  object from our internal array of displayed objects.
    //
    NSUInteger index = [indexPath row];
    NSString *text = [[self displayedObjects] objectAtIndex:index];
    
    //  Populate the cell's text label with our object's value.
    [[cell textLabel] setText:text];
    
    //  Populate the cell's detail text label.
    NSString *detailText = [NSString stringWithFormat:@"Detail text for %@.", 
                            [text lowercaseString]];
    [[cell detailTextLabel] setText:detailText];
    
    return cell;
}

//
//  The table view's delegate is notified of runtime events, such as when
//  the user taps on a given row, or attempts to add, remove or reorder rows.

//  Notifies the delegate when the user selects a row.
//
- (void)tableView:(UITableView *)tableView
didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	Base::gEventQueue.addNativeUIEvent([currentScreen tag], [indexPath indexAtPosition:1+1]);
    NSLog(@"the tag value is: %d index: %d.", [currentScreen tag], [indexPath indexAtPosition:1]+1);
}

@end

@implementation WidgetHandler
@synthesize msg;
@synthesize x,y,l,h, widgetId, rsc;
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return NO;
}
@end

// Code for the MoSyncView
@implementation MoSyncView
- (void)updateMoSyncView:(CGImageRef)ref {
	mosyncView = ref;
	[self performSelectorOnMainThread : @ selector(setNeedsDisplay) withObject:nil waitUntilDone:YES];
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		self.clearsContextBeforeDrawing = NO;
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

/*
- (void)mTimerProcess{
	DoneUpdatingMoSyncView();
} 
*/

- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;

    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, rect, mosyncView);	
	DoneUpdatingMoSyncView();
	/*
	NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.000001f
													  target:self
													selector:@selector(mTimerProcess)
													userInfo:nil
													 repeats:NO];
	[timer fire];
	*/
	 
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
	myLabel.backgroundColor = [UIColor clearColor];
	//myLabel.font = [UIFont systemFontOfSize:12];
	myLabel.opaque = NO;
	myLabel.shadowColor = [UIColor colorWithWhite:1.0 alpha:0.5];
	myLabel.textAlignment = UITextAlignmentCenter;
	myLabel.textColor = [UIColor darkGrayColor];
	[self addSubview:myLabel];
}

-(void) showLabel: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h widgetId:(int) widgetid {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	wh.x = x;
	wh.y = y;
	wh.l = l;
	wh.h = h;
	wh.widgetId = widgetid;
	[self performSelectorOnMainThread: @ selector(addLabel:) withObject:(id)wh waitUntilDone:NO];
	
}

-(void) addButton:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	button.frame = CGRectMake(wh.x, wh.y, wh.l, wh.h);
	[button setTitle:wh.msg forState:UIControlStateNormal];
	button.tag = wh.widgetId;
	[button addTarget:self action:@selector(passEvent:) forControlEvents:UIControlEventTouchUpInside];
	[self addSubview:button];
	
	
}

-(void) showButton: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h widgetId:(int) widgetid {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	wh.x = x;
	wh.y = y;
	wh.l = l;
	wh.h = h;
	wh.widgetId = widgetid;
	[self performSelectorOnMainThread: @ selector(addButton:) withObject:(id)wh waitUntilDone:NO];
	
}

-(void) addTextField:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(wh.x, wh.y, wh.l, wh.h)];
	textField.placeholder = wh.msg;
	textField.borderStyle = UITextBorderStyleRoundedRect;
	textField.delegate = wh;
	[self addSubview:textField];
}

-(void) showTextField: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h widgetId:(int) widgetid {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	wh.x = x;
	wh.y = y;
	wh.l = l;
	wh.h = h;
	wh.widgetId = widgetid;
	[self performSelectorOnMainThread: @ selector(addTextField:) withObject:(id)wh waitUntilDone:NO];
}

- (void)navigationBar:(UINavigationBar*)bar buttonClicked:(int)button
{
    if (button == 1)
    {
		[self.superview sendSubviewToBack:currentScreen];
        NSLog(@"Navigation bar back button clicked.");
    }

}

//Action methods for toolbar buttons:
/*- (void) pressButton1:(id)sender {
	//Actionsheet
	/*UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle:@"Menu" delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:nil otherButtonTitles:@"test", nil];
	[actionSheet showInView:self];
	[actionSheet release];*/
/*	NSLog(@"+ button clicked.");
}*/

-(void) addScreen:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	MoSyncView *v = [[MoSyncView alloc] initWithFrame:self.frame];
	v.tag = wh.widgetId;
	[self addSubview:v ];
	v.backgroundColor = [UIColor groupTableViewBackgroundColor];
	//v.backgroundColor = [UIColor whiteColor];
	[[UIApplication sharedApplication] setStatusBarHidden:NO animated:NO];
	currentScreen = v;
	Base::gEventQueue.addNativeUIEvent([v tag], 0);
	NSLog(@"the tag value is: %d", [v tag]);
}

-(MoSyncView *) showScreen:(int) widgetid {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.widgetId = widgetid;
	[self performSelectorOnMainThread: @ selector(addScreen:) withObject:(id)wh waitUntilDone:YES];
	return currentScreen;
}

-(void) addScreenList:(id) obj {
	//WidgetHandler *wh = (WidgetHandler*) obj;
	controller = [[MyTableViewController alloc]
				  initWithStyle:UITableViewStyleGrouped];
    controller.displayedObjects = listItems;
    [self addSubview:[controller view]];
	
}

-(MoSyncView *) showScreenList:(int) widgetid {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.widgetId = widgetid;
	[self performSelectorOnMainThread: @ selector(addScreenList:) withObject:(id)wh waitUntilDone:YES];
	return currentScreen;
}

-(void) addListItem:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	[listItems addObject: wh.msg];
}

-(void) showListItem: (NSString*) msg widgetId: (int) widgetid {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.widgetId = widgetid;
	wh.msg = msg;
	if(listItems == nil) listItems = [NSMutableArray arrayWithCapacity:1];
	[self performSelectorOnMainThread: @ selector(addListItem:) withObject:(id)wh waitUntilDone:YES];
}

-(void) addNavigationBar:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	UINavigationBar *nav = [[UINavigationBar alloc] 
							initWithFrame: CGRectMake(0.0f, 20.0f, self.frame.size.width, 48.0f)];
    [nav showButtonsWithLeftTitle: @"Back" 
				   rightTitle: nil leftBack: YES];
	CGRect frame = CGRectMake(0, 0, 320, 48);
	UILabel *label = [[[UILabel alloc] initWithFrame:frame] autorelease];
	label.backgroundColor = [UIColor clearColor];
	label.font = [UIFont boldSystemFontOfSize:20.0];
	label.shadowColor = [UIColor colorWithWhite:0.0 alpha:0.5];
	label.textAlignment = UITextAlignmentCenter;
	label.textColor = [UIColor whiteColor];
	label.text = wh.msg;
	[nav addSubview:label];
    [nav setDelegate: self];
	[self addSubview:nav];
}

-(void) showNavigationBar: (NSString*) msg {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.msg = msg;
	[self performSelectorOnMainThread: @ selector(addNavigationBar:) withObject:(id)wh waitUntilDone:NO];
	
}

-(void) addToolBar:(id) obj {
	//WidgetHandler *wh = (WidgetHandler*) obj;
	toolbar = [UIToolbar new];
	toolbar.barStyle = UIBarStyleDefault;
	[toolbar sizeToFit];
	toolbar.frame = CGRectMake(0, /*(self.frame.size.height - 44)*/480-44, self.frame.size.width, 44);
	[self addSubview:toolbar];
}

-(void) showToolBar {
	WidgetHandler *wh = [WidgetHandler alloc];
	[self performSelectorOnMainThread: @ selector(addToolBar:) withObject:(id)wh waitUntilDone:NO];
}

-(void) addToolBarItem:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(wh.rsc);	
	UIBarButtonItem *systemItem1 = [[UIBarButtonItem alloc] initWithImage:[UIImage imageWithCGImage:img->image]
																	style:UIBarButtonItemStylePlain target:self action:@selector(passEvent:)];
	systemItem1.tag = wh.widgetId;
	[items addObject: systemItem1];
	[toolbar setItems:items animated:YES];
}

-(void) showToolBarItem: (int) widgetid withIcon: (int) rsc {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.widgetId = widgetid;
	wh.rsc = rsc;
	items = [NSMutableArray arrayWithCapacity:1];
	[self performSelectorOnMainThread: @ selector(addToolBarItem:) withObject:(id)wh waitUntilDone:NO];
}

-(void) addImage:(id) obj {
	WidgetHandler *wh = (WidgetHandler*) obj;
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(wh.rsc);
	CGRect myImageRect = CGRectMake(wh.x, wh.y, CGImageGetWidth(img->image), CGImageGetHeight(img->image));
	UIImageView *myImage = [[UIImageView alloc] initWithFrame:myImageRect];
	[myImage setImage:[UIImage imageWithCGImage:img->image]];
	myImage.opaque = YES; // explicitly opaque for performance
	[self addSubview:myImage];
}

-(void) showImage: (int) widgetid withImage: (int) rsc posX:(int) x posY:(int) y {
	WidgetHandler *wh = [WidgetHandler alloc];
	wh.widgetId = widgetid;
	wh.rsc = rsc;
	wh.x = x;
	wh.y = y;
	[self performSelectorOnMainThread: @ selector(addImage:) withObject:(id)wh waitUntilDone:NO];
}

-(void) passEvent:(id) obj {
	Base::gEventQueue.addNativeUIEvent([obj tag], 0);
	NSLog(@"the tag value is: %d", [obj tag]);
}

@end

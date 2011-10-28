//
//  NativeLinpack3ViewController.m
//  NativeLinpack3
//
//  Created by QA on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "NativeLinpack3ViewController.h"

void reg_objc_obj(void *);
void run(void);

@implementation NativeLinpack3ViewController

@synthesize txtFlops;

- (IBAction)bench:(id)sender {
    reg_objc_obj(self);
    run();
}

- (void)updateFlops:(double)mflops {
    NSString *flopsString;
    flopsString = [[NSString alloc] initWithFormat:@"%f",mflops];
    txtFlops.text = flopsString;
    
    [flopsString release];
}

- (void)dealloc
{
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
}
*/

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end

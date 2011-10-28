//
//  MemBenchNativeIOSViewController.m
//  MemBenchNativeIOS
//
//  Created by QA on 7/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MemBenchNativeIOSViewController.h"
#import "membench.h"

@implementation MemBenchNativeIOSViewController

- (IBAction)startBench:(id)sender 
{
    //Start the benchmark
    txt = [NSString stringWithFormat:@"%s", ""];
    MemBench * mb = new MemBench;
    mb->reg_objc_obj(self);
    mb->bench();
    
    delete mb;
}

- (void)postResult:(char *)res 
{
    txt = [NSString stringWithFormat:@"%@%s", txt, res];
    txtView.text = txt;	
    //[txt release];
    
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

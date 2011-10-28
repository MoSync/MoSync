//
//  StatsViewController.m
//  GLBenchIOSNative
//
//  Created by QA on 7/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "StatsViewController.h"


@implementation StatsViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
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


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{

    //create a frame that sets the bounds of the view
    CGRect frame = CGRectMake(0, 0, 320, 480);
    
    //allocate the view
    self.view = [[UIView alloc] initWithFrame:frame];
    
    //set the view's background color
    self.view.backgroundColor = [UIColor whiteColor];
    
    //set the position of the label
    frame = CGRectMake(100, 170, 100, 50);
    
    //allocate the label
    UILabel *label = [[UILabel alloc] initWithFrame:frame];
    
    //set the label's text
    label.text = @"Hello World!";
    
    //add the label to the view
    [self.view addSubview:label];
    
    //release the label
    [label release];
    
}


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

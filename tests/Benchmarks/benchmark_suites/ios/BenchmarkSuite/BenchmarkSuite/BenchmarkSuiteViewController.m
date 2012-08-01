//
//  BenchmarkSuiteViewController.m
//  BenchmarkSuite
//
//  Created by Alexander Samuelsson on 8/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "BenchmarkSuiteViewController.h"

@implementation BenchmarkSuiteViewController

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

-(void)runNextBench{
    UIApplication *ourApplication = [UIApplication sharedApplication];
    NSString *ourPath = @"opengles://";
    NSURL *ourURL = [NSURL URLWithString:ourPath];
    if ([ourApplication canOpenURL:ourURL]) {
        [ourApplication openURL:ourURL];
    }
    else {
        //Display error
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"GLBenchNativeiOS Not Found" message:@"It must be installed to perform the next benchmark." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alertView show];
        [alertView release];
    }
    exit(0);
}

@end

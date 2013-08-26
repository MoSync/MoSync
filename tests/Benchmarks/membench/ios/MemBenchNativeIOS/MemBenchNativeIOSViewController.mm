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

//
//	MemBenchNativeIOSViewController.m
//	MemBenchNativeIOS
//
//	Created by QA on 7/14/11.
//

#import "MemBenchNativeIOSViewController.h"
#import "membench.h"

@implementation MemBenchNativeIOSViewController
@synthesize txtView;
@synthesize activityIndicatorView;

- (IBAction)startBench:(id)sender
{
	//Start the benchmark
	[activityIndicatorView startAnimating];

	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		txt = [NSString stringWithFormat:@"%s", ""];
		MemBench * mb = new MemBench;
		mb->reg_objc_obj(self);
		mb->bench();
		NSLog(@"\n\nRunning in the background!\n\n");
		delete mb;
	});



}

- (void)postResult:(char *)res
{
	[activityIndicatorView stopAnimating];
	txt = [NSString stringWithFormat:@"%@%s", txt, res];
	dispatch_async(dispatch_get_main_queue(), ^{ //call UIKit on the main thread
		txtView.text = txt;
	});
	//[txt release];

}

-(void)runNextBench:(char*)msg{
	UIApplication *ourApplication = [UIApplication sharedApplication];
	NSString *ourPath = @"linpack://";
	NSURL *ourURL = [NSURL URLWithString:ourPath];
	if ([ourApplication canOpenURL:ourURL]) {
		[ourApplication openURL:ourURL];
	}
	else {
		//Display error
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"NativeLinpackiOS Not Found" message:@"It must be installed to perform the next benchmark." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
		[alertView show];
		[alertView release];
	}
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


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
	[super viewDidLoad];

	UIActivityIndicatorView *ai = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
	ai.center = self.view.center;
	[self.view addSubview:ai];
}


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

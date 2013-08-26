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
//	NativeLinpack3ViewController.m
//	NativeLinpack3
//
//	Created by QA on 6/20/11.
//

#import "NativeLinpack3ViewController.h"

void reg_objc_obj(void *);
void run(void);

@implementation NativeLinpack3ViewController

@synthesize txtFlops;
@synthesize activityIndicator;

- (IBAction)bench:(id)sender {
	reg_objc_obj(self);
	run();
}

- (void)updateFlops:(double)mflops {
	NSString *flopsString;
	flopsString = [[NSString alloc] initWithFormat:@"%f",mflops];
	txtFlops.text = flopsString;

	//send result to database
	mBr.benchmark = "linpack";
	mBr.mflops = mflops;

	BenchDBConnector * bdbc = [[BenchDBConnector alloc] init];
	[bdbc submit:mBr];
	//[activityIndicator stopAnimating]; //stop the activityIndicator from spinning

	[flopsString release];
	sleep(2);
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

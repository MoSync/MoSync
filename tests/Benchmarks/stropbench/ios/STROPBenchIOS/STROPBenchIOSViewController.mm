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
//  STROPBenchIOSViewController.m
//  STROPBenchIOS
//
//  Created by QA on 7/8/11.
//

#import "STROPBenchIOSViewController.h"
#import "stropbench.h"

@implementation STROPBenchIOSViewController

- (IBAction)startBench:(id)sender
{
    //Start the benchmark
    txt = [NSString stringWithFormat:@"%s", ""];
    StropBencher * sb = new StropBencher;
    sb->reg_objc_obj(self);
    sb->bench();

    delete sb;
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

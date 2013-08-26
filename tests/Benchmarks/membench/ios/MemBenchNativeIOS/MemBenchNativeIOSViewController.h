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
//  MemBenchNativeIOSViewController.h
//  MemBenchNativeIOS
//
//  Created by QA on 7/14/11.
//

#import <UIKit/UIKit.h>

@interface MemBenchNativeIOSViewController : UIViewController {
    IBOutlet UITextView *txtView;
    IBOutlet UIActivityIndicatorView *activityIndicatorView;
    NSMutableString *txt;
}

@property(nonatomic,retain) IBOutlet UITextView *txtView;
@property(nonatomic,retain) IBOutlet UIActivityIndicatorView *activityIndicatorView;

- (IBAction)startBench:(id)sender;
- (void)postResult:(char *)res;
- (void)runNextBench:(char*)msg;

@end

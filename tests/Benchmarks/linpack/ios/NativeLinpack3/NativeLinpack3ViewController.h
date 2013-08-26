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
//  NativeLinpack3ViewController.h
//  NativeLinpack3
//
//  Created by QA on 6/20/11.
//

#import <UIKit/UIKit.h>
#import "BenchDB.h"

@interface NativeLinpack3ViewController : UIViewController {
    IBOutlet UITextField *txtFlops;
    IBOutlet UIActivityIndicatorView *activityIndicator;
    struct BenchResult mBr; //bench result struct
}

@property(nonatomic,retain) IBOutlet UITextField *txtFlops;
@property(nonatomic,retain) IBOutlet UIActivityIndicatorView *activityIndicator;

- (IBAction) bench:(id) sender;
- (void)updateFlops:(double)mflops;

@end

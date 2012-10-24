//
//  MemBenchNativeIOSViewController.h
//  MemBenchNativeIOS
//
//  Created by QA on 7/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
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

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
    NSMutableString *txt;
}

@property(nonatomic,retain) IBOutlet UITextView *txtView;

- (IBAction)startBench:(id)sender;
- (void)postResult:(char *)res;

@end

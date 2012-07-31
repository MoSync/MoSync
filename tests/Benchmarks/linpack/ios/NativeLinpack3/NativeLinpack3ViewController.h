//
//  NativeLinpack3ViewController.h
//  NativeLinpack3
//
//  Created by QA on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
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

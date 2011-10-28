//
//  NativeLinpack3ViewController.h
//  NativeLinpack3
//
//  Created by QA on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NativeLinpack3ViewController : UIViewController {
    IBOutlet UITextField *txtFlops;
}

@property(nonatomic,retain) IBOutlet UITextField *txtFlops;

- (IBAction) bench:(id) sender;
- (void)updateFlops:(double)mflops;

@end

//
//  STROPBenchIOSViewController.h
//  STROPBenchIOS
//
//  Created by QA on 7/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface STROPBenchIOSViewController : UIViewController {
    IBOutlet UITextView *txtView;
    NSMutableString *txt;
}

@property(nonatomic,retain) IBOutlet UITextView *txtView;

- (IBAction)startBench:(id)sender;
- (void)postResult:(char *)res;

@end

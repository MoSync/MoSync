//
//  StatsView.h
//  GLBenchIOSNative
//
//  Created by QA on 7/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol StatsViewDelegate
- (void)drawView:(UIView *)theView;
- (void)setupView:(UIView *)theView;
@end

@interface StatsView : UIView 
{
    
@private
    
    id <StatsViewDelegate>     delegate;
    
}
@property (assign) /* weak ref */ id <StatsViewDelegate> delegate;
- (void)drawView;
@end
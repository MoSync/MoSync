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
//  StatsView.h
//  GLBenchIOSNative
//
//  Created by QA on 7/1/11.

#import <UIKit/UIKit.h>

@protocol StatsViewDelegate
- (void)drawView:(UIView *)theView;
//- (void)setupView:(UIView *)theView;
@end

@interface StatsView : UIView
{

@private

    id <StatsViewDelegate>     delegate;

}
@property (assign) /* weak ref */ id <StatsViewDelegate> delegate;
//- (void)drawView;
@end

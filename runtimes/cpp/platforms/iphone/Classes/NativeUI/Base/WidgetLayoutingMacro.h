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

#ifndef MOSYNC_WIDGET_LAYOUTING_MACRO_h
#define MOSYNC_WIDGET_LAYOUTING_MACRO_h

// this generates a wrapper that routes the layoutSubviews and sizeThatFits commands to an IWidget
// make sure not to call the IWidget::view.layoutSubviews or IWidget::view.sizeThatFits from the function  but the super* functions..
#define MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSyncName, UIViewName) \
@interface MoSyncName ## UIViewName : UIViewName {\
IWidget* mWidget;\
}\
- (void)setWidget:(IWidget*)widget;\
- (void)layoutSubviews;\
- (void)superLayoutSubviews;\
@end\
@implementation MoSync##UIViewName \
- (void)setWidget:(IWidget*)widget { \
mWidget = widget; \
}\
- (void)layoutSubviews {\
[mWidget layoutSubviews:self];\
}\
- (void)superLayoutSubviews {\
[super layoutSubviews];\
}\
@end\

#endif

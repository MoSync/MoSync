/* Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
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

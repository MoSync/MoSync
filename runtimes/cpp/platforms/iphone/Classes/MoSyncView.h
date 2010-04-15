/* Copyright (C) 2009 Mobile Sorcery AB
 
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

#import <UIKit/UIKit.h>

@interface MoSyncView : UIView {
	CGImageRef mosyncView;

}

-(void) updateMoSyncView: (CGImageRef)ref;
-(void) showMessageBox:(NSString*)msg shouldKill:(bool)kill;
-(void) addLabel:(id) obj;
-(void) showLabel:(NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h widgetId:(int) widgetid;
-(void) addButton:(id) obj;
-(void) showButton:(NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h widgetId:(int) widgetid;
-(void) addTextField:(id) obj;
-(void) showTextField: (NSString*) msg posX:(int) x posY:(int) y length:(int) l height:(int) h widgetId:(int) widgetid;
- (void)navigationBar:(UINavigationBar*)bar buttonClicked:(int)button;
-(void) addNavigationBar:(id) obj;
-(void) showNavigationBar: (NSString*) msg;
-(void) addScreen:(id) obj;
-(MoSyncView *) showScreen:(int) widgetid;
-(void) passEvent:(id) obj;

@end

/*
 Copyright (C) 2013 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

/**
 * @file MoSyncUIAlertView.h
 * @author Bogdan Iusco
 *
 * @brief Shown an alert view on the screen.
 * Used by the maAlert and maMessageBox syscalls.
 */


#import <Foundation/Foundation.h>

@interface MoSyncUIAlertView : NSObject <UIAlertViewDelegate>

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+ (MoSyncUIAlertView*)getInstance;

/**
 * Releases the class instance.
 */
+ (void)deleteInstance;

/**
 * Set/Get the string that appears in the alert view's title bar.
 * Will be set to nil when a new alert view is shown(when showAlertView method is called).
 */
@property (nonatomic, retain) NSString* title;

/**
 * Set/Get the text message displayed by the alert view.
 * Will be set to nil when a new alert view is shown(when showAlertView method is called).
 */
@property (nonatomic, retain) NSString* message;

/**
 * Add a new button for the next shown alert view(when showAlertView is called).
 * @param title Title for the new button. If it's nil or empty string it will not be added.
 */
- (void)addButtonTitle:(NSString*)title;

/**
 * Show alert view with previously set attributes(such as title, message).
 * Alert view's title and message will be set to nil. Also, previously added
 * buttons will be removed.
 */
- (void)showAlertView;

/**
 * Show alert view with given options.
 * Calling this method doesn't modify previously set title, message or any added button.
 * @param title The string that appears in the alert view's title bar.
 * @param message The text message displayed by the alert view.
 * @param button1 First button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button2 Second button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button1 Third button's title in the alert view. If nil or empty string
 * the button will not be shown.
 */
- (void)showAlertView:(NSString*)title
			  message:(NSString*)message
			  button1:(NSString*)button1
			  button2:(NSString*)button2
			  button3:(NSString*)button3;

/**
 * Show alert view with given options.
 * Calling this method doesn't modify previously set title, message or any added button.
 * @param titleChar The string that appears in the alert view's title bar.
 * @param messageChar The text message displayed by the alert view.
 * @param button1Char First button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button2Char Second button's title in the alert view. If nil or empty string
 * the button will not be shown.
 * @param button1Char Third button's title in the alert view. If nil or empty string
 * the button will not be shown.
 */
- (void)showAlertViewChar:(const char*)titleChar
			  messageChar:(const char*)messageChar
			  button1Char:(const char*)button1Char
			  button2Char:(const char*)button2Char
			  button3Char:(const char*)button3Char;
@end

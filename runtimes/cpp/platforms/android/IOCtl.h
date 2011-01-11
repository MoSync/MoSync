/* Copyright (C) 2010 MoSync AB

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

#include <Syscall.h>

namespace Base
{
	// Framebuffer API
	
	int _maFrameBufferGetInfo(MAFrameBufferInfo *info);
	
	int _maFrameBufferInit(void *data, int memStart, JNIEnv* jNIEnv, jobject jThis);
	
	int _maFrameBufferClose(JNIEnv* jNIEnv, jobject jThis);
	
	// Bluetooth API
	
	int _maBtStartDeviceDiscovery(int names, JNIEnv* jNIEnv, jobject jThis);
	
	int _maBtGetNewDevice(
		int memStart, 
		int nameBufPointer, 
		int nameBufSize, 
		int actualNameLengthPointer,
		int addressPointer,
		JNIEnv* jNIEnv, 
		jobject jThis);
		
	int _maBtStartServiceDiscovery(
		MABtAddr* addressPointer, 
		MAUUID* uuidPointer, 
		JNIEnv* jNIEnv, 
		jobject jThis);
		
	int _maBtGetNextServiceSize(
		int memStart,
		int nameBufSizePointer,
		int nUuidsPointer,
		JNIEnv* jNIEnv, 
		jobject jThis);
		
	int _maBtGetNewService(
		int memStart,
		int portPointer,
		int nameBufPointer,
		int nameBufSize,
		int uuidsPointer,
		JNIEnv* jNIEnv, 
		jobject jThis);
		
	int _maBtCancelDiscovery(JNIEnv* jNIEnv, jobject jThis);
	
	// Server API
		
	int _maAccept(int serverHandle, JNIEnv* jNIEnv, jobject jThis);
	
	// Location API
	
	int _maLocationStart(JNIEnv* jNIEnv, jobject jThis);
	
	int _maLocationStop(JNIEnv* jNIEnv, jobject jThis);
	
	// Other IOCTLs
	
	int _maGetSystemProperty(const char* key, int buf, int memStart, int size, JNIEnv* jNIEnv, jobject jThis);

	int _maPlatformRequest(const char* url, JNIEnv* jNIEnv, jobject jThis);
	
	int _maWriteLog(const char* str, int b, JNIEnv* jNIEnv, jobject jThis);
	
	/**
	 * Internal function corresponding to the maShowVirtualKeyboard IOCtl.
	 * Shows the android soft keyboard.
	 *
	 * @param jNIEnv	JNI environment used
	 * @param jThis		Pointer to the java class
	 *
	 * @return			Value returned by the maTextBox 
	 *					java method 
	 */
	int _maShowVirtualKeyboard(JNIEnv* jNIEnv, jobject jThis);
	
	/**
	 * Internal function corresponding to the maTextBox IOCtl.
	 * Displays a full screen editable text field with 
	 * OK and Cancel buttons.
	 *
	 * @param title			Title of the text box
	 * @param inText		Initial content of the text box
	 * @param outText		Buffer that will contain the text 
	 *						entered by the user
	 * @param maxSize		Maximum size of outText
	 * @param constraints	Not implemented yet
	 * @param memStart		Pointer to the begining of the 
	 *						MoSync memory
	 * @param jNIEnv		JNI environment used
	 * @param jThis			Pointer to the java class
	 *
	 * @return				Value returned by the maTextBox 
	 *						java method
	 */
	int _maTextBox(
		const wchar* title, 
		const wchar* inText, 
		int outText, 
		int maxSize, 
		int constraints, 
		int memStart, 
		JNIEnv* jNIEnv, 
		jobject jThis);

	/**
	 * Add a notification item.
	 *
	 * Note that there can only be one notification of type
	 * NOTIFICATION_TYPE_APPLICATION_LAUNCHER. Additional notification 
	 * types may be added in the future. This syscall is available 
	 * on Android only.
	 *
	 * @param type The \link #NOTIFICATION_TYPE_APPLICATION_LAUNCHER 
	 * \endlink constant.
	 * @param id The id of the notification. The id must be unique within 
	 * the application.
	 * @param title Title of the notification.
	 * @param text String to be displayed as part of the notification.
	 * @return \< 0 on error or if the syscall is not available on the 
	 * current platform.
	 */
	int _maNotificationAdd(
		int type, 
		int id, 
		const char* title, 
		const char* text, 
		JNIEnv* jNIEnv, 
		jobject jThis);

	/**
	 * Remove a notification item.
	 * @param id The id of the notification.
	 * @return \< 0 on error.
	 */
	int _maNotificationRemove(int id, JNIEnv* jNIEnv, jobject jThis);
	
	/**
	* Sends the application to the background, unless it's already there.
	* Generates a \link #EVENT_TYPE_FOCUS_LOST FOCUS_LOST \endlink event.
	* \note Only available on multi-tasking operating systems.
	*/
	int _maSendToBackground(JNIEnv* jNIEnv, jobject jThis);
	
	/**
	 * Set the screen orientation.
	 * @param orientation One of the \link #SCREEN_ORIENTATION 
	 * \endlink constants.
	 * @return \< 0 on error.
	 */
	int _maScreenSetOrientation(int orientation, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Enable/disable fullscreen mode.
	 * @param fullscreen 1 for fullscreen on, 0 for fullscreen off.
	 * @return \< 0 on error.
	 */
	int _maScreenSetFullscreen(int fullscreen, JNIEnv* jNIEnv, jobject jThis);
	
	/**
	* Set the background image of the phone's home screen.
	* @param data Image data in JPEG or PNG format.
	* @return \< 0 on error.
	*/
	int _maWallpaperSet(MAHandle data, JNIEnv* jNIEnv, jobject jThis);
		
	/**
	* Turn on/off sending of HomeScreen events. Off by default.
	* @param eventsOn 1 = events on, 0 = events off
	* @return \< 0 on error.
	*/
	int _maHomeScreenEventsOnOff(int eventsOn, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Add shortcut icon to the device home screen.
	* Available on Android only.
	* @return \< 0 on error.
	*/
	int _maHomeScreenShortcutAdd(const char* name, JNIEnv* jNIEnv, jobject jThis);	
	
	/**
	* Remove shortcut icon from the device home screen.
	* Available on Android only.
	* @return \< 0 on error.
	*/
	int _maHomeScreenShortcutRemove(const char* name, JNIEnv* jNIEnv, jobject jThis);

	int _maTextBox(const wchar* title, const wchar* inText, int outText, int maxSize, int constraints, int memStart, JNIEnv* jNIEnv, jobject jThis);
	
	// Widget API
	int _maWidgetCreate(const char *widgetType, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetDestroy(int widget, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetAddChild(int parent, int child, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetRemoveChild(int parent, int child, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetSetProperty(int widget, const char *property, const char* value, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetEvauluateScript(int widget, const char *script, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetGetProperty(int widget, const char *property, const char* valueBuffer, int bufferSize, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetGetMessageData(int memStart, int messageId, int messageBufferPointer, int bufferSize, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetScreenShow(int screenWidget, JNIEnv* jNIEnv, jobject jThis);
}

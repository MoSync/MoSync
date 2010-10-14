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

#include <syscall.h>

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
	int _maTextBox(const wchar* title, const wchar* inText, int outText, int maxSize, int constraints, int memStart, JNIEnv* jNIEnv, jobject jThis);

	// Widget API
	int _maWidgetCreate(int widgetType, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetDestroy(int widgetHandle, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetOpen(int widgetHandle, int widgetParentHandle, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetClose(int widgetHandle, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetLoadHTML(int widgetHandle, const char* html, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetLoadURL(int widgetHandle, const char* url, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetEvaluateScript(int widgetHandle, const char* script, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetGetMessageSize(int messageId, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetGetMessage(int messageId, int buf, int memStart, int size, JNIEnv* jNIEnv, jobject jThis);
}

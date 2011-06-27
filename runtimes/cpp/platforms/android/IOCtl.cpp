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

#include "IOCtl.h"

#include <helpers/cpp_defs.h>

/**
 * Custom Wide Char String length calculation function.
 * Exists because the android NDK does not support wchars.
 *
 * @param s		input multi-byte string
 *
 * @return		length of the input string
 *
 * TODO: Use a build in function instead if this one.
 * This function is written in a non-readable way,
 * improved it a little bit by changing a variable name.
 */
inline size_t wideCharStringLength(const wchar * s)
{
	const wchar *current;
	if (s == 0)
	{
		return 0;
	}
	for (current = s; *current; ++current);
	return current - s;
}

namespace Base
{		
	int _maFrameBufferGetInfo(MAFrameBufferInfo *info)
	{	
		int size = maGetScrSize();
		int width = (size&0xffff0000) >> 16;
		int height = size&0x0000ffff;
	
		info->bitsPerPixel = 32;
		info->bytesPerPixel = 4;
		info->redMask = 0x000000ff;
		info->greenMask = 0x0000ff00;
		info->blueMask = 0x00ff0000;

		
		info->width = width;
		info->height = height;
		info->pitch = info->width*4;
		
		info->sizeInBytes = info->pitch * info->height;
		
		info->redShift = 0;
		info->greenShift = 8;
		info->blueShift = 16;

		info->redBits = 8;
		info->greenBits = 8;
		info->blueBits = 8;
		
		info->supportsGfxSyscalls = 0;

		return 1;
	}


	int _maFrameBufferInit(void *data, int memStart, JNIEnv* jNIEnv, jobject jThis)
	{	
		int rdata = (int)data - memStart;
	
		char* b = (char*)malloc(200);
		sprintf(b,"Framebuffer data: %i", rdata);
		__android_log_write(ANDROID_LOG_INFO,"JNI",b);
		free(b);
	
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "_enableFramebuffer", "(I)V");
		if (methodID == 0) return 0;
		jNIEnv->CallVoidMethod(jThis, methodID, rdata);
		
		jNIEnv->DeleteLocalRef(cls);
	
		return 1;
	}

	int _maFrameBufferClose(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "_disableFramebuffer", "()V");
		if (methodID == 0) return 0;
		jNIEnv->CallVoidMethod(jThis, methodID);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return 1;
	}
	
	int _maBtStartDeviceDiscovery(int names, JNIEnv* jNIEnv, jobject jThis)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtStartDeviceDiscovery begin");
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtStartDeviceDiscovery", "(I)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, names);
		
		jNIEnv->DeleteLocalRef(cls);
		
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtStartDeviceDiscovery end");
		
		return (int)ret;
	}
	
	/**
	 * Returns actual length of device name.
	 */
	int _maBtGetNewDevice(
		int memStart, 
		int nameBufPointer, 
		int nameBufSize, 
		int actualNameLengthPointer,
		int addressPointer,
		JNIEnv* jNIEnv, 
		jobject jThis)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtGetNewDevice begin");
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtGetNewDevice", "(IIII)I");
		if (methodID == 0) return 0;
		
		jint ret = jNIEnv->CallIntMethod(
			jThis, 
			methodID, 
			nameBufPointer - memStart,
			nameBufSize,
			actualNameLengthPointer - memStart,
			addressPointer - memStart);
		
		jNIEnv->DeleteLocalRef(cls);
		
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtGetNewDevice end");
		
		return (int)ret;
	}
	
	int _maBtStartServiceDiscovery(MABtAddr* addr, MAUUID* uuid, JNIEnv* jNIEnv, jobject jThis)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtStartServiceDiscovery begin");
		
		// Device address converted to string.
		char addressBuf[64];
		sprintf(addressBuf, "%02X%02X%02X%02X%02X%02X", 
			addr->a[0], addr->a[1], addr->a[2], addr->a[3], addr->a[4], addr->a[5]);
		jstring jstrAddress = jNIEnv->NewStringUTF(addressBuf);
		
		// UUID converted to string.
		char uuidBuf[64];
		char* u = (char*) uuid;
		sprintf(uuidBuf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
			u[3], u[2], u[1], u[0], u[7], u[6], u[5], u[4], u[11], u[10], u[9], u[8], u[15], u[14], u[13], u[12]);
		jstring jstrUUID = jNIEnv->NewStringUTF(uuidBuf);
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtStartServiceDiscovery", "(Ljava/lang/String;Ljava/lang/String;)I");
		if (methodID == 0) return 0;
		
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, jstrAddress, jstrUUID);
		
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrAddress);
		jNIEnv->DeleteLocalRef(jstrUUID);
		
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtStartServiceDiscovery end");
		
		return (int)ret;
	}
	
	int _maBtGetNextServiceSize(
		int memStart,
		int nameBufSizePointer,
		int nUuidsPointer,
		JNIEnv* jNIEnv, 
		jobject jThis)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtGetNextServiceSize begin");
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtGetNextServiceSize", "(II)I");
		if (methodID == 0) return 0;
		
		jint ret = jNIEnv->CallIntMethod(
			jThis, 
			methodID, 
			nameBufSizePointer - memStart,
			nUuidsPointer - memStart);
		
		jNIEnv->DeleteLocalRef(cls);
		
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtGetNextServiceSize end");
		
		return (int)ret;
	}
	
	int _maBtGetNewService(
		int memStart,
		int portPointer,
		int nameBufPointer,
		int nameBufSize,
		int uuidsPointer,
		JNIEnv* jNIEnv, 
		jobject jThis)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtGetNewService begin");
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtGetNewService", "(IIII)I");
		if (methodID == 0) return 0;
		
		jint ret = jNIEnv->CallIntMethod(
			jThis, 
			methodID, 
			portPointer - memStart,
			nameBufPointer - memStart,
			nameBufSize,
			uuidsPointer - memStart);
		
		jNIEnv->DeleteLocalRef(cls);
		
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtGetNewService end");
		
		return (int)ret;
	}
	
	int _maBtCancelDiscovery(JNIEnv* jNIEnv, jobject jThis)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtCancelDiscovery begin");
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtCancelDiscovery", "()I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID);
		jNIEnv->DeleteLocalRef(cls);
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtCancelDiscovery end");
		
		return (int)ret;
	}
	
	int _maAccept(int serverHandle, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maAccept", "(I)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, serverHandle);
		jNIEnv->DeleteLocalRef(cls);
		return (int)ret;
	}
	
	int _maLocationStart(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maLocationStart", "()I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)ret;
	}
	
	int _maLocationStop(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maLocationStop", "()I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)ret;
	}
	
	int _maGetSystemProperty(const char* key, int buf, int memStart, int size, JNIEnv* jNIEnv, jobject jThis)
	{
		jstring jstrKey = jNIEnv->NewStringUTF(key);

		int rBuf = buf - memStart;
	
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maGetSystemProperty", "(Ljava/lang/String;II)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, jstrKey, rBuf, size);
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrKey);
		
		return (int)ret;
	}
	
	int _maPlatformRequest(const char* url, JNIEnv* jNIEnv, jobject jThis)
	{
		jstring jstrURL = jNIEnv->NewStringUTF(url);
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maPlatformRequest", "(Ljava/lang/String;)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, jstrURL);
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrURL);
		
		return (int)ret;
	}
	
	int _maWriteLog(const char* str, int b, JNIEnv* jNIEnv, jobject jThis)
	{
		jstring jstrLOG = jNIEnv->NewStringUTF(str);
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWriteLog", "(Ljava/lang/String;I)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, jstrLOG);
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrLOG);
		
		return (int)ret;
	}
	
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
	int _maShowVirtualKeyboard(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = 
			jNIEnv->GetMethodID(cls, "maShowVirtualKeyboard", "()I");
		if (methodID == 0) return 0;
		int ret = jNIEnv->CallIntMethod(jThis, methodID);
		jNIEnv->DeleteLocalRef(cls);
		
		return ret;
	}
	
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
		jobject jThis)
	{

		// Initialization.
		jstring jstrTITLE = jNIEnv->NewString((jchar*)title, wideCharStringLength(title));
		jstring jstrINTEXT = jNIEnv->NewString((jchar*)inText, wideCharStringLength(inText));
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		// Remove the offset from the output buffer's address
		int rBuf = outText - memStart;
		
		// Call the java method
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maTextBox", 
												 "(Ljava/lang/String;Ljava/lang/String;III)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, jstrTITLE,
										 jstrINTEXT, rBuf, maxSize, constraints);
		
		// Clean
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrTITLE);
		jNIEnv->DeleteLocalRef(jstrINTEXT);
		
		return (int)ret;
	}
	
	int _maMessageBox(const char* title, const char* text, JNIEnv* jNIEnv, jobject jThis)
	{
		Base::gSyscall->VM_Yield();
		
		jstring jstrTitle = jNIEnv->NewStringUTF(title);
		jstring jstrText = jNIEnv->NewStringUTF(text);
		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maMessageBox", "(Ljava/lang/String;Ljava/lang/String;)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, jstrTitle, jstrText);
		
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrTitle);
		jNIEnv->DeleteLocalRef(jstrText);
		
		return ret;
	}
	
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
		jobject jThis)
	{
		jstring jstrTitle = jNIEnv->NewStringUTF(title);
		jstring jstrText = jNIEnv->NewStringUTF(text);
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maNotificationAdd", 
			"(IILjava/lang/String;Ljava/lang/String;)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(
			jThis, 
			methodID, 
			type, 
			id, 
			jstrTitle, 
			jstrText);
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrTitle);
		jNIEnv->DeleteLocalRef(jstrText);
		
		return (int)result;
	}

	/**
	 * Remove a notification item.
	 * @param id The id of the notification.
	 * @return \< 0 on error.
	 */
	int _maNotificationRemove(int id, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maNotificationRemove", 
			"(I)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, id);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	/**
	* Sends the application to the background, unless it's already there.
	* Generates a \link #EVENT_TYPE_FOCUS_LOST FOCUS_LOST \endlink event.
	* \note Only available on multi-tasking operating systems.
	*/
	int _maSendToBackground(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maSendToBackground", 
			"()I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	/**
	 * Set the screen orientation.
	 * @param orientation One of the \link #SCREEN_ORIENTATION 
	 * \endlink constants.
	 * @return \< 0 on error.
	 */
	int _maScreenSetOrientation(int orientation, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maScreenSetOrientation", 
			"(I)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, orientation);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	/**
	 * Enable/disable fullscreen mode.
	 * @param fullscreen 1 for fullscreen on, 0 for fullscreen off.
	 * @return \< 0 on error.
	 */
	int _maScreenSetFullscreen(int fullscreen, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maScreenSetFullscreen", 
			"(I)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, fullscreen);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	
	
	/**
	* Turn on/off sending of HomeScreen events. Off by default.
	* @param eventsOn 1 = events on, 0 = events off
	* @return \< 0 on error.
	*/
	int _maHomeScreenEventsOnOff(int eventsOn, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maHomeScreenEventsOnOff", 
			"(I)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, eventsOn);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	/**
	* Add shortcut icon to the device home screen.
	* Available on Android only.
	* @param name The name (label) of the shortcut to add.
	* @return \< 0 on error.
	*/
	int _maHomeScreenShortcutAdd(const char* name, JNIEnv* jNIEnv, jobject jThis)
	{
		jstring jstrName = jNIEnv->NewStringUTF(name);
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maHomeScreenShortcutAdd", 
			"(Ljava/lang/String;)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, jstrName);
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrName);
		
		return (int)result;
	}
	
	/**
	* Remove shortcut icon from the device home screen.
	* Available on Android only.
	* @param name The name (label) of the shortcut to remove.
	* @return \< 0 on error.
	*/
	int _maHomeScreenShortcutRemove(const char* name, JNIEnv* jNIEnv, jobject jThis)
	{
		jstring jstrName = jNIEnv->NewStringUTF(name);
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maHomeScreenShortcutRemove", 
			"(Ljava/lang/String;)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, jstrName);
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrName);
		
		return (int)result;
	}
	
	/**
	* Turn on/off sending of screen on/off events. Off by default.
	* @param eventsOn 1 = events on, 0 = events off
	* @return \< 0 on error.
	*/
	int _maScreenStateEventsOnOff(int eventsOn, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
			cls, 
			"maScreenStateEventsOnOff", 
			"(I)I");
		if (methodID == 0) return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, eventsOn);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maWidgetCreate(const char *widgetType, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jstring jstrWidgetType = jNIEnv->NewStringUTF(widgetType);
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetCreate", "(Ljava/lang/String;)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the Java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, jstrWidgetType);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrWidgetType);
		
		return result;
	}
	
	int _maWidgetDestroy(int widget, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetDestroy", "(I)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, widget);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}
	
	int _maWidgetAddChild(int parent, int child, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetAddChild", "(II)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, parent, child);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}

	int _maWidgetInsertChild(int parent, int child, int index, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetInsertChild", "(III)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, parent, child, index);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}

	
	int _maWidgetRemoveChild(int child, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetRemoveChild", "(I)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, child);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}
	
	int _maWidgetSetProperty(int widget, const char *property, const char* value, JNIEnv* jNIEnv, jobject jThis)
	{
		// Convert to Java parameters
		jstring jstrProperty = jNIEnv->NewStringUTF(property);
		jstring jstrValue = jNIEnv->NewStringUTF(value);
		
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetSetProperty", "(ILjava/lang/String;Ljava/lang/String;)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, widget, jstrProperty, jstrValue);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrValue);
		jNIEnv->DeleteLocalRef(jstrProperty);
		
		return result;
	}
	
	int _maWidgetGetProperty(int memStart, int widget, const char *property, int memBuffer, int bufferSize, JNIEnv* jNIEnv, jobject jThis)
	{
		// Convert to Java parameters
		jstring jstrProperty = jNIEnv->NewStringUTF(property);
		
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetGetProperty", "(ILjava/lang/String;II)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, widget, jstrProperty, memBuffer - memStart, bufferSize);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrProperty);
		
		return result;
	}

	int _maWidgetScreenShow(int screenWidget, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetScreenShow", "(I)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, screenWidget);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}

	int _maWidgetStackScreenPush(int stackScreenWidget, int newScreen, JNIEnv* jNIEnv, jobject jThis)
	{
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetStackScreenPush", "(II)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, stackScreenWidget, newScreen);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}

	int _maWidgetStackScreenPop(int stackScreenWidget, JNIEnv* jNIEnv, jobject jThis)
	{	
		// Get the Java method
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maWidgetStackScreenPop", "(I)I");
		if (methodID == 0)
		{
			return 0;
		}
		
		// Call the java method
		int result = jNIEnv->CallIntMethod(jThis, methodID, stackScreenWidget);
		
		// Delete allocated memory
		jNIEnv->DeleteLocalRef(cls);
		
		return result;
	}

	int _maOpenGLInitFullscreen()
	{
		return IOCTL_UNAVAILABLE; 
	}
	
	int _maOpenGLCloseFullscreen()
	{
		return IOCTL_UNAVAILABLE;
	}
	
	int _maOpenGLTexImage2D(MAHandle image, JNIEnv* jNIEnv, jobject jThis)
	{		
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "loadGlTexture", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, image);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	int _maOpenGLTexSubImage2D(MAHandle image, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "loadGlSubTexture", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		jint result = jNIEnv->CallIntMethod(jThis, methodID, image);
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	
	
	
	MAHandle _maFileOpen(
		const char* path,
		int mode,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jstring jstrPath = jNIEnv->NewStringUTF(path);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileOpen", 
												 "(Ljava/lang/String;I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, jstrPath, mode);
		
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrPath);
		
		return (int)result;
	}

	int _maFileExists(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileExists", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileClose(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileClose", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileCreate(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileCreate", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileDelete(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileDelete", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileSize(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileSize", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileAvailableSpace(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileAvailableSpace", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileTotalSpace(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileTotalSpace", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileDate(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileDate", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileRename(
		MAHandle file,
		const char* newName,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jstring jstrNewName = jNIEnv->NewStringUTF(newName);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileRename", 
												 "(ILjava/lang/String;)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, 
											file, jstrNewName);
		
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrNewName);
		
		return (int)result;
	}

	int _maFileTruncate(
		MAHandle file,
		int offset,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileTruncate", 
												 "(II)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file, offset);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileWrite(
		MAHandle file,
		int src,
		int len,
		int memStart,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		int fixedSrc = src - memStart;
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileWrite", 
												 "(III)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file,
											fixedSrc, len);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	int _maFileWriteFromData(
		MAHandle file,
		MAHandle data,
		int offset,
		int len,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileWriteFromData", 
												 "(IIII)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file,
											data, offset, len);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	int _maFileRead(
		MAHandle file,
		int dst,
		int len,
		int memStart,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		int fixedDst = dst - memStart;
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileRead", 
												 "(III)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file, fixedDst, len);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	int _maFileReadToData(
		MAHandle file,
		MAHandle data,
		int offset,
		int len,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileReadToData", 
												 "(IIII)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file,
											data, offset, len);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileTell(MAHandle file, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileTell", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, file);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileSeek(
		MAHandle file,
		int offset,
		int whence,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileSeek", 
												 "(III)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID,
											file, offset, whence);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}
	
	MAHandle _maFileListStart(
		const char* path,
		const char* filter,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jstring jstrPath = jNIEnv->NewStringUTF(path);
		jstring jstrFilter = jNIEnv->NewStringUTF(filter);
		
		jmethodID methodID = jNIEnv->GetMethodID(
								cls, 
								"maFileListStart", 
								"(Ljava/lang/String;Ljava/lang/String;)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID,
											jstrPath, jstrFilter);
		
		jNIEnv->DeleteLocalRef(cls);
		jNIEnv->DeleteLocalRef(jstrPath);
		jNIEnv->DeleteLocalRef(jstrFilter);
		
		return (int)result;
	}

	int _maFileListNext(
		MAHandle list,
		int nameBuf,
		int bufSize,
		int memStart,
		JNIEnv* jNIEnv,
		jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		int fixedNameBuf = nameBuf - memStart;
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileListNext", 
												 "(III)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID,
											list, nameBuf, bufSize);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

	int _maFileListClose(MAHandle list, JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		
		jmethodID methodID = jNIEnv->GetMethodID(
												 cls, 
												 "maFileListClose", 
												 "(I)I");
		if (methodID == 0)
			return 0;
		
		jint result = jNIEnv->CallIntMethod(jThis, methodID, list);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)result;
	}

}

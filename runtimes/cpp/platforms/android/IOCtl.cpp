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
 * Custom conversion function from Wide Char String to Multi Byte String.
 * Exists because the android NDK does not support wchars.
 *
 * @param s		output multi-byte string
 * @param pwcs	input wide char string
 * @param n		length of the input wide char string
 *
 * @return		returns the number of bytes converted 
 *				(not including any terminating null), 
 *				if successful; otherwise, it returns (size_t)-1.
 */
inline size_t wideCharString2multiByteString(char *s, const wchar *pwcs, size_t n)
{
	if(s == NULL)
	{
		int num_bytes = 0;
		while(*pwcs != 0)
		{
			num_bytes++;
			pwcs++;
		}
		return num_bytes;
	}
	int count = 0;
	
	if (n != 0) {
		do {
			if ((*s++ = (char) *pwcs++) == 0)
				break;
			count++;
		} while (--n != 0);
	}
	return count;
}

/**
 * Custom Wide Char String length calculation function.
 * Exists because the android NDK does not support wchars.
 *
 * @param s		input multi-byte string
 *
 * @return		length of the input string
 */
inline size_t wideCharStringLength(const wchar * s)
{
	const wchar *save;
	if (s == 0)
		return 0;
	for (save = s; *save; ++save);
	return save-s;
}


/**
 * Converts a Wide Char String to a Java String.
 *
 * @param env	JNI Environment used
 * @param s		Input Wide Char String
 *
 * @return		Java String
 */
inline jobject wchar2jstring(JNIEnv* env, const wchar* s)
{
    jobject result = 0;
    size_t len = wideCharStringLength(s);
    size_t sz = wideCharString2multiByteString (0, s, len);
	char* c = (char*)malloc(sizeof(char)*(sz+1));
    wideCharString2multiByteString (c, s, len);
    c[sz] = '\0';
    result = env->NewStringUTF(c);
	free(c);
    return result;
}

namespace Base
{
	int _maFrameBufferGetInfo(MAFrameBufferInfo *info)
	{	
		int size = maGetScrSize();
		int width = (size&0xffff0000) >> 16;
		int height = size&0x0000ffff;
	
		char* b = (char*)malloc(200);
		sprintf(b,"Framebuffer width: %i height: %i", width, height);
		__android_log_write(ANDROID_LOG_INFO,"JNI",b);
		free(b);
	
		info->bitsPerPixel = 32;//backBuffer->bitsPerPixel;
		info->bytesPerPixel = 4;//backBuffer->bytesPerPixel;
		info->redMask = 0xff0000ff; //backBuffer->redMask;
		info->greenMask = 0x0000ff00;// backBuffer->greenMask;
		info->blueMask = 0x00ff0000; //backBuffer->blueMask;
		info->sizeInBytes = width * height * 4; //backBuffer->pitch*backBuffer->height;
		info->width = width; //backBuffer->width;
		info->height = height; // backBuffer->height;
		info->pitch = width*4; //backBuffer->pitch;
		info->redShift = 0; //backBuffer->redShift;
		info->greenShift = 8; //backBuffer->greenShift;
		info->blueShift = 16; //backBuffer->blueShift;
		info->redBits = 8;// backBuffer->redBits;
		info->greenBits = 8;// backBuffer->greenBits;
		info->blueBits = 8; //backBuffer->blueBits;
		info->supportsGfxSyscalls = 0;

		return 1;
	}


	int _maFrameBufferInit(void *data, int memStart, JNIEnv* jNIEnv, jobject jThis)
	{
	/*
		if(sInternalBackBuffer!=NULL) return 0;
		sInternalBackBuffer = backBuffer;
		backBuffer = new Image((unsigned char*)data, NULL, backBuffer->width, backBuffer->height, backBuffer->pitch, backBuffer->pixelFormat, false, false);
		currentDrawSurface = backBuffer;
		return 1;
	*/
	
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
	/*
		if(sInternalBackBuffer==NULL) return 0;
		delete backBuffer;
		backBuffer = sInternalBackBuffer;
		sInternalBackBuffer = NULL;
		currentDrawSurface = backBuffer;
		return 1;
	*/
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
	
	int _maBtStartServiceDiscovery(JNIEnv* jNIEnv, jobject jThis)
	{
		return -1;
	}
	
	int _maBtGetNewService(MABtService* dst, JNIEnv* jNIEnv, jobject jThis)
	{
		return -1;
	}
	
	int _maBtGetNextServiceSize(JNIEnv* jNIEnv, jobject jThis)
	{
		return -1;
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
	int _maTextBox(const wchar* title, const wchar* inText, int outText,
				   int maxSize,  int constraints, int memStart, JNIEnv* jNIEnv, jobject jThis)
	{
		// Initialization
		jstring jstrTITLE = (jstring)wchar2jstring(jNIEnv,  title);
		jstring jstrINTEXT = (jstring)wchar2jstring(jNIEnv,  inText);
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
}

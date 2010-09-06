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

#include "libtomcrypt/src/headers/tomcrypt.h"

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
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtStartDeviceDiscovery enter");
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maBtStartDeviceDiscovery", "(I)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID);
		
		jNIEnv->DeleteLocalRef(cls);
		
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "_maBtStartDeviceDiscovery leaving");
		
		return (int)ret;
	}
	
	int _maBtGetNewDevice(MABtDevice* dst, JNIEnv* jNIEnv, jobject jThis)
	{
		return -1;
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
	
	int _maLocationStart(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maLocationStart", "()I");
		if (methodID == 0) return 0;
		int retval = jNIEnv->CallIntMethod(jThis, methodID);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}
	
	int _maLocationStop(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maLocationStop", "()I");
		if (methodID == 0) return 0;
		int retval = jNIEnv->CallIntMethod(jThis, methodID);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return retval;
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
	
	int _maShowVirtualKeyboard(JNIEnv* jNIEnv, jobject jThis)
	{
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maShowVirtualKeyboard", "()I");
		if (methodID == 0) return 0;
		int retval = jNIEnv->CallIntMethod(jThis, methodID);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}
	int _maSecureRandSeed(int seedData, int seedLength, int memStart, JNIEnv* jNIEnv, jobject jThis)
	{
		int rData = seedData - memStart;
	
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maSecureRandSeed", "(II)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, rData, seedLength);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)ret;
	}
	

	
	int _maSecureRandBytes(int randData, int randLength, int memStart, JNIEnv* jNIEnv, jobject jThis)
	{	
		int rData = randData - memStart;
	
		jclass cls = jNIEnv->GetObjectClass(jThis);
		jmethodID methodID = jNIEnv->GetMethodID(cls, "maSecureRandBytes", "(II)I");
		if (methodID == 0) return 0;
		jint ret = jNIEnv->CallIntMethod(jThis, methodID, rData, randLength);
		
		jNIEnv->DeleteLocalRef(cls);
		
		return (int)ret;
	}
	
	unsigned char* rsaData = NULL;
	int rsaDataSize;
	unsigned char* rsaExponent = NULL;
	int rsaExponentSize;
	unsigned char* rsaModulus = NULL;
	int rsaModulusSize;
	
	int _maLoadRSAData(int data, int length, JNIEnv* jNIEnv, jobject jThis)
	{		
		rsaData = (unsigned char*)data;
		rsaDataSize = length;
	}
	
	int _maLoadRSAExponent(int exponent, int length, JNIEnv* jNIEnv, jobject jThis)
	{	
		rsaExponent = (unsigned char*)exponent;
		rsaExponentSize = length;
	}
	
	int _maLoadRSAModulus(int modulus, int length, JNIEnv* jNIEnv, jobject jThis)
	{
		rsaModulus = (unsigned char*)modulus;
		rsaModulusSize = length;
	}

	int _maGetRSA(int data, int length, JNIEnv* jNIEnv, jobject jThis)
	{
			
		unsigned char* result = (unsigned char*)data;
		unsigned long resultSize;
		rsa_key rsaKey;
		
		ltc_mp = ltm_desc;
		rsaKey.type = PK_PUBLIC;
		ltc_mp.init(&rsaKey.e);
		ltc_mp.init(&rsaKey.N);
		ltc_mp.unsigned_read(rsaKey.e, rsaExponent, rsaExponentSize);
		ltc_mp.unsigned_read(rsaKey.N, rsaModulus, rsaModulusSize);
		resultSize = length;

		ltc_mp.rsa_me(rsaData, rsaDataSize, result, &resultSize, rsaKey.type, &rsaKey);

		ltc_mp.deinit(rsaKey.e);
		ltc_mp.deinit(rsaKey.N);

	}
}

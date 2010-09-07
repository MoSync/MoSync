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

#include "config_platform.h"
#include "Syscall.h"
#include "Core.h"

#include "IOCtl.h"

#include <helpers/cpp_defs.h>
#include <helpers/fifo.h>

#include <jni.h>

#define ERROR_EXIT { MoSyncErrorExit(-1); }

//#define SYSLOG(a) __android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", a);
#define SYSLOG(...)

namespace Base
{
	Syscall* gSyscall;

	JNIEnv* mJNIEnv;
	jobject mJThis;
	
	static ResourceArray gResourceArray;
	static CircularFifo<MAEvent, EVENT_BUFFER_SIZE> gEventFifo;
	
	int mClipLeft, mClipTop, mClipWidth, mClipHeight;
	MAHandle drawTargetHandle = HANDLE_SCREEN;
	
	/**
	* Syscall constructor	
	*/
	Syscall::Syscall()
	{
		gSyscall = this;
//		mIsLooked = false;
//		mGotLockedEvent = false;
	}

	/**
	* Returns the JNI Environment variable.
	* 
	* @ return The JNI Environment
	*/
	JNIEnv* Syscall::getJNIEnvironment()
	{
		return mJNIEnv;
	}
	
	/**
	* Returns a reference to the JNI Object which initiated the execution of this native code
	*
	* @return A reference the JNI  Object
	*/
	jobject Syscall::getJNIThis()
	{
		return mJThis;
	}
	
	/**
	* 
	* 
	*/
	bool Syscall::loadImage(int resourceIndex, int pos, int length)
	{
		SYSLOG("loadImage");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "loadImage", "(IIII)Z");
		if (methodID == 0) ERROR_EXIT;
		bool retVal = mJNIEnv->CallBooleanMethod(mJThis, methodID, resourceIndex, pos, length, 0);
				
		mJNIEnv->DeleteLocalRef(cls);
		return retVal;
	}
	
	Image* Syscall::loadSprite(void* surface, ushort left, ushort top, ushort width, ushort height, ushort cx, ushort cy)
	{
		SYSLOG("loadSprite - NOT IMPLEMENTED");
		
		return NULL;
	}
	
	char* Syscall::loadBinary(int resourceIndex, int size)
	{
		SYSLOG("loadBinary");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "loadBinary", "(II)Ljava/nio/ByteBuffer;");
		if (methodID == 0) return NULL;
		jobject jo = mJNIEnv->CallObjectMethod(mJThis, methodID, resourceIndex, size);
		char* buffer = (char*)mJNIEnv->GetDirectBufferAddress(jo);

		mJNIEnv->DeleteLocalRef(cls);		
		return buffer;
	}

	void Syscall::loadUBinary(int resourceIndex, int offset, int size)
	{
		SYSLOG("loadUBinary");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "loadUBinary", "(III)V");
		if (methodID == 0) return;
		mJNIEnv->CallVoidMethod(mJThis, methodID, resourceIndex, offset, size);
		
		mJNIEnv->DeleteLocalRef(cls);
	}
	
	void Syscall::destroyResource(int resourceIndex)
	{
		SYSLOG("destroyResource");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "destroyResource", "(I)V");
		if (methodID == 0) return;
		mJNIEnv->CallVoidMethod(mJThis, methodID, resourceIndex);
		
		mJNIEnv->DeleteLocalRef(cls);
	}
	
	/**
	* Calls the Java fucntion 'stroeIfBinaryAudioresource'.
	* If this resource is an audio resource, with a correct mime header,
	* this file will be saved to the memoory.
	* This is because Android can only play commpressed audio formats 
	* with a file descriptor and not from a buffer or array in memory.
	*
	* @param resourceIndex		The resource index of the resource which shall be checked.
	*
	*/
	void Syscall::checkAndStoreAudioResource(int resourceIndex)
	{
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "storeIfBinaryAudioResource", "(I)V");
		if (methodID != 0)
			mJNIEnv->CallVoidMethod(mJThis, methodID, resourceIndex);
				
		mJNIEnv->DeleteLocalRef(cls);
	}
	
	void Syscall::platformDestruct()
	{
	}

	void Syscall::setJNIEnvironment(JNIEnv* je, jobject jthis)
	{
		SYSLOG("setJNIEnvironment")
		
		mJNIEnv = je;
		mJThis = jthis;
	}
	
	void Syscall::postEvent(MAEvent event)
	{
		SYSLOG("PostEvent");
		gEventFifo.put(event);
	}
	
	SYSCALL(int,  maSetColor(int rgb))
	{
		SYSLOG("maSetColor");
	
		if(rgb<=0xffffff) rgb += 0xff000000;
	
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSetColor", "(I)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, rgb);
		
		mJNIEnv->DeleteLocalRef(cls);
		return retval;
	}

	SYSCALL(void,  maSetClipRect(int left, int top, int width, int height))
	{
		SYSLOG("maSetClipRect");
		
		mClipLeft = left;
		mClipTop = top;
		mClipWidth = width;
		mClipHeight = height;
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSetClipRect", "(IIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, left, top, width, height);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maGetClipRect(MARect* rect))
	{
		SYSLOG("maGetClipRect");
		
		gSyscall->ValidateMemRange(rect, sizeof(MARect));		
		rect->left = mClipLeft;
		rect->top = mClipTop;
		rect->width = mClipWidth;
		rect->height = mClipHeight;
	}

	SYSCALL(void,  maPlot(int posX, int posY))
	{
		SYSLOG("maPlot");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maPlot", "(II)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, posX, posY);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maLine(int startX, int startY, int endX, int endY))
	{
		SYSLOG("maLine");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maLine", "(IIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, startX, startY, endX, endY);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maFillRect(int left, int top, int width, int height))
	{
		SYSLOG("maFillRect");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maFillRect", "(IIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, left, top, width, height);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maFillTriangleStrip(const MAPoint2d* points, int count))
	{
		SYSLOG("maFillTriangleStrip NOT IMPLEMENTED");

	}

	SYSCALL(void,  maFillTriangleFan(const MAPoint2d* points, int count))
	{
		SYSLOG("maFillTriangleFan NOT IMPLEMENTED");

	}

	SYSCALL(MAExtent,  maGetTextSize(const char* str))
	{
		//SYSLOG("maGetTextSize");
		
		jstring jstr = mJNIEnv->NewStringUTF(str);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maGetTextSize", "(Ljava/lang/String;)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, jstr);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
	
		return retval;
	}
	
	int wcharLength(const wchar* str)
	{
		int l = 0;
		while(str[l] != 0) l++;
		return l;
	}

	SYSCALL(MAExtent,  maGetTextSizeW(const wchar* str))
	{
		//SYSLOG("maGetTextSizeW");
		
		jsize len = wcharLength(str);
		jstring jstr = mJNIEnv->NewString((jchar*)str, len);

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maGetTextSizeW", "(Ljava/lang/String;)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, jstr);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
		
		return retval;
	}

	SYSCALL(void,  maDrawText(int left, int top, const char* str))
	{
		//SYSLOG("maDrawText");
		
		jstring jstr = mJNIEnv->NewStringUTF(str);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maDrawText", "(IILjava/lang/String;)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, left, top, jstr);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
	}

	SYSCALL(void,  maDrawTextW(int left, int top, const wchar* str))
	{
		//SYSLOG("maDrawTextW");
		
		jsize len = wcharLength(str);
		jstring jstr = mJNIEnv->NewString((jchar*)str, len);

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maDrawTextW", "(IILjava/lang/String;)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, left, top, jstr);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
	}

	SYSCALL(void,  maUpdateScreen(void))
	{
		SYSLOG("maUpdateScreen");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);		
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maUpdateScreen", "()V");	
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID);

		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maResetBacklight(void))
	{
		SYSLOG("maResetBacklight NOT IMPLEMENTED");
	}

	SYSCALL(MAExtent,  maGetScrSize(void))
	{
		SYSLOG("maGetScrSize");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maGetScrSize", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(void,  maDrawImage(MAHandle image, int left, int top))
	{
		SYSLOG("maDrawImage");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maDrawImage", "(III)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, image, left, top);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect, int scanLength))
	{
		SYSLOG("maDrawRGB");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maDrawRGB", "(IIIIIIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, dstPoint->x, dstPoint->y, src, srcRect->left, srcRect->top, srcRect->width, srcRect->height, scanLength);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maDrawImageRegion(MAHandle image, const MARect* srcRect, const MAPoint2d* dstPoint, int transformMode))
	{
		SYSLOG("maDrawImageRegion");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maDrawImageRegion", "(IIIIIIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, image, srcRect->left, srcRect->top, srcRect->width, srcRect->height, dstPoint->x, dstPoint->y, transformMode);
		
		mJNIEnv->DeleteLocalRef(cls);
		
	}

	SYSCALL(MAExtent,  maGetImageSize(MAHandle image))
	{
		SYSLOG("maGetImageSize");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maGetImageSize", "(I)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, image);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(void,  maGetImageData(MAHandle image, void* dst, const MARect* srcRect, int scanlength))
	{
		SYSLOG("maGetImageData");

		if (srcRect->width > scanlength) maPanic(ERR_IMAGE_OOB, "maGetImageData, scanlenght < width");
		
		int rdst = (int)dst - (int)gCore->mem_ds;
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maGetImageData", "(IIIIIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, image, rdst, srcRect->left, srcRect->top, srcRect->width, srcRect->height, scanlength);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(MAHandle,  maSetDrawTarget(MAHandle image))
	{
		SYSLOG("maSetDrawTarget");
		
		MAHandle temp = drawTargetHandle;
		int currentDrawSurface;
		
		if(drawTargetHandle != HANDLE_SCREEN)
		{
			SYSCALL_THIS->resources.extract_RT_FLUX(drawTargetHandle);
			if(SYSCALL_THIS->resources.add_RT_IMAGE(drawTargetHandle, NULL) == RES_OUT_OF_MEMORY)
			{
				maPanic(ERR_RES_OOM, "maSetDrawTarget couldn't allocate drawtarget");
			}
			drawTargetHandle = HANDLE_SCREEN;
		}
		
		if(image == HANDLE_SCREEN)
		{
			currentDrawSurface = HANDLE_SCREEN;
		} 
		else
		{
			currentDrawSurface = image;
			SYSCALL_THIS->resources.extract_RT_IMAGE(image);
			if(SYSCALL_THIS->resources.add_RT_FLUX(image, NULL) == RES_OUT_OF_MEMORY)
			{
				maPanic(ERR_RES_OOM, "maSetDrawTarget couldn't allocate drawtarget");
			}
		}
		drawTargetHandle = image;
			
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSetDrawTarget", "(I)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, currentDrawSurface);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return temp;
	}

	SYSCALL(int,  maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size))
	{
		SYSLOG("maCreateImageFromData");
		
		if(SYSCALL_THIS->resources.add_RT_IMAGE(placeholder, NULL) == RES_OUT_OF_MEMORY) return RES_OUT_OF_MEMORY;
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maCreateImageFromData", "(IIII)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, placeholder, data, offset, size);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(int,  maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int alpha))
	{
		SYSLOG("maCreateImageRaw");
		
		int imgHeight = size&0xffff;
		int imgWidth = (size>>16)&0xffff;
		
		int imgSize = imgWidth * imgHeight * 4;
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maCreateImageRawGetData", "(I)Ljava/nio/ByteBuffer;");
		if (methodID == 0) return -1;
		jobject jo = mJNIEnv->CallObjectMethod(mJThis, methodID, imgSize);
		char* img = (char*)mJNIEnv->GetDirectBufferAddress(jo);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		if(1==alpha)
		{
			char* srcImg = (char*)src;
			int j = 0;
			for(int i = 0 ; i < imgSize/4; i++)
			{
				(*(img+j)) = (*(srcImg+j));j++;
				(*(img+j)) = (*(srcImg+j));j++;
				(*(img+j)) = (*(srcImg+j));j++;
				//(*(img+j)) = 255;j++;
			}
		}
		else
		{
			memcpy(img, src, imgSize);
		}
		
		cls = mJNIEnv->GetObjectClass(mJThis);
		methodID = mJNIEnv->GetMethodID(cls, "_maCreateImageRaw", "(III)I");
		if (methodID == 0) ERROR_EXIT;
		int retVal = mJNIEnv->CallIntMethod(mJThis, methodID, placeholder, imgWidth, imgHeight);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		SYSCALL_THIS->resources.add_RT_IMAGE(placeholder, NULL);
		return retVal;
	}

	SYSCALL(int,  maCreateDrawableImage(MAHandle placeholder, int width, int height))
	{
		SYSLOG("maCreateDrawableImage");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maCreateDrawableImage", "(III)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, placeholder, width, height);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		if(RES_OK == retval)
			return SYSCALL_THIS->resources.add_RT_IMAGE(placeholder, NULL);
		
		return retval;
	}

	SYSCALL(MAHandle,  maOpenStore(const char* name, int flags))
	{
		SYSLOG("maOpenStore");
		
		jstring jname = mJNIEnv->NewStringUTF(name);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maOpenStore", "(Ljava/lang/String;I)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, jname, flags);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jname);
		
		return retval;
	}

	SYSCALL(int,  maWriteStore(MAHandle store, MAHandle data))
	{
		SYSLOG("maWriteStore");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maWriteStore", "(II)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, store, data);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(int,  maReadStore(MAHandle store, MAHandle placeholder))
	{
		SYSLOG("maReadStore");

/*		
		char* b = loadBinary(rI, size);
		MemStream* ms = new MemStream(b, size);
		ROOM(resources.dadd_RT_BINARY(rI, ms));

		if(SYSCALL_THIS->resources.add_RT_BINARY(placeholder, NULL) == RES_OUT_OF_MEMORY) return RES_OUT_OF_MEMORY;
		placeholder = placeholder&(~DYNAMIC_PLACEHOLDER_BIT);
	*/	
			
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maReadStore", "(II)Ljava/nio/ByteBuffer;");
		if (methodID == 0) ERROR_EXIT;
		jobject ob = mJNIEnv->CallObjectMethod(mJThis, methodID, store, placeholder);
		char* buffer = (char*)mJNIEnv->GetDirectBufferAddress(ob);
		MemStream* ms = new MemStream(buffer, (int)mJNIEnv->GetDirectBufferCapacity(ob));

		if(SYSCALL_THIS->resources.add_RT_BINARY(placeholder, ms) == RES_OUT_OF_MEMORY) return RES_OUT_OF_MEMORY;

		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(ob);
		
		return RES_OK;
	}

	SYSCALL(void,  maCloseStore(MAHandle store, int remove))
	{
		SYSLOG("maCloseStore");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maCloseStore", "(II)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, store, remove);
		
		mJNIEnv->DeleteLocalRef(cls);
	}
	
	SYSCALL(MAHandle,  maConnect(const char* url))
	{
		SYSLOG("maConnect");
		
		jstring jstr = mJNIEnv->NewStringUTF(url);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnect", "(Ljava/lang/String;)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, jstr);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
		
		return retval;
	}

	SYSCALL(void,  maConnClose(MAHandle conn))
	{
		SYSLOG("maConnClose");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnClose", "(I)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maConnRead(MAHandle conn, void* dst, int size))
	{
		SYSLOG("maConnRead");
		
		int rdst = (int)dst - (int)gCore->mem_ds;
		
		char* b = (char*)malloc(200);
		sprintf(b,"dst: %i",rdst);
		SYSLOG(b);
		free(b);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnRead", "(III)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn, (jint)rdst, size);
		
		mJNIEnv->DeleteLocalRef(cls);
		
	}

	SYSCALL(void,  maConnWrite(MAHandle conn, const void* src, int size))
	{
		SYSLOG("maConnWrite");
	 
		int rsrc = (int)src - (int)gCore->mem_ds;
	 
		char* b = (char*)malloc(200);
		sprintf(b,"src: %i",rsrc);
		SYSLOG(b);
		free(b);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnWrite", "(III)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn, (jint)rsrc, size);
		
		mJNIEnv->DeleteLocalRef(cls);

	}

	SYSCALL(void,  maConnReadToData(MAHandle conn, MAHandle data, int offset, int size))
	{
		SYSLOG("maConnReadToData");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnReadToData", "(IIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn, data, offset, size);
		
		mJNIEnv->DeleteLocalRef(cls);

	}

	SYSCALL(void,  maConnWriteFromData(MAHandle conn, MAHandle data, int offset, int size))
	{
		SYSLOG("maConnWriteFromData");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnWriteFromData", "(IIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn, data, offset, size);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(int,  maConnGetAddr(MAHandle conn, MAConnAddr* addr))
	{
		SYSLOG("maConnGetAddr");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnGetAddr", "(IJ)V");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, conn, (jlong)addr);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(MAHandle,  maHttpCreate(const char* url, int method))
	{
		SYSLOG("mahttpCreate");
		
		jstring jstr = mJNIEnv->NewStringUTF(url);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maHttpCreate", "(Ljava/lang/String;I)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, jstr, method);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
		
		return retval;
	}

	SYSCALL(void,  maHttpSetRequestHeader(MAHandle conn, const char* key, const char* value))
	{
		SYSLOG("maHttpSetRequestHeader");

		jstring jstrKey = mJNIEnv->NewStringUTF(key);
		jstring jstrValue = mJNIEnv->NewStringUTF(value);
			
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maHttpSetRequestHeader", "(ILjava/lang/String;Ljava/lang/String;)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn, jstrKey, jstrValue);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstrKey);
		mJNIEnv->DeleteLocalRef(jstrValue);
	}

	SYSCALL(int,  maHttpGetResponseHeader(MAHandle conn, const char* key, char* buffer, int bufSize))
	{
		SYSLOG("maHttpGetResponseHeader");
		
		int rbuffer = (int)buffer - (int)gCore->mem_ds;
		
		jstring jstr = mJNIEnv->NewStringUTF(key);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maHttpGetResponseHeader", "(ILjava/lang/String;JI)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, conn, jstr, (jlong)rbuffer, bufSize);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
		
		return retval;
	}

	SYSCALL(void,  maHttpFinish(MAHandle conn))
	{
		SYSLOG("maHttpFinish");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maHttpFinish", "(I)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, conn);
		
		mJNIEnv->DeleteLocalRef(cls);
	}
	
	SYSCALL(void,  maLoadProgram(MAHandle data, int reload))
	{
		SYSLOG("maLoadProgram NOT IMPLEMENTED");
	}

	SYSCALL(int,  maGetKeys(void))
	{
		SYSLOG("maGetKeys NOT IMPLEMENTED");
		return -1;
	}

	// NOT USED? 
	//void* mGetEventData;
	
	// Parameter event points to event object on the MoSync side.
	SYSCALL(int,  maGetEvent(MAEvent* event))
	{
		gSyscall->ValidateMemRange(event, sizeof(MAEvent));	
		MYASSERT(((uint)event & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment
		
		// Exit if event queue is empty.
		if (gEventFifo.count() == 0) return 0;
		
//		SYSLOG("maGetEvent");

		// Copy runtime side event to MoSync side event.
		*event = gEventFifo.get();
		
		// Copy event data to memory on the MoSync side.
		#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(event->type == eventType) { \
			memcpy(Core::GetCustomEventPointer(gCore), event->data, sizeof(dataType)); \
			delete (dataType*) event->data; \
			event->data = (void*) (int(Core::GetCustomEventPointer(gCore)) - int(gCore->mem_ds)); }

		// Macro CUSTOM_EVENTS is defined in runtimes/cpp/base/Syscall.h
		CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);
		
		return 1;
	}

	SYSCALL(void,  maWait(int timeout))
	{
		SYSLOG("maWait");

		if(gEventFifo.count() != 0)
			return;
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maWait", "(I)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, timeout);

		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(int,  maTime(void))
	{
		SYSLOG("maTime");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maTime", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(int,  maLocalTime(void))
	{
		SYSLOG("maLocalTime");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maLocalTime", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(int,  maGetMilliSecondCount(void))
	{
//		SYSLOG("maGetMilliSecondCount");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maGetMilliSecondCount", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(int,  maFreeObjectMemory(void))
	{
		SYSLOG("maFreeObjectMemory NOT IMPLEMENTED");
		return -1;
	}

	SYSCALL(int,  maTotalObjectMemory(void))
	{
		SYSLOG("maTotalObjectMemory NOT IMPLEMENTED");
		return -1;
	}

	SYSCALL(int,  maVibrate(int ms))
	{
		SYSLOG("maVibrate NOT IMPLEMENTED");
		return -1;
	}

	SYSCALL(void, maPanic(int result, const char* message))
	{
		SYSLOG("maPanic");
		
		int yield = Core::GetVMYield(gCore);
		yield = 1;
		
		jstring jstr = mJNIEnv->NewStringUTF(message);
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maPanic", "(ILjava/lang/String;)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, (jint)result, jstr);
		
		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jstr);
	}

	SYSCALL(int,  maSoundPlay(MAHandle soundResource, int offset, int size))
	{
		SYSLOG("maSoundPlay");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSoundPlay", "(III)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, soundResource, offset, size);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(void,  maSoundStop(void))
	{
		SYSLOG("maStopSound");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSoundStop", "()V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(int,  maSoundIsPlaying(void))
	{
		SYSLOG("maSoundIsPlaying");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSoundIsPlaying", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(int,  maSoundGetVolume(void))
	{
		SYSLOG("maSoundGetVolume");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSoundGetVolume", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		mJNIEnv->DeleteLocalRef(cls);
		
		return retval;
	}

	SYSCALL(void,  maSoundSetVolume(int volume))
	{
		SYSLOG("maSoundSetVolume");
		
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSoundSetVolume", "(I)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, volume);
		
		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(int,  maInvokeExtension(int function, int a, int b, int c))
	{
		SYSLOG("maInvokeExtension NOT IMPLEMENTED");
		return -1;
	}

	/**
	* Call one of the platform dependant syscalls. For more information about each of these syscalls,
	* their paramaters and return values, please check the MoSync documentation.
	* 
	* @param function	The number of the syscall, this number is generated by the IDL compiler
	* @param a		The first paramater
	* @param b		The second paramater
	* @param c		The third paramater
	* @return		< 0
	*				if error
	*				> 0
	*				on success
	* 				-1
	*				if this syscall is not implemented on this platfom.
	*
	*/
	SYSCALL(int,  maIOCtl(int function, int a, int b, int c))
	{
		SYSLOG("maIOCtl");
		__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "maIOCtl");
		
		switch(function) {
		
		case maIOCtl_maWriteLog:
			SYSLOG("maIOCtl_maWriteLog");
			return _maWriteLog((const char*)gSyscall->GetValidatedMemRange(a, b), b, mJNIEnv, mJThis);
		
		case maIOCtl_maSendTextSMS:
			SYSLOG("maIOCtl_maSendTextSMS NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maGetBatteryCharge:
			SYSLOG("maIOCtl_maGetBatteryCharge NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maKeypadIsLocked:
			SYSLOG("maIOCtl_maKeypadIsLocked NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maLockKeypad:
			SYSLOG("maIOCtl_maLockKeypad NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maUnlockKeypad:
			SYSLOG("maIOCtl_maUnlockKeypad NOT IMPLEMENTED");
			return -1;

		// Bluetooth syscalls
		
		case maIOCtl_maBtStartDeviceDiscovery:
			__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "maIOCtl_maBtStartDeviceDiscovery");
			SYSLOG("maIOCtl_maBtStartDeviceDiscovery");
			return _maBtStartDeviceDiscovery(a, mJNIEnv, mJThis);
		
		case maIOCtl_maBtGetNewDevice:
		{
			__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "maIOCtl_maBtGetNewDevice");
			SYSLOG("maIOCtl_maBtGetNewDevice");
			
			// a is pointer to struct MABtDevice
			MABtDevice* deviceInfo = (MABtDevice*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MABtDevice));
			int nameBufSize = deviceInfo->nameBufSize;
			char* name = (char*) SYSCALL_THIS->GetValidatedMemRange((int)deviceInfo->name, nameBufSize);
			
			// Here we get the address of deviceInfo->actualNameLength. Structs are packed, 
			// which means that the start is at sizeof(char*) + sizeof(int), which is 8.
			int actualNameLength = ((int)deviceInfo) + 8;
			
			// Here we get the address of the start of MABtAddr.a. Structs are packed, 
			// which means that the start is at sizeof(char*) + sizeof(int) + sizeof(int),
			// which is 12.
			int address = ((int)deviceInfo) + 12;
			
			// Returns 1 for success, 0 for no more devices.
			return _maBtGetNewDevice(
				(int)gCore->mem_ds,
				(int)name,
				nameBufSize,
				actualNameLength,
				address,
				mJNIEnv, 
				mJThis);
		}
		
		case maIOCtl_maBtStartServiceDiscovery:
			SYSLOG("maIOCtl_maBtStartServiceDiscovery NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maBtGetNewService:
			SYSLOG("maIOCtl_maBtGetNewService NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maBtGetNextServiceSize:
			SYSLOG("maIOCtl_maBtGetNextServiceSize NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maBtCancelDiscovery:
			SYSLOG("maIOCtl_maBtCancelDiscovery");
			return _maBtCancelDiscovery(mJNIEnv, mJThis);
		
		// Frame buffer syscalls
		
		case maIOCtl_maFrameBufferGetInfo:
			SYSLOG("maIOCtl_maFrameBufferGetInfo");
			return _maFrameBufferGetInfo(GVMRA(MAFrameBufferInfo));
		
		case maIOCtl_maFrameBufferInit:
			SYSLOG("maIOCtl_maFrameBufferInit");
			return _maFrameBufferInit(GVMRA(void*), (int)gCore->mem_ds, mJNIEnv, mJThis);
		
		case maIOCtl_maFrameBufferClose:
			SYSLOG("maIOCtl_maFrameBufferClose");
			return _maFrameBufferClose(mJNIEnv, mJThis);

/*
		case maIOCtl_maAudioBufferInit:
			SYSLOG("maIOCtl_maAudioBufferInit NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maAudioBufferReady:
			SYSLOG("maIOCtl_maAudioBufferReady NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maAudioBufferClose:
			SYSLOG("maIOCtl_maAudioBufferClose NOT IMPLEMENTED");
			return -1;
*/
		// Location syscalls
		
		case maIOCtl_maLocationStart:
			SYSLOG("maIOCtl_maLocationStart");
			return _maLocationStart(mJNIEnv, mJThis);
			
		case maIOCtl_maLocationStop:
			SYSLOG("maIOCtl_maLocationStop");
			return _maLocationStop(mJNIEnv, mJThis);

		// File syscalls
		
		case maIOCtl_maFileOpen:
			SYSLOG("maIOCtl_maFileOpen NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maFileExists:
			SYSLOG("maIOCtl_maFileExists NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maFileClose:
			SYSLOG("maIOCtl_maFileClose NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maFileCreate:
			SYSLOG("maIOCtl_maFileCreate NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileDelete:
			SYSLOG("maIOCtl_maFileDelete NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileSize:
			SYSLOG("maIOCtl_maFileSize NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileAvailableSpace:
			SYSLOG("maIOCtl_maFileAvailableSpace NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileTotalSpace:
			SYSLOG("maIOCtl_maFileTotalSpace NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileDate:
			SYSLOG("maIOCtl_maFileDate NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileRename:
			SYSLOG("maIOCtl_maFileRename NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileTruncate:
			SYSLOG("maIOCtl_maFileTruncate NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maFileWrite:
			SYSLOG("maIOCtl_maFileWrite NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileWriteFromData:
			SYSLOG("maIOCtl_maFileWriteFromData NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileRead:
			SYSLOG("maIOCtl_maFileRead NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileReadToData:
			SYSLOG("maIOCtl_maFileReadToData NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maFileTell:
			SYSLOG("maIOCtl_maFileTell NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileSeek:
			SYSLOG("maIOCtl_maFileSeek NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maFileListStart:
			SYSLOG("maIOCtl_maFileListStart NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileListNext:
			SYSLOG("maIOCtl_maFileListNext NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maFileListClose:
			SYSLOG("maIOCtl_maFileListClose NOT IMPLEMENTED");
			return -1;
/*
		case maIOCtl_maStartVideoStream:
			SYSLOG("maIOCtl_maStartVideoStream NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maPauseStream:
			SYSLOG("maIOCtl_maPauseStream NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maResumeStream:
			SYSLOG("maIOCtl_maResumeStream NOT IMPLEMENTED");
			return -1;
		
		case maIOCtl_maCloseStream:
			SYSLOG("maIOCtl_maCloseStream NOT IMPLEMENTED");
			return -1;
*/		

		// Other syscalls
		
		case maIOCtl_maGetSystemProperty:		
			SYSLOG("maIOCtl_maGetSystemProperty");
			return _maGetSystemProperty(SYSCALL_THIS->GetValidatedStr(a), (int)SYSCALL_THIS->GetValidatedMemRange(b, c),  (int)gCore->mem_ds, c, mJNIEnv, mJThis);
			
		case maIOCtl_maPlatformRequest:
			SYSLOG("maIOCtl_maPlatformRequest");
			return _maPlatformRequest(SYSCALL_THIS->GetValidatedStr(a), mJNIEnv, mJThis);

		case maIOCtl_maShowVirtualKeyboard:
			SYSLOG("maIOCtl_maShowVirtualKeyboard");
			return _maShowVirtualKeyboard(mJNIEnv, mJThis);
			
		}
		
		return IOCTL_UNAVAILABLE;
	}
}

void MoSyncExit(int errorCode)
{
	__android_log_write(ANDROID_LOG_INFO, "MoSyncExit!", "Program has exited!");

	exit(errorCode);
}

void MoSyncErrorExit(int errorCode)
{
	char* b = (char*)malloc(200);
	sprintf(b, "MoSync error: %i", errorCode);

	__android_log_write(ANDROID_LOG_INFO, "MoSyncErrorExit!", b);


	jstring jstr = Base::mJNIEnv->NewStringUTF(b);
	
	free(b);
	
	jclass cls = Base::mJNIEnv->GetObjectClass(Base::mJThis);
	jmethodID methodID = Base::mJNIEnv->GetMethodID(cls, "threadPanic", "(ILjava/lang/String;)V");
	if (methodID == 0) ERROR_EXIT;
	Base::mJNIEnv->CallVoidMethod(Base::mJThis, methodID, (jint)errorCode, jstr);
	
	Base::mJNIEnv->DeleteLocalRef(cls);
	Base::mJNIEnv->DeleteLocalRef(jstr);

	exit(errorCode);
}

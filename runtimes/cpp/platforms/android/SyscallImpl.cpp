/* Copyright (C) 2010 MoSync AB
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
#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include "helpers/CPP_IX_AUDIOBUFFER.h"
#include "helpers/CPP_IX_OPENGL_ES.h"
#include "helpers/CPP_IX_GL1.h"
#include "helpers/CPP_IX_GL2.h"
//#include "helpers/CPP_IX_GL_OES_FRAMEBUFFER_OBJECT.h"
#include "helpers/CPP_IX_PIM.h"

#define ERROR_EXIT { MoSyncErrorExit(-1); }

//#define SYSLOG(a) __android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", a);
#define SYSLOG(...)

namespace Base
{
	Syscall* gSyscall;

	JNIEnv* mJNIEnv = 0;
	jobject mJThis;

	int mReloadHandle = 0;
	bool mIsReloading = false;

	static ResourceArray gResourceArray;
	static CircularFifo<MAEvent, EVENT_BUFFER_SIZE> gEventFifo;

	int gClipLeft = 0;
	int gClipTop = 0;
	int gClipWidth = 0;
	int gClipHeight = 0;

	/**
	 * Flag to keep track of whether the initial clip rect has
	 * been set of not. Used in maGetClipRect and maSetClipRect.
	 */
	int gClipRectIsSet = 0;

	MAHandle gDrawTargetHandle = HANDLE_SCREEN;

	/**
	* Syscall constructor
	*
	* Sets the global syscall to use this newly consturcted object.
	* If the event queue isn't empty, we will clear it since this is
	* only called when an application has restarted
	*/
	Syscall::Syscall()
	{
		gSyscall = this;
		// mIsLooked = false;
		// mGotLockedEvent = false;
		init();

		// Make sure the event queue is empty.
		gEventFifo.clear();
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
	* @return A reference the JNI  Objec
	*/
	jobject Syscall::getJNIThis()
	{
		return mJThis;
	}

	/**
	*
	*
	*/
	bool Syscall::loadImage(int resourceIndex, int pos, int length, int binaryResourceHandle)
	{
		SYSLOG("loadImage");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "loadImage", "(IIII)Z");
		if (methodID == 0) ERROR_EXIT;
		bool retVal = mJNIEnv->CallBooleanMethod(mJThis, methodID, resourceIndex, pos, length, binaryResourceHandle);

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

		char* b = (char*)malloc(200);
		sprintf(b, "loadBinary index:%d size:%d", resourceIndex, size);
		//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", b);
		free(b);

		char* buffer = (char*)malloc(size);
		jobject byteBuffer = mJNIEnv->NewDirectByteBuffer((void*)buffer, size);

		if(byteBuffer == NULL) return NULL;

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "loadBinary", "(ILjava/nio/ByteBuffer;)Z");
		if (methodID == 0) return NULL;

		jboolean ret = mJNIEnv->CallBooleanMethod(mJThis, methodID, resourceIndex, byteBuffer);

		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(byteBuffer);

		if(ret == false)
		{
			free(buffer);
			return NULL;
		}
		return buffer;
	}

	int Syscall::loadBinaryStore(int resourceIndex, int size)
	{
		char* b = (char*)malloc(200);
		sprintf(b, "loadBinaryStore index:%d size:%d", resourceIndex, size);
		//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", b);
		free(b);
		return maCreateData(resourceIndex, size);
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

	bool Syscall::destroyBinaryResource(int resourceIndex)
	{
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "destroyBinary", "(I)Ljava/nio/ByteBuffer;");
		if (methodID == 0) return false;

		jobject jo = mJNIEnv->CallObjectMethod(mJThis, methodID, resourceIndex);
		bool destroyed = false;
		if(jo != NULL)
		{
			char* buffer = (char*)mJNIEnv->GetDirectBufferAddress(jo);
			free(buffer);
			destroyed = true;
		}

		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jo);

		return destroyed;
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
	* Calls the Java function 'storeIfBinaryAudioresource'.
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

		// Clip rect is now set.
		gClipRectIsSet = 1;

		gClipLeft = left;
		gClipTop = top;
		gClipWidth = width;
		gClipHeight = height;

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maSetClipRect", "(IIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, left, top, width, height);

		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maGetClipRect(MARect* rect))
	{
		SYSLOG("maGetClipRect");

		// If no clip rect is set, we set it to the screen size.
		if (!gClipRectIsSet)
		{
			MAExtent extent = maGetScrSize();
			gClipLeft = 0;
			gClipTop = 0;
			gClipWidth = EXTENT_X(extent);
			gClipHeight = EXTENT_Y(extent);

			// Clip rect is now set.
			gClipRectIsSet = 1;
		}

		gSyscall->ValidateMemRange(rect, sizeof(MARect));
		rect->left = gClipLeft;
		rect->top = gClipTop;
		rect->width = gClipWidth;
		rect->height = gClipHeight;
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
		SYSLOG("maFillTriangleStrip");

		int heapPoints = (int)points - (int)gCore->mem_ds;

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maFillTriangleStrip", "(II)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, heapPoints, count);

		mJNIEnv->DeleteLocalRef(cls);
	}

	SYSCALL(void,  maFillTriangleFan(const MAPoint2d* points, int count))
	{
		SYSLOG("maFillTriangleFan");

		int heapPoints = (int)points - (int)gCore->mem_ds;

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maFillTriangleFan", "(II)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, heapPoints, count);

		mJNIEnv->DeleteLocalRef(cls);
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
		//SYSLOG("maUpdateScreen");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maUpdateScreen", "()V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID);

		mJNIEnv->DeleteLocalRef(cls);
	}

	/**
	* Reset backlight is not implemented on Android since it has nothing similar
	*/
	SYSCALL(void,  maResetBacklight(void))
	{
		//SYSLOG("maResetBacklight");
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

		int rsrc = (int)src - (int)gCore->mem_ds;

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maDrawRGB", "(IIIIIIII)V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID, dstPoint->x, dstPoint->y, rsrc, srcRect->left, srcRect->top, srcRect->width, srcRect->height, scanLength);

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

		MAHandle temp = gDrawTargetHandle;
		int currentDrawSurface;

		if(gDrawTargetHandle != HANDLE_SCREEN)
		{
			SYSCALL_THIS->resources.extract_RT_FLUX(gDrawTargetHandle);
			if(SYSCALL_THIS->resources.add_RT_IMAGE(
				gDrawTargetHandle, new int[1]) == RES_OUT_OF_MEMORY)
			{
				maPanic(ERR_RES_OOM, "maSetDrawTarget couldn't allocate drawtarget");
			}
			gDrawTargetHandle = HANDLE_SCREEN;
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
		gDrawTargetHandle = image;

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

		if (SYSCALL_THIS->resources.add_RT_IMAGE(
			placeholder, new int[1]) == RES_OUT_OF_MEMORY)
		{
			return RES_OUT_OF_MEMORY;
		}

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

		// Malloc some memory to use when creating the image
		char* img = (char*)malloc(imgSize);
		if(img == NULL) return 0;

		jobject jBuffer = mJNIEnv->NewDirectByteBuffer((void*)img, imgSize);
		if(jBuffer == NULL) return 0;

		if(0==alpha)
		{
			char* srcImg = (char*)src;
			int j = 0;
			for(int i = 0 ; i < imgSize/4; i++)
			{
				(*(img+j)) = (*(srcImg+j+2));j++;
				(*(img+j)) = (*(srcImg+j));j++;
				(*(img+j)) = (*(srcImg+j-2));j++;
				(*(img+j)) = 255;j++;
			}
		}
		else
		{
			char* srcImg = (char*)src;
			int j = 0;
			for(int i = 0 ; i < imgSize/4; i++)
			{
				(*(img+j)) = (*(srcImg+j+2));j++;
				(*(img+j)) = (*(srcImg+j));j++;
				(*(img+j)) = (*(srcImg+j-2));j++;
				(*(img+j)) = (*(srcImg+j));j++;
			}
		}

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maCreateImageRaw", "(IIILjava/nio/ByteBuffer;)I");
		if (methodID == 0) ERROR_EXIT;
		int retVal = mJNIEnv->CallIntMethod(mJThis, methodID, placeholder, imgWidth, imgHeight, jBuffer);

		mJNIEnv->DeleteLocalRef(cls);
		mJNIEnv->DeleteLocalRef(jBuffer);

		free(img);

		SYSCALL_THIS->resources.add_RT_IMAGE(placeholder,  new int[1]);

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
		{
			return SYSCALL_THIS->resources.add_RT_IMAGE(placeholder,  new int[1]);
		}

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

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "_maReadStore", "(II)I");
		if (methodID == 0) ERROR_EXIT;
		jint res = mJNIEnv->CallIntMethod(mJThis, methodID, store, placeholder);

		mJNIEnv->DeleteLocalRef(cls);

		return res;
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

		int addrPointer = (int)addr - (int)gCore->mem_ds;
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maConnGetAddr", "(II)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, conn, addrPointer);
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
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maHttpGetResponseHeader", "(ILjava/lang/String;II)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, conn, jstr, rbuffer, bufSize);

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

	// TODO : Implement maLoadProgram

	int Syscall::getReloadHandle()
	{
		return mReloadHandle;
	}

	int Syscall::setReloadHandle(MAHandle handle)
	{
		mReloadHandle = handle;
	}

	bool Syscall::isReloading()
	{
		return mIsReloading;
	}

	void Syscall::setReloading(bool state)
	{
		mIsReloading = state;
	}

	void Syscall::resetSyscallState()
	{
		// Reset the state
		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "initSyscalls", "()V");
		if (methodID == 0) ERROR_EXIT;
		mJNIEnv->CallVoidMethod(mJThis, methodID);

		mJNIEnv->DeleteLocalRef(cls);

		// Yield the runtime so that it can load the new program
		SYSCALL_THIS->VM_Yield();
	}

	SYSCALL(void,  maLoadProgram(MAHandle data, int reload))
	{
		SYSLOG("maLoadProgram");

		//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "@@@@ MA LOAD PROGRAM");

		mReloadHandle = data;

		if(0 == reload)
			mIsReloading = false;
		else
			mIsReloading = true;

		gSyscall->resetSyscallState();
	}

	// TODO : Implement maGetKeys

	SYSCALL(int,  maGetKeys(void))
	{
		SYSLOG("maGetKeys NOT IMPLEMENTED");
		return -1;
	}

	// Parameter event points to event object on the MoSync side.
	SYSCALL(int,  maGetEvent(MAEvent* event))
	{
		gSyscall->ValidateMemRange(event, sizeof(MAEvent));
		MYASSERT(((uint)event & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

		// Exit if event queue is empty.
		if (gEventFifo.count() == 0) return 0;

		// Copy runtime side event to MoSync side event.
		*event = gEventFifo.get();

		// Copy event data to memory on the MoSync side.
		#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(event->type == eventType) { \
			memcpy(Core::GetCustomEventPointer(gCore), (void*)event->data, sizeof(dataType)); \
			delete (dataType*) event->data; \
			event->data = (int(Core::GetCustomEventPointer(gCore)) - int(gCore->mem_ds)); }

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

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maGetMilliSecondCount", "()I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID);

		mJNIEnv->DeleteLocalRef(cls);

		return retval;
	}

	// TODO: Implement maFreeObjectMemory

	SYSCALL(int,  maFreeObjectMemory(void))
	{
		SYSLOG("maFreeObjectMemory NOT IMPLEMENTED");
		return -1;
	}

	// TODO : Implement maTotalObjectMemory

	SYSCALL(int,  maTotalObjectMemory(void))
	{
		SYSLOG("maTotalObjectMemory NOT IMPLEMENTED");
		return -1;
	}

	// TODO : Implement maVibrate

	SYSCALL(int, maVibrate(int ms))
	{
		SYSLOG("maVibrate");

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "maVibrate", "(I)I");
		if (methodID == 0) ERROR_EXIT;
		int retval = mJNIEnv->CallIntMethod(mJThis, methodID, ms);
		mJNIEnv->DeleteLocalRef(cls);

		return retval;
	}

	SYSCALL(void, maPanic(int result, const char* message))
	{
		SYSLOG("maPanic");

		Base::gSyscall->VM_Yield();

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

	// TODO : Implement maInvokeExtension

	SYSCALL(int,  maInvokeExtension(int function, int a, int b, int c))
	{
		SYSLOG("maInvokeExtension NOT IMPLEMENTED");
		return -1;
	}

	// Temporary kludge to include the implementation of glString,
	// a better solution would be to get a .h generated and
	// then add gl.h.cpp to the list of files.
	#include <generated/gl.h.cpp>

	// Maybe the wrapper generator shouldn't the three functions
	// below, so that we can specify them in the ioctl switch.
	// For now, wrap them here and call functions in ioctl.h.

	/**
	 * Internal wrapper for maOpenGLTexImage2D tha
	 * calls the real implementation _maOpenGLTexImage2D
	 * in ioctl.h.
	 *
	 * @param image The image to load as a texture.
	 */
	int maOpenGLTexImage2D(MAHandle image)
	{
		return _maOpenGLTexImage2D(image, mJNIEnv, mJThis);
	}

	/**
	 * Internal wrapper for maOpenGLTexSubImage2D tha
	 * calls the real implementation _maOpenGLTexSubImage2D
	 * in ioctl.h.
	 *
	 * @param image The image to load as a texture.
	 */
	int maOpenGLTexSubImage2D(MAHandle image)
	{
		return _maOpenGLTexSubImage2D(image, mJNIEnv, mJThis);
	}

	/**
	 * Internal wrapper for maOpenGLInitFullscreen tha
	 * calls the real implementation _maOpenGLInitFullscreen
	 * in ioctl.h.
	 */
	int maOpenGLInitFullscreen(int glApi)
	{
		return _maOpenGLInitFullscreen(glApi, mJNIEnv, mJThis);
	}

	/**
	 * Internal wrapper for maOpenGLCloseFullscreen tha
	 * calls the real implementation _maOpenGLCloseFullscreen
	 * in ioctl.h.
	 */
	int maOpenGLCloseFullscreen()
	{
		return _maOpenGLCloseFullscreen(mJNIEnv, mJThis);
	}

// the wrapper generator can't yet handle a few set of functions
// in the opengles 2.0 api (so we manually override them).
// remove implementations for broken bindings..
// override implementations for broken bindings..
#undef maIOCtl_glGetPointerv_case
#define maIOCtl_glGetPointerv_case(func) \
case maIOCtl_glGetPointerv: \
{\
GLenum _pname = (GLuint)a; \
void* _pointer = GVMR(b, MAAddress);\
wrap_glGetPointerv(_pname, _pointer); \
return 0; \
}

#undef maIOCtl_glGetVertexAttribPointerv_case
#define maIOCtl_glGetVertexAttribPointerv_case(func) \
case maIOCtl_glGetVertexAttribPointerv: \
{\
GLuint _index = (GLuint)a; \
GLenum _pname = (GLuint)b; \
void* _pointer = GVMR(c, MAAddress);\
wrap_glGetVertexAttribPointerv(_index, _pname, _pointer); \
return 0; \
}


#undef maIOCtl_glShaderSource_case
#define maIOCtl_glShaderSource_case(func) \
case maIOCtl_glShaderSource: \
{ \
GLuint _shader = (GLuint)a; \
GLsizei _count = (GLsizei)b; \
void* _string = GVMR(c, MAAddress); \
const GLint* _length = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), GLint); \
wrap_glShaderSource(_shader, _count, _string, _length); \
return 0; \
}

    void wrap_glShaderSource(GLuint shader, GLsizei count, void* strings, const GLint* length) {
        int* stringsArray = (int*)strings;
        const GLchar** strCopies = new const GLchar*[count];

        for(int i = 0; i < count; i++) {
            void* src = GVMR(stringsArray[i], MAAddress);
            strCopies[i] = (GLchar*)src;
        }

        glShaderSource(shader, count, strCopies, length);
        delete strCopies;
    }

    void wrap_glGetVertexAttribPointerv(GLuint index, GLenum pname, void* pointer) {
        GLvoid* outPointer;
        glGetVertexAttribPointerv(index, pname, &outPointer);

        if(pname != GL_VERTEX_ATTRIB_ARRAY_POINTER)
            return;

        *(int*)pointer = gSyscall->TranslateNativePointerToMoSyncPointer(outPointer);
    }

    void wrap_glGetPointerv(GLenum pname, void* pointer) {
        GLvoid* outPointer;
        glGetPointerv(pname, &outPointer);

        if(pname != GL_COLOR_ARRAY_POINTER &&
           pname != GL_NORMAL_ARRAY_POINTER &&
           pname != GL_POINT_SIZE_ARRAY_POINTER_OES &&
           pname != GL_TEXTURE_COORD_ARRAY_POINTER &&
           pname != GL_VERTEX_ARRAY_POINTER)
            return;

        *(int*)pointer = gSyscall->TranslateNativePointerToMoSyncPointer(outPointer);
    }

	/**
	 * Utility function for displaying and catching pending
	 * exceptions.
	 */
	static void handlePendingExceptions(JNIEnv* env)
	{
		jthrowable exc;
		exc = env->ExceptionOccurred();
		if (exc)
		{
			//__android_log_write(
			//					ANDROID_LOG_INFO,
			//					"@@@ MoSync",
			//					"Found pending exception");
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
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
		//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl");
		//handlePendingExceptions(mJNIEnv);

		switch(function)
		{
		maIOCtl_IX_OPENGL_ES_caselist
		maIOCtl_IX_GL1_caselist
		maIOCtl_IX_GL2_caselist
	//	maIOCtl_IX_GL_OES_FRAMEBUFFER_OBJECT_caselist

		case maIOCtl_maWriteLog:
			SYSLOG("maIOCtl_maWriteLog");
			#ifdef LOGGING_ENABLED
				return _maWriteLog((const char*)gSyscall->GetValidatedMemRange(a, b), b, mJNIEnv, mJThis);
			#else
				return -1;
			#endif

		case maIOCtl_maSendTextSMS:
			SYSLOG("maIOCtl_maSendTextSMS");
			return _maSendTextSMS(
				SYSCALL_THIS->GetValidatedStr(a),
				SYSCALL_THIS->GetValidatedStr(b),
				mJNIEnv,
				mJThis);

		// TODO : Implement maGetBatteryCharge
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

		// int maBtStartDeviceDiscovery(int names)
		case maIOCtl_maBtStartDeviceDiscovery:
			//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtStartDeviceDiscovery");
			SYSLOG("maIOCtl_maBtStartDeviceDiscovery");
			return _maBtStartDeviceDiscovery(a, mJNIEnv, mJThis);

		// int maBtGetNewDevice(MABtDevice* d)
		case maIOCtl_maBtGetNewDevice:
		{
			//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtGetNewDevice");
			SYSLOG("maIOCtl_maBtGetNewDevice");

			// a is pointer to struct MABtDevice
			MABtDevice* deviceInfo = (MABtDevice*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MABtDevice));

			// Size of buffer to store device name.
			int nameBufSize = deviceInfo->nameBufSize;

			// Pointer to buffer to store device name.
			int nameBufPointer = (int) SYSCALL_THIS->GetValidatedMemRange((int)deviceInfo->name, nameBufSize);

			// Here we get the address of deviceInfo->actualNameLength. Structs are packed,
			// which means that the start is at sizeof(char*) + sizeof(int), which is 8.
			int actualNameLengthPointer = ((int)deviceInfo) + 8;

			// Here we get the address of the start of MABtAddr.a. Structs are packed,
			// which means that the start is at sizeof(char*) + sizeof(int) + sizeof(int),
			// which is 12.
			int addressPointer = ((int)deviceInfo) + 12;

			// Returns 1 for success, 0 for no more devices.
			return _maBtGetNewDevice(
				(int)gCore->mem_ds,
				nameBufPointer,
				nameBufSize,
				actualNameLengthPointer,
				addressPointer,
				mJNIEnv,
				mJThis);
		}

		// int maBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid)
		case maIOCtl_maBtStartServiceDiscovery:
			SYSLOG("maIOCtl_maBtStartServiceDiscovery NOT IMPLEMENTED");
			//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtStartServiceDiscovery NOT IMPLEMENTED");
			return -1;
		/*
		{
			SYSLOG("maIOCtl_maBtStartServiceDiscovery");
			__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtStartServiceDiscovery");

			// a is pointer to struct MABtAddr
			MABtAddr* addressPointer = (MABtAddr*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MABtAddr));

			// b is pointer to struct MAUUID
			MAUUID* uuidPointer = (MAUUID*) SYSCALL_THIS->GetValidatedMemRange(b, sizeof(MAUUID));

			return _maBtStartServiceDiscovery(
				addressPointer,
				uuidPointer,
				mJNIEnv,
				mJThis);
		}
		*/

		// int maBtGetNextServiceSize(MABtServiceSize* dst)
		case maIOCtl_maBtGetNextServiceSize:
			SYSLOG("maIOCtl_maBtGetNextServiceSize NOT IMPLEMENTED");
			//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtGetNextServiceSize NOT IMPLEMENTED");
			return -1;
		/*
		{
			SYSLOG("maIOCtl_maBtGetNextServiceSize");
			__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtGetNextServiceSize");

			// a is pointer to struct MABtServiceSize
			MABtServiceSize* dstPointer = (MABtServiceSize*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MABtServiceSize));
			int nameBufSizePointer = (int)dstPointer;
			int nUuidsPointer = ((int)dstPointer) + sizeof(int);

			return _maBtGetNextServiceSize(
				(int)gCore->mem_ds,
				nameBufSizePointer,
				nUuidsPointer,
				mJNIEnv,
				mJThis);
		}
		*/

		// int maBtGetNewService(MABtService* dst)
		case maIOCtl_maBtGetNewService:
			SYSLOG("maIOCtl_maBtGetNewService NOT IMPLEMENTED");
			//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtGetNewService NOT IMPLEMENTED");
			return -1;
		/*
		{
			SYSLOG("maIOCtl_maBtGetNewService");
			__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtGetNewService");

			// a is pointer to struct MABtService
			MABtService* serviceInfo = (MABtService*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MABtService));

			// Pointer to int to store port (BT channel) number
			// (this is the first field in the struct).
			int portPointer = (int) serviceInfo;

			// Size of buffer to store service name.
			int nameBufSize = serviceInfo->nameBufSize;

			// Pointer to buffer to store service name.
			int nameBufPointer = (int) SYSCALL_THIS->GetValidatedMemRange((int)serviceInfo->name, nameBufSize);

			// Pointer to buffer to store uuids. We cannot validate this memory
			// since we do not have access to the number of uuids here.
			//int uuidsPointer = SYSCALL_THIS->GetValidatedMemRange((int)serviceInfo->uuids, size??);
			int uuidsPointer = (int) (((char*)gCore->mem_ds) + ((int)serviceInfo->uuids));

			return _maBtGetNewService(
				(int)gCore->mem_ds,
				portPointer,
				nameBufPointer,
				nameBufSize,
				uuidsPointer,
				mJNIEnv,
				mJThis);
		}
		*/

		case maIOCtl_maBtCancelDiscovery:
			SYSLOG("maIOCtl_maBtCancelDiscovery");
			//__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "maIOCtl_maBtCancelDiscovery");
			return _maBtCancelDiscovery(mJNIEnv, mJThis);

		// Server syscalls

		case maIOCtl_maAccept:
			SYSLOG("maIOCtl_maAccept");
			return _maAccept(a, mJNIEnv, mJThis);

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

		// Audio buffer syscalls

		case maIOCtl_maAudioBufferInit:
			SYSLOG("maIOCtl_maAudioBufferInit NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maAudioBufferReady:
			SYSLOG("maIOCtl_maAudioBufferReady NOT IMPLEMENTED");
			return -1;

		case maIOCtl_maAudioBufferClose:
			SYSLOG("maIOCtl_maAudioBufferClose NOT IMPLEMENTED");
			return -1;

		// Location syscalls

		case maIOCtl_maLocationStart:
			SYSLOG("maIOCtl_maLocationStart");
			return _maLocationStart(mJNIEnv, mJThis);

		case maIOCtl_maLocationStop:
			SYSLOG("maIOCtl_maLocationStop");
			return _maLocationStop(mJNIEnv, mJThis);

		// Video syscalls
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
			return _maGetSystemProperty(
				SYSCALL_THIS->GetValidatedStr(a),
				(int)SYSCALL_THIS->GetValidatedMemRange(b, c),
				(int)gCore->mem_ds,
				c,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPlatformRequest:
			SYSLOG("maIOCtl_maPlatformRequest");
			return _maPlatformRequest(SYSCALL_THIS->GetValidatedStr(a), mJNIEnv, mJThis);

		case maIOCtl_maShowVirtualKeyboard:
			SYSLOG("maIOCtl_maShowVirtualKeyboard");
			return _maShowVirtualKeyboard(mJNIEnv, mJThis);

		case maIOCtl_maTextBox:
		{
			SYSLOG("maIOCtl_maTextBox");

			// Send a focus lost event since the application will run in
			// the background during the time the maTextBox is running.
			MAEvent event;
			event.type = EVENT_TYPE_FOCUS_LOST;
			event.data = NULL;
			Base::gSyscall->postEvent(event);

			// Get the two first parameters of the IOCtl function
			const wchar* _title = GVWS(a);
			const wchar* _inText = GVWS(b);
			// Get two parameters from the stack
			int _maxSize = SYSCALL_THIS->GetValidatedStackValue(0);
			int _constraints = SYSCALL_THIS->GetValidatedStackValue(4);
			// Allocate memory for the output buffer
			int _outText = (int) SYSCALL_THIS->GetValidatedMemRange(
				c,
				_maxSize * sizeof(char));
			// Call the actual internal _maTextBox function
			return _maTextBox(
				_title,
				_inText,
				_outText,
				_maxSize,
				_constraints,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maWidgetCreate:
			SYSLOG("maIOCtl_maWidgetCreate");
			return _maWidgetCreate(SYSCALL_THIS->GetValidatedStr(a), mJNIEnv, mJThis);

		case maIOCtl_maWidgetDestroy:
			SYSLOG("maIOCtl_maWidgetDestroy");
			return _maWidgetDestroy(a, mJNIEnv, mJThis);

		case maIOCtl_maWidgetAddChild:
			SYSLOG("maIOCtl_maWidgetAddChild");
			return _maWidgetAddChild(a, b, mJNIEnv, mJThis);

		case maIOCtl_maWidgetInsertChild:
			SYSLOG("maIOCtl_maWidgetInsertChild");
			return _maWidgetInsertChild(a, b, c, mJNIEnv, mJThis);

		case maIOCtl_maWidgetRemoveChild:
			SYSLOG("maIOCtl_maWidgetRemoveChild");
			return _maWidgetRemoveChild(a, mJNIEnv, mJThis);

		case maIOCtl_maWidgetModalDialogShow:
			SYSLOG("maIOCtl_maWidgetModalDialogShow");
			return _maWidgetModalDialogShow(a, mJNIEnv, mJThis);

		case maIOCtl_maWidgetModalDialogHide:
			SYSLOG("maIOCtl_maWidgetModalDialogHide");
			return _maWidgetModalDialogHide(a, mJNIEnv, mJThis);

		case maIOCtl_maWidgetSetProperty:
			SYSLOG("maIOCtl_maWidgetSetProperty");
			return _maWidgetSetProperty(a, SYSCALL_THIS->GetValidatedStr(b), SYSCALL_THIS->GetValidatedStr(c), mJNIEnv, mJThis);

		case maIOCtl_maWidgetGetProperty:
		{
			SYSLOG("maIOCtl_maWidgetGetProperty");
			int _widget = a;
			const char *_property = SYSCALL_THIS->GetValidatedStr(b);
			//Read the fourth parameter from the register
			//(the first three can be read directly)
			int _valueBufferSize = SYSCALL_THIS->GetValidatedStackValue(0);
			int _valueBuffer = (int) SYSCALL_THIS->GetValidatedMemRange(
				c,
				_valueBufferSize * sizeof(char));

			return _maWidgetGetProperty((int)gCore->mem_ds, _widget, _property, _valueBuffer, _valueBufferSize, mJNIEnv, mJThis);
		}

		case maIOCtl_maWidgetScreenShow:
			SYSLOG("maIOCtl_maWidgetScreenShow");
			return _maWidgetScreenShow(a, mJNIEnv, mJThis);

		case maIOCtl_maWidgetStackScreenPush:
			SYSLOG("maIOCtl_maWidgetStackScreenPush");
			return _maWidgetStackScreenPush(a, b, mJNIEnv, mJThis);

		case maIOCtl_maWidgetStackScreenPop:
			SYSLOG("maIOCtl_maWidgetStackScreenPop");
			return _maWidgetStackScreenPop(a, mJNIEnv, mJThis);

		case maIOCtl_maNotificationAdd:
			SYSLOG("maIOCtl_maNotificationAdd");
			return _maNotificationAdd(
				a,
				b,
				SYSCALL_THIS->GetValidatedStr(c),
				SYSCALL_THIS->GetValidatedStr(
					SYSCALL_THIS->GetValidatedStackValue(0)),
				mJNIEnv,
				mJThis);

		case maIOCtl_maNotificationRemove:
			SYSLOG("maIOCtl_maNotificationRemove");
			return _maNotificationRemove(a, mJNIEnv, mJThis);

		case maIOCtl_maSendToBackground:
			SYSLOG("maIOCtl_maSendToBackground");
			// Send EVENT_TYPE_FOCUS_LOST
			return _maSendToBackground(mJNIEnv, mJThis);

		case maIOCtl_maBringToForeground:
			// Not available on Android.
			return -1;

		case maIOCtl_maScreenSetOrientation:
			SYSLOG("maIOCtl_maScreenSetOrientation");
			return _maScreenSetOrientation(a, mJNIEnv, mJThis);

		case maIOCtl_maScreenSetFullscreen:
			SYSLOG("maIOCtl_maScreenSetFullscreen");
			return _maScreenSetFullscreen(a, mJNIEnv, mJThis);

		case maIOCtl_maHomeScreenEventsOn:
			SYSLOG("maIOCtl_maHomeScreenEventsOn");
			// 1 = events on
			return _maHomeScreenEventsOnOff(1, mJNIEnv, mJThis);

		case maIOCtl_maHomeScreenEventsOff:
			SYSLOG("maIOCtl_maHomeScreenEventsOff");
			// 0 = events off
			return _maHomeScreenEventsOnOff(0, mJNIEnv, mJThis);

		case maIOCtl_maHomeScreenShortcutAdd:
			SYSLOG("maIOCtl_maHomeScreenShortcutAdd");
			return _maHomeScreenShortcutAdd(
				SYSCALL_THIS->GetValidatedStr(a),
				mJNIEnv,
				mJThis);

		case maIOCtl_maHomeScreenShortcutRemove:
			SYSLOG("maIOCtl_maHomeScreenShortcutRemove");
			return _maHomeScreenShortcutRemove(
				SYSCALL_THIS->GetValidatedStr(a),
				mJNIEnv,
				mJThis);

		case maIOCtl_maScreenStateEventsOn:
			SYSLOG("maIOCtl_maScreenStateEventsOn");
			// 1 = events on
			return _maScreenStateEventsOnOff(1, mJNIEnv, mJThis);

		case maIOCtl_maScreenStateEventsOff:
			SYSLOG("maIOCtl_maScreenStateEventsOff");
			// 0 = events off
			return _maScreenStateEventsOnOff(0, mJNIEnv, mJThis);

		case maIOCtl_maMessageBox:
			SYSLOG("maIOCtl_maMessageBox");
			return _maMessageBox(
				SYSCALL_THIS->GetValidatedStr(a),
				SYSCALL_THIS->GetValidatedStr(b),
				mJNIEnv,
				mJThis);

		case maIOCtl_maAlert:
			SYSLOG("maIOCtl_maAlert");
			return _maAlert(
				SYSCALL_THIS->GetValidatedStr(a),
				SYSCALL_THIS->GetValidatedStr(b),
				SYSCALL_THIS->GetValidatedStr(c),
				SYSCALL_THIS->GetValidatedStr(
					SYSCALL_THIS->GetValidatedStackValue(0)),
				SYSCALL_THIS->GetValidatedStr(
					SYSCALL_THIS->GetValidatedStackValue(4)),
				mJNIEnv,
				mJThis);

		case maIOCtl_maImagePickerOpen:
			SYSLOG("maIOCtl_maImagePickerOpen");
			return _maImagePickerOpen(
				mJNIEnv,
				mJThis);

		case maIOCtl_maOptionsBox:
			{
			SYSLOG("maIOCtl_maOptionsBox");
			const wchar* _title = GVWS(a);
			const wchar* _text = GVWS(b);
			const wchar* _cancel = GVWS(c);

			// Read the fourth and fifth parameter from the register.
			return _maOptionsBox(
				_title,
				_text,
				_cancel,
				SYSCALL_THIS->GetValidatedStackValue(0),
				SYSCALL_THIS->GetValidatedStackValue(4),
				mJNIEnv,
				mJThis);
			}

		case maIOCtl_maFileOpen:
			SYSLOG("maIOCtl_maFileOpen");
			return _maFileOpen(
				SYSCALL_THIS->GetValidatedStr(a),
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileExists:
			return _maFileExists(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileClose:
			return _maFileClose(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileCreate:
			return _maFileCreate(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileDelete:
			return _maFileDelete(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileSize:
			return _maFileSize(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileAvailableSpace:
			return _maFileAvailableSpace(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileTotalSpace:
			return _maFileTotalSpace(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileDate:
			return _maFileDate(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileRename:
			return _maFileRename(
				a,
				SYSCALL_THIS->GetValidatedStr(b),
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileTruncate:
			return _maFileTruncate(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileWrite:
			return _maFileWrite(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(char)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileWriteFromData:
			return _maFileWriteFromData(
				a,
				b,
				c,
				SYSCALL_THIS->GetValidatedStackValue(0),
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileRead:
			return _maFileRead(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(char)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileReadToData:
			return _maFileReadToData(
				a,
				b,
				c,
				SYSCALL_THIS->GetValidatedStackValue(0),
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileTell:
			return _maFileTell(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileSeek:
			return _maFileSeek(
				a,
				b,
				c,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileListStart:
			return _maFileListStart(
				SYSCALL_THIS->GetValidatedStr(a),
				SYSCALL_THIS->GetValidatedStr(b),
				mJNIEnv,
				mJThis);

		case maIOCtl_maFileListNext:
			return _maFileListNext(
				a,
				b,
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis
				);

		case maIOCtl_maFileListClose:
			return _maFileListClose(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFontLoadDefault:
			SYSLOG("maIOCtl_maFontLoadDefault");
			return _maFontLoadDefault(
				a,
				b,
				c,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFontSetCurrent:
			SYSLOG("maIOCtl_maFontSetCurrent");
			return _maFontSetCurrent(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFontGetCount:
			SYSLOG("maIOCtl_maFontGetCount");
			return _maFontGetCount(
				mJNIEnv,
				mJThis);

		case maIOCtl_maFontGetName:
			{
			SYSLOG("maIOCtl_maFontGetName");
			int _index = a;
			int _valueBufferSize = c;
			int _valueBuffer = (int) SYSCALL_THIS->GetValidatedMemRange(
				b,
				_valueBufferSize * sizeof(char));

			return _maFontGetName((int)gCore->mem_ds, _index, _valueBuffer, _valueBufferSize, mJNIEnv, mJThis);
			}

		case maIOCtl_maFontLoadWithName:
			SYSLOG("maIOCtl_maFontLoadWithName");
			return _maFontLoadWithName(
				SYSCALL_THIS->GetValidatedStr(a),
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maFontDelete:
			SYSLOG("maIOCtl_maFontDelete");
			return _maFontDelete(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraStart:
			return _maCameraStart(
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraStop:
			return _maCameraStop(
				mJNIEnv,
				mJThis);


		case maIOCtl_maCameraSnapshot:
			return _maCameraSnapshot(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraSetPreview:
			return _maCameraSetPreview(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraNumber:
			return _maCameraNumber(
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraSelect:
			return _maCameraSelect(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraRecord:
			return _maCameraRecord(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraFormatNumber:
			return _maCameraFormatNumber(
				mJNIEnv,
				mJThis);

		case maIOCtl_maCameraFormat:
		{

			// b is pointer to struct 	MA_CAMERA_FORMAT
				MA_CAMERA_FORMAT* sizeInfo = (MA_CAMERA_FORMAT*) SYSCALL_THIS->GetValidatedMemRange(b, sizeof(MA_CAMERA_FORMAT));

			// Size of buffer to store device name.
			int width = sizeInfo->width;

			// Size of buffer to store device name.
			int height = sizeInfo->height;


			// Returns 1 for success, 0 for no more devices.
			return _maCameraFormat(
				a,
				width,
				height,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maCameraSetProperty:
			return _maCameraSetProperty(SYSCALL_THIS->GetValidatedStr(a), SYSCALL_THIS->GetValidatedStr(b), mJNIEnv, mJThis);

		case maIOCtl_maCameraGetProperty:
		{
			const char *_property = SYSCALL_THIS->GetValidatedStr(a);
			int _valueBufferSize = c;
			int _valueBuffer = (int) SYSCALL_THIS->GetValidatedMemRange(
				b,
				_valueBufferSize * sizeof(char));

			return _maCameraGetProperty((int)gCore->mem_ds, _property, _valueBuffer, _valueBufferSize, mJNIEnv, mJThis);
		}

		case maIOCtl_maSensorStart:
			SYSLOG("maIOCtl_maSensorStart");
			return _maSensorStart(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maSensorStop:
			SYSLOG("maIOCtl_maSensorStop");
			return _maSensorStop(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimListOpen:
			SYSLOG("maIOCtl_maPimListOpen");
			return _maPimListOpen(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimListNext:
			SYSLOG("maIOCtl_maPimListNext");
			return _maPimListNext(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimListClose:
			SYSLOG("maIOCtl_maPimListClose");
			return _maPimListClose(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemCount:
			SYSLOG("maIOCtl_maPimItemCount");
			return _maPimItemCount(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemGetField:
			SYSLOG("maIOCtl_maPimItemGetField");
			return _maPimItemGetField(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemFieldCount:
			SYSLOG("maIOCtl_maPimItemFieldCount");
			return _maPimItemFieldCount(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemGetAttributes:
			SYSLOG("maIOCtl_maPimItemGetAttributes");
			return _maPimItemGetAttributes(
				a,
				b,
				c,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemSetLabel:
		{
			SYSLOG("maIOCtl_maPimItemSetLabel");

			MA_PIM_ARGS* args = (MA_PIM_ARGS*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MA_PIM_ARGS));

			return _maPimItemSetLabel(
				args->item,
				args->field,
				args->buf,
				args->bufSize,
				b,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maPimItemGetLabel:
		{
			SYSLOG("maIOCtl_maPimItemGetLabel");

			MA_PIM_ARGS* args = (MA_PIM_ARGS*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MA_PIM_ARGS));

			return _maPimItemGetLabel(
				args->item,
				args->field,
				args->buf,
				args->bufSize,
				b,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maPimFieldType:
			SYSLOG("maIOCtl_maPimFieldType");
			return _maPimFieldType(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemGetValue:
		{
			SYSLOG("maIOCtl_maPimItemGetValue");

			MA_PIM_ARGS* args = (MA_PIM_ARGS*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MA_PIM_ARGS));

			return _maPimItemGetValue(
				args->item,
				args->field,
				args->buf,
				args->bufSize,
				b,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maPimItemSetValue:
		{
			SYSLOG("maIOCtl_maPimItemSetValue");

			MA_PIM_ARGS* args = (MA_PIM_ARGS*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MA_PIM_ARGS));

			return _maPimItemSetValue(
				args->item,
				args->field,
				args->buf,
				args->bufSize,
				b,
				c,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maPimItemAddValue:
		{
			SYSLOG("maIOCtl_maPimItemAddValue");

			MA_PIM_ARGS* args = (MA_PIM_ARGS*) SYSCALL_THIS->GetValidatedMemRange(a, sizeof(MA_PIM_ARGS));

			return _maPimItemAddValue(
				args->item,
				args->field,
				args->buf,
				args->bufSize,
				b,
				mJNIEnv,
				mJThis);
		}

		case maIOCtl_maPimItemRemoveValue:
			SYSLOG("maIOCtl_maPimItemRemoveValue");
			return _maPimItemRemoveValue(
				a,
				b,
				c,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemClose:
			SYSLOG("maIOCtl_maPimItemClose");
			return _maPimItemClose(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemCreate:
			SYSLOG("maIOCtl_maPimItemCreate");
			return _maPimItemCreate(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maPimItemRemove:
			SYSLOG("maIOCtl_maPimItemRemove");
			return _maPimItemRemove(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCStart:
			SYSLOG("maIOCtl_maNFCStart");
			return _maNFCStart(mJNIEnv, mJThis);

		case maIOCtl_maNFCStop:
			SYSLOG("maIOCtl_maNFCStop");
			return _maNFCStop(mJNIEnv, mJThis);

		case maIOCtl_maNFCReadTag:
			return _maNFCReadTag(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCDestroyTag:
			return _maNFCDestroyTag(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCConnectTag:
			return _maNFCConnectTag(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCCloseTag:
			return _maNFCCloseTag(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCIsType:
			return _maNFCIsType(a, b, mJNIEnv, mJThis);

		case maIOCtl_maNFCGetTypedTag:
			return _maNFCGetTypedTag(a, b, mJNIEnv, mJThis);

		case maIOCtl_maNFCBatchStart:
			return _maNFCBatchStart(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCBatchCommit:
			return _maNFCBatchCommit(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCBatchRollback:
			return _maNFCBatchRollback(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCTransceive:
		{
			int dst = SYSCALL_THIS->GetValidatedStackValue(0);
			int dstLen = SYSCALL_THIS->GetValidatedStackValue(4);
			return _maNFCTransceive(
					a,
					(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
					c,
					(int) SYSCALL_THIS->GetValidatedMemRange( dst, dstLen * sizeof(byte)),
					dstLen,
					(int)gCore->mem_ds,
					mJNIEnv,
					mJThis);
		}

		case maIOCtl_maNFCGetNDEFMessage:
			return _maNFCGetNDEFMessage(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCReadNDEFMessage:
			return _maNFCReadNDEFMessage(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCWriteNDEFMessage:
			return _maNFCWriteNDEFMessage(a, b, mJNIEnv, mJThis);

		case maIOCtl_maNFCCreateNDEFMessage:
			return _maNFCCreateNDEFMessage(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCGetNDEFRecord:
			return _maNFCGetNDEFRecord(a, b, mJNIEnv, mJThis);

		case maIOCtl_maNFCGetNDEFRecordCount:
			return _maNFCGetNDEFRecordCount(a, mJNIEnv, mJThis);

		case maIOCtl_maNFCGetId:
			return _maNFCGetId(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCGetPayload:
			return _maNFCGetPayload(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCGetTnf:
			return _maNFCGetTnf(
				a,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCGetType:
			return _maNFCGetType(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCSetId:
			return _maNFCSetId(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCSetPayload:
			return _maNFCSetPayload(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCSetTnf:
			return _maNFCSetTnf(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCSetType:
			return _maNFCSetType(
				a,
				(int) SYSCALL_THIS->GetValidatedMemRange( b, c * sizeof(byte)),
				c,
				(int)gCore->mem_ds,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCAuthenticateSector:
		{
			int keyAddr = SYSCALL_THIS->GetValidatedStackValue(0);
			int keyLen = SYSCALL_THIS->GetValidatedStackValue(4);
			return _maNFCAuthenticateSector(
					a,
					b,
					c,
					(int) SYSCALL_THIS->GetValidatedMemRange( keyAddr, keyLen * sizeof(byte)),
					keyLen,
					(int)gCore->mem_ds,
					mJNIEnv,
					mJThis);
		}

		case maIOCtl_maNFCGetSectorCount:
			return _maNFCGetSectorCount(
					a,
					mJNIEnv,
					mJThis);

		case maIOCtl_maNFCGetBlockCountInSector:
			return _maNFCGetBlockCountInSector(
					a,
					b,
					mJNIEnv,
					mJThis);

		case maIOCtl_maNFCSectorToBlock:
			return _maNFCSectorToBlock(
				a,
				b,
				mJNIEnv,
				mJThis);

		case maIOCtl_maNFCReadBlocks:
		{
			int len = SYSCALL_THIS->GetValidatedStackValue(0);
			return _maNFCReadBlocks(
					a,
					b,
					(int) SYSCALL_THIS->GetValidatedMemRange( c, len * sizeof(byte)),
					len,
					(int)gCore->mem_ds,
					mJNIEnv,
					mJThis);
		}

		case maIOCtl_maNFCReadPages:
		{
			int len = SYSCALL_THIS->GetValidatedStackValue(0);
			return _maNFCReadPages(
					a,
					b,
					(int) SYSCALL_THIS->GetValidatedMemRange( c, len * sizeof(byte)),
					len,
					(int)gCore->mem_ds,
					mJNIEnv,
					mJThis);
		}

		case maIOCtl_maNFCWriteBlocks:
		{
			int len = SYSCALL_THIS->GetValidatedStackValue(0);
			return _maNFCWriteBlocks(
					a,
					b,
					(int) SYSCALL_THIS->GetValidatedMemRange( c, len * sizeof(byte)),
					len,
					(int)gCore->mem_ds,
					mJNIEnv,
					mJThis);
		}

		case maIOCtl_maNFCWritePages:
		{
			int len = SYSCALL_THIS->GetValidatedStackValue(0);
			return _maNFCWritePages(
					a,
					b,
					(int) SYSCALL_THIS->GetValidatedMemRange( c, len * sizeof(byte)),
					len,
					(int)gCore->mem_ds,
					mJNIEnv,
					mJThis);
		}

		case maIOCtl_maNFCSetReadOnly:
			return _maNFCSetReadOnly(
					a,
					mJNIEnv,
					mJThis);

		case maIOCtl_maNFCIsReadOnly:
			return _maNFCIsReadOnly(
					a,
					mJNIEnv,
					mJThis);

		case maIOCtl_maSyscallPanicsEnable:
			SYSLOG("maIOCtl_maSyscallPanicsEnable");
			return _maSyscallPanicsEnable(
				mJNIEnv,
				mJThis);

		case maIOCtl_maSyscallPanicsDisable:
			SYSLOG("maIOCtl_maSyscallPanicsDisable");
			return _maSyscallPanicsDisable(
				mJNIEnv,
				mJThis);

		} // End of switch

		return IOCTL_UNAVAILABLE;
	}
}

bool reloadProgram()
{
	if(true == Base::gSyscall->isReloading())
	{
		Base::gSyscall->resetSyscallState();
		return true;
	}
	return false;
}

void MoSyncExit(int errorCode)
{
	//__android_log_write(ANDROID_LOG_INFO, "MoSyncExit!", "Program has exited!");

	if(false == reloadProgram())
	{
		//__android_log_write(ANDROID_LOG_INFO, "MoSyncExit!", "nahh.. just die now");

		exit(errorCode);
	}
	else
	{
		//__android_log_write(ANDROID_LOG_INFO, "MoSyncExit!", "Should reload program");

		Base::gEventFifo.clear();

		Base::SYSCALL_THIS->VM_Yield();
	}
}

void MoSyncErrorExit(int errorCode)
{
	if(false == reloadProgram())
	{
		char* b = (char*)malloc(200);
		sprintf(b, "MoSync error: %i", errorCode);
		//__android_log_write(ANDROID_LOG_INFO, "MoSyncErrorExit!", b);
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

	Base::gEventFifo.clear();
	Base::SYSCALL_THIS->VM_Yield();
}



// Build the event.


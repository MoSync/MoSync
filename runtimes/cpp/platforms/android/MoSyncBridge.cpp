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

#include <Core.h>
#include <base/Syscall.h>

#include <string.h>
#include <stdio.h>
#include <jni.h>

#include <unistd.h>

#include <android/log.h>

// Comment/comment out to turn on/off logging.
// TODO: Move this to a header file to support common logging for all files.
#define SYSLOG(m) __android_log_write(ANDROID_LOG_INFO, "MoSync JNI", m)
//#define SYSLOG(...)

/* 
// This is how you trap exeptions from the Java side.
// TODO: This function is not used, but could be used
// to check for exceptions after calling a Java method. 
static void handlePendingExceptions(JNIEnv* env)
{
	jthrowable exc;
	exc = env->ExceptionOccurred();
	if (exc) 
	{
		__android_log_write(
			ANDROID_LOG_INFO, 
			"@@@ MoSync", 
			"Found pending exception");
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}
*/

/**
* @brief Function that initializes the native core	
*/
static jboolean nativeInitRuntime(JNIEnv* env, jobject jthis)
{
	SYSLOG("InitRuntime start");

	Base::Syscall *syscall = 0;
	syscall = new Base::Syscall();

	gCore = Core::CreateCore(*syscall);
	if(NULL == gCore)
	{
		SYSLOG("Couldn't create core");
		return false;
	}
	SYSLOG("Core created");
	return true;
}

/**
* /return The newly created Data Section as a Direct ByteBuffer object
*/
static jboolean nativeLoad(
	JNIEnv* env, 
	jobject jthis, 
	jobject program, 
	jlong programOffset, 
	jobject resource, 
	jlong resourceOffset)
{
	SYSLOG("load program and resource");

	int prgFd;
	/**
		Due to logic -1 is never passed to LoadVMApp, use -2 when
		no resource file is needed
	    details: 
			-1: resource file needed but not found
			-2: resource file is not needed
	*/
	int resFd = -2;
	
	SYSLOG("get program file");
	jclass fdClass = env->FindClass("java/io/FileDescriptor");
	if (fdClass != NULL) 
	{
		jclass fdPrgClassRef = (jclass) env->NewGlobalRef(fdClass); 
		jfieldID fdClassDescriptorFieldID = 
			env->GetFieldID(fdPrgClassRef, "descriptor", "I");
		
		if (fdClassDescriptorFieldID != NULL && program != NULL) 
		{			
			jint fd = env->GetIntField(program, fdClassDescriptorFieldID);	
			prgFd = dup(fd);
			lseek(prgFd, programOffset, SEEK_SET);
		}
	}
	
	if (resourceOffset != 0)
	{
		SYSLOG("get resource file");
		jclass fdClass2 = env->FindClass("java/io/FileDescriptor");
		if (fdClass2 != NULL) 
		{
			jclass fdResClassRef = (jclass) env->NewGlobalRef(fdClass2); 
			jfieldID fdClassDescriptorFieldID = 
				env->GetFieldID(fdResClassRef, "descriptor", "I");
			
			if (fdClassDescriptorFieldID != NULL && resource != NULL) 
			{			
				jint fd = env->GetIntField(resource, fdClassDescriptorFieldID);	
				resFd = dup(fd);
				lseek(resFd, resourceOffset, SEEK_SET);
			}
			else if (resource == NULL)
			{
				resFd = -2;
			}
		}
	}

	SYSLOG("both files was obtained!");
	
	if (-1 == prgFd)
	{
		SYSLOG("No program file available!");
		return false;
	}
	
	SYSLOG("set jni environment");
	
	gCore->mJniEnv = env; gCore->mJThis = jthis;
	Base::gSyscall->setJNIEnvironment(env, jthis);
	
	SYSLOG("call load vm app");

	if (NULL == gCore) 
	{
		maPanic(-1, "MoSyncBridge.cpp: nativeLoad: NULL == gCore");
	}
	else
	{
		SYSLOG("gCore exists");
	}
	
	if (-1 == prgFd)
	{
		maPanic(-1, "MoSyncBridge.cpp: nativeLoad: -1 == prgFd");
	}
	else
	{
		SYSLOG("prgFd exists");
	}
	
	if (-1 == resFd)
	{
		maPanic(-1, "MoSyncBridge.cpp: nativeLoad: -1 == resFd");
	}
	else
	{
		SYSLOG("resFd exists");
	}
	
	return Core::LoadVMApp(gCore, prgFd, resFd);
}

/**
* @brief nativeLoadResource
*/
static jboolean nativeLoadResource(JNIEnv* env, jobject jthis, jobject resource)
{
	SYSLOG("load resource");

	char* resourceBuffer = (char*)env->GetDirectBufferAddress(resource);
	
}

/**
* /return The newly created Data Section as a Direct ByteBuffer object
*/
static jobject nativeLoadCombined(JNIEnv* env, jobject jthis, jobject combined)
{
	SYSLOG("load combined file");

	char* combinedBuffer = (char*)env->GetDirectBufferAddress(combined);
	
	// TODO: Remove commented out code.
	//gCore->jniEnv = env;
	// do the thing
	
	return gCore->mem_ds_jobject;
}

/**
* /return The newly created Data Section as a Direct ByteBuffer object
*/
static void nativeRun(JNIEnv* env, jobject jthis)
{
	SYSLOG("nativeRun");
	
	Base::gSyscall->setJNIEnvironment(env, jthis);
	
	while(1)
	{
		Core::Run2(gCore);
		
		SYSLOG("Runtime yielded!");
		
		// Check if we should load a resource as a program/resource combfile
		int reloadHandle = Base::gSyscall->getReloadHandle();
		if (0 != reloadHandle)
		{
			SYSLOG("Program is loading from handle");
		
			Base::Stream* stream = 
				Base::gSyscall->resources.extract_RT_BINARY(reloadHandle);
			
			bool res = Core::LoadVMApp(gCore, *stream);
			
			delete stream;
			
			if (!res) 
			{
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
			}
			
			Base::gSyscall->setReloadHandle(0);
		}
		// Check if we should reload the initial program and resource file
		else if (Base::gSyscall->isReloading())
		{
			SYSLOG("Program is reloading! 1");
			
			Base::gSyscall->setReloading(false);
			
			SYSLOG("Program is reloading! 2");
			
			jclass cls = env->GetObjectClass(jthis);
			jmethodID methodID = env->GetMethodID(cls, "loadProgram", "()Z");
			if (methodID == 0) return;
			jboolean res = env->CallBooleanMethod(jthis, methodID);
			
			SYSLOG("Program is reloading! 3");
			
			if( !res) 
			{
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
			}
			
			SYSLOG("Program is reloading! 4");
		}
	}
}

/**
* @brief nativePostEvent
*/
static void nativePostEvent(JNIEnv* env, jobject jthis, jintArray eventBuffer)
{
	// Get array data.
	jsize arrayLength = env->GetArrayLength(eventBuffer);
	jint *intArray = env->GetIntArrayElements(eventBuffer, 0);
	
	// Must have at least one element.
	if (arrayLength < 1)
	{
		maPanic(-1, "MoSyncBridge.cpp: nativePostEvent: array length is zero");
		return;
	}
	
	// Build the event.
	MAEvent event;
	event.type = intArray[0];
	event.data = NULL;
	
	// Print log message.
	char logBuf[256];
	sprintf(logBuf, "nativePostEvent event.type: %i", event.type);
	SYSLOG(logBuf);
	
	if (event.type == EVENT_TYPE_POINTER_PRESSED || 
		event.type == EVENT_TYPE_POINTER_RELEASED || 
		event.type == EVENT_TYPE_POINTER_DRAGGED)
	{
		event.point.x = intArray[1];
		event.point.y = intArray[2];
		event.touchId = intArray[3];
	}
	else if (
		event.type == EVENT_TYPE_KEY_RELEASED || 
		event.type == EVENT_TYPE_KEY_PRESSED)
	{
		event.key = intArray[1];
		event.nativeKey = intArray[2];
	}
	else if (event.type == EVENT_TYPE_CONN)
	{
		event.conn.handle = intArray[1];
		event.conn.opType = intArray[2];
		event.conn.result = intArray[3];
	}
	else if (event.type == EVENT_TYPE_LOCATION)
	{
		// Copy location data. This data starts at array index 1.
		int size = sizeof(MALocation);
		byte* data = new byte[size];
		memcpy(data, intArray + 1, size);
		event.data = (int)data;
	}
	else if (event.type == EVENT_TYPE_LOCATION_PROVIDER)
	{
		event.state = intArray[1];
	}
	else if (event.type == EVENT_TYPE_BT)
	{
		event.state = intArray[1];
	}
	else if (event.type == EVENT_TYPE_TEXTBOX)
	{
		event.textboxResult = intArray[1];
		event.textboxLength = intArray[2];
	}
	else if (event.type == EVENT_TYPE_IMAGE_PICKER)
	{
		event.imagePickerState = intArray[1];
	}
	else if (event.type == EVENT_TYPE_WIDGET)
	{
		/*
		 * Structure of intArray for widget events.
		 *
		 * Required: 
		 * intArray[0] - EVENT_TYPE_WIDGET
		 * intArray[1] - Specifies exactly which widget event that occurred.
		 * intArray[2] - Handle to the widget that sent the event.
		 *
		 * Optional:
		 * WIDGET_EVENT_MESSAGE
		 * intArray[3] - The id of the message being sent (if it has 
		 *               dynamically allocated data)
		 * intARray[4] - Size of the message.
		 *
		 * WIDGET_EVENT_CLICKED
		 * intArray[3] - Can be used to determine a checkbox or toggle button that was clicked.
		 *
		 * WIDGET_EVENT_ITEM_CLICKED
		 * intArray[3] - The index of the list item that was clicked.
		 *
		 * WIDGET_EVENT_TAB_CHANGED
		 * intArray[3] - The index of the new tab.
		 *
		 * WIDGET_EVENT_STACK_SCREEN_POPPED
		 * intArray[3] - Handle to the screen that was popped.
		 * intArray[4] - Handle to the screen that we popped to.
		 *
		 * WIDGET_EVENT_SLIDER_VALUE_CHANGED
		 * intArray[3] - The slider value.
		 *
		 * WIDGET_EVENT_DATE_PICKER_VALUE_CHANGED
		 * intArray[3] - The day of month value.
		 * intArray[4] - The month value.
		 * intArray[5] - The year value.
		 *
		 * WIDGET_EVENT_TIME_PICKER_VALUE_CHANGED
		 * intArray[3] - The current hour value.
		 * intArray[4] - The current minute value.
		 *
		 * WIDGET_EVENT_NUMBER_PICKER_VALUE_CHANGED
		 * intArray[3] - The current value.
		 *
		 * WIDGET_EVENT_VIDEO_STATE_CHANGED
		 * intArray[3] - One of the MAW_VIDEO_WIDGET_STATE constants.
		 */

		int widgetEventType = intArray[1];
		// MAGetEvent will handle the deallocation of this memory
		MAWidgetEventData *widgetEvent = new MAWidgetEventData;
		
		widgetEvent->eventType    = intArray[1];
		widgetEvent->widgetHandle = intArray[2];
		
		if (widgetEventType == MAW_EVENT_CLICKED)
		{
			widgetEvent->checked = intArray[3];
		}
		else if (widgetEventType == MAW_EVENT_ITEM_CLICKED)
		{
			widgetEvent->listItemIndex = intArray[3];
		}
		else if (widgetEventType == MAW_EVENT_TAB_CHANGED)
		{
			widgetEvent->tabIndex = intArray[3];
		}
		else if (widgetEventType == MAW_EVENT_STACK_SCREEN_POPPED)
		{
			widgetEvent->fromScreen = intArray[3];
			widgetEvent->toScreen = intArray[4];
		}
		else if(widgetEventType == MAW_EVENT_SLIDER_VALUE_CHANGED)
		{
			widgetEvent->sliderValue = intArray[3];
		}
		else if(widgetEventType == MAW_EVENT_DATE_PICKER_VALUE_CHANGED)
		{
			widgetEvent->dayOfMonth = intArray[3];
			widgetEvent->month = intArray[4];
			widgetEvent->year = intArray[5];
		}
		else if(widgetEventType == MAW_EVENT_TIME_PICKER_VALUE_CHANGED)
		{
			widgetEvent->hour = intArray[3];
			widgetEvent->minute = intArray[4];
		}
		else if(widgetEventType == MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED)
		{
			widgetEvent->numberPickerValue = intArray[3];
		}
		else if(widgetEventType == MAW_EVENT_VIDEO_STATE_CHANGED)
		{
			widgetEvent->videoViewState = intArray[3];
		}
		event.data = (int)widgetEvent;
	}
	
	// Release the memory used for the int array.
	env->ReleaseIntArrayElements(eventBuffer, intArray, 0);
	
	Base::gSyscall->postEvent(event);
}

static int nativeCreateBinaryResource(
	JNIEnv* env, 
	jobject jthis, 
	int resourceIndex, 
	int size)
{
	return Base::gSyscall->loadBinaryStore(resourceIndex, size);
}

/**
* @brief jniRegisterNativeMethods
*/
int jniRegisterNativeMethods(
	JNIEnv* env, 
	const char* className, 
	const JNINativeMethod* gMethods, 
	int numMethods)
{
	jclass clazz;

	SYSLOG("Register native functions");
	
	clazz = env->FindClass(className);
	if (clazz == NULL)
	{
		SYSLOG("Class not found");
		return -1;
	}
	
	SYSLOG("Class found!");
	
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
	{
		SYSLOG("Failed to register native methods");
		return -1;
	}
	
	return 0;
}

jint gNumJavaMethods = 7;
static JNINativeMethod sMethods[] =
{
    // name, signature, funcPtr 
	{ "nativeInitRuntime", "()Z", (void*)nativeInitRuntime }, 
	{ "nativeLoad", "(Ljava/io/FileDescriptor;JLjava/io/FileDescriptor;J)Z", (void*)nativeLoad },
	{ "nativeLoadResource", "(Ljava/nio/ByteBuffer;)Z", (void*)nativeLoadResource },
	{ "nativeLoadCombined", "(Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;", (void*)nativeLoadCombined },
	{ "nativeRun", "()V", (void*)nativeRun },
	{ "nativePostEvent", "([I)V", (void*)nativePostEvent },
	{ "nativeCreateBinaryResource", "(II)I", (void*)nativeCreateBinaryResource }
};

/**
* @brief JNI_OnLoad
*/
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	SYSLOG("JNI_OnLoad");
	
	SYSLOG("Check JNI version");

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	{
		return result;
	}
	
	jniRegisterNativeMethods(
		env, 
		"com/mosync/internal/android/MoSyncThread", 
		sMethods, 
		gNumJavaMethods);
	
	return JNI_VERSION_1_4;
}

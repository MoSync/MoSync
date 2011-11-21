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
//#define SYSLOG(m) __android_log_write(ANDROID_LOG_INFO, "MoSync JNI", m)
#define SYSLOG(...)

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
 * TODO: This comment seems outdated. Returns a boolean rather than a
 * byte buffer. UPDATE COMMENT.
 * @return The newly created Data Section as a Direct ByteBuffer object
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

	// Program file descriptor.
	int prgFd;

	// Due to logic -1 is never passed to LoadVMApp, use -2 when
	// no resource file is needed.
	// Details:
	//  -1: resource file needed but not found
	//  -2: resource file is not needed
	int resFd = -2;

	// Check that global variable gCore has a value.
	if (NULL == gCore)
	{
		maPanic(-1, "MoSyncBridge.cpp: nativeLoad: NULL == gCore");
	}
	else
	{
		SYSLOG("gCore exists");
	}

	// Get program file descriptor.
	SYSLOG("MoSyncBridge.cpp: nativeLoad: Get program file descriptor");

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

	// Check that program file exists.
	if (-1 == prgFd)
	{
		maPanic(-1, "MoSyncBridge.cpp: nativeLoad: No program file available!");
	}
	else
	{
		SYSLOG("MoSyncBridge.cpp: nativeLoad: Program file exists!");
	}

	// Get resorce file descriptor.

	if (resourceOffset != 0)
	{
		SYSLOG("MoSyncBridge.cpp: nativeLoad: Get resource file descriptor");
		jclass fdClass2 = env->FindClass("java/io/FileDescriptor");
		if (fdClass2 != NULL)
		{
			jclass fdResClassRef = (jclass) env->NewGlobalRef(fdClass2);
			jfieldID fdClassDescriptorFieldID =
				env->GetFieldID(fdResClassRef, "descriptor", "I");

			if (fdClassDescriptorFieldID != NULL && resource != NULL)
			{
				jint fd = env->GetIntField(
					resource,
					fdClassDescriptorFieldID);
				resFd = dup(fd);
				lseek(resFd, resourceOffset, SEEK_SET);
			}
			else if (resource == NULL)
			{
				resFd = -2;
			}
		}
	}

	SYSLOG("MoSyncBridge.cpp: nativeLoad: Setting jni environment");

	gCore->mJniEnv = env;
	gCore->mJThis = jthis;
	Base::gSyscall->setJNIEnvironment(env, jthis);

	SYSLOG("MoSyncBridge.cpp: nativeLoad: Calling Core::LoadVMApp");

	return Core::LoadVMApp(gCore, prgFd, resFd);
}

/**
* @brief nativeLoadResource
* TODO: This method is unfinished!
*/
static jboolean nativeLoadResource(JNIEnv* env, jobject jthis, jobject resource)
{
	SYSLOG("load resource");

	char* resourceBuffer = (char*)env->GetDirectBufferAddress(resource);

	// TODO: Now what?
}

/**
 * @return The newly created Data Section as a Direct ByteBuffer object
 * TODO: This method looks unfinished!
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

			if (!res)
			{
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
			}

			//__android_log_write(ANDROID_LOG_INFO,"JNI","Program is reloading! 4");
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
		event.imagePickerItem = intArray[2];
	}
	else if (event.type == EVENT_TYPE_SMS)
	{
		event.status = intArray[1];
	}
	else if (event.type == EVENT_TYPE_ALERT)
	{
		event.alertButtonIndex = intArray[1];
	}
	else if (event.type == EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED)
	{
		event.optionsBoxButtonIndex = intArray[1];
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
		 * MAW_EVENT_CLICKED
		 * intArray[3] - Can be used to determine a checkbox or toggle button that was clicked.
		 *
		 * MAW_EVENT_ITEM_CLICKED
		 * intArray[3] - The index of the list item that was clicked.
		 *
		 * MAW_EVENT_TAB_CHANGED
		 * intArray[3] - The index of the new tab.
		 *
		 * MAW_EVENT_STACK_SCREEN_POPPED
		 * intArray[3] - Handle to the screen that was popped.
		 * intArray[4] - Handle to the screen that we popped to.
		 *
		 * MAW_EVENT_WEB_VIEW_CONTENT_LOADING
		 * intArray[3] - Page loading status.
		 *
		 * MAW_EVENT_WEB_VIEW_HOOK_INVOKED
		 * intArray[3] - Hook type.
		 * intArray[4] - Handle to url data.
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
		 *
		 * MAW_EVENT_WEB_VIEW_CONTENT_LOADING
		 * intArray[3] - Page loading status.
		 *
		 * MAW_EVENT_WEB_VIEW_HOOK_INVOKED
		 * intArray[3] - Hook type.
		 * intArray[4] - Handle to url data.
		 *
		 */

		// Allocate the widget event data structure.
		// maGetEvent will handle the deallocation of this memory
		MAWidgetEventData *widgetEvent = new MAWidgetEventData();

		// Temporary variable for readability (?).
		int widgetEventType = intArray[1];

		widgetEvent->eventType = widgetEventType;
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
		else if (widgetEventType == MAW_EVENT_WEB_VIEW_CONTENT_LOADING)
		{
			widgetEvent->status = intArray[3];
		}
		else if (widgetEventType == MAW_EVENT_WEB_VIEW_HOOK_INVOKED)
		{
			widgetEvent->hookType = intArray[3];
			widgetEvent->urlData = intArray[4];
		}

		event.data = (int)widgetEvent;
	}
	else if (event.type == EVENT_TYPE_SENSOR)
	{
		event.sensor.type = intArray[1];
		memcpy( event.sensor.values, intArray + 2, (arrayLength - 2) * sizeof(jint) );
	}
	else if (event.type == EVENT_TYPE_NFC_TAG_RECEIVED ||
			event.type == EVENT_TYPE_NFC_TAG_DATA_READ ||
			event.type == EVENT_TYPE_NFC_TAG_DATA_WRITTEN ||
			event.type == EVENT_TYPE_NFC_BATCH_OP ||
			event.type == EVENT_TYPE_NFC_TAG_AUTH_COMPLETE ||
			event.type == EVENT_TYPE_NFC_TAG_READ_ONLY)
	{
		event.nfc.handle = intArray[1];
		event.nfc.result = intArray[2];
		event.nfc.dstId = intArray[3];
	}

	// Release the memory used for the int array.
	env->ReleaseIntArrayElements(eventBuffer, intArray, 0);

	Base::gSyscall->postEvent(event);
}

/**
 * This function now uses the JavaVM object to get the current thread.
 * Please see the links below and file SyscallImpl.cpp:
 * http://www.netmite.com/android/mydroid/1.6/dalvik/docs/jni-tips.html
 * http://www.milk.com/kodebase/dalvik-docs-mirror/docs/jni-tips.html (same as above?)
 * http://books.google.se/books?id=8M3F_sSSvWkC&pg=PA103&lpg=PA103&dq=JavaVM+obtain+the+JNIEnv+pointer&source=bl&ots=QlZ8PhF_dl&sig=9P_s_GGaN6jmWHkwokMvPAOadtQ&hl=sv&ei=kPAJToLnDsjPsga7kMjTDg&sa=X&oi=book_result&ct=result&resnum=4&ved=0CD4Q6AEwAw#v=onepage&q=JavaVM%20obtain%20the%20JNIEnv%20pointer&f=false
 * http://www.scribd.com/doc/57157456/1430226471#ad_unit=Doc_Sideboard_MediumRectangle_BTF_300x250&url=http%3A//www.scribd.com/doc/57157456/1430226471&attributes=QuantSegs%3DD%26IABParent%3DTechnology%2520%2526%2520Computing%26Extension%3Dpdf%26AdLayout%3D-1472436212%26DocUser%3D79236763%26IABChild%3DJava%252CShareware/Freeware%252C3-D%2520Graphics%252CAntivirus%2520Software%26Document%3D57157456%26FourGen%3DTrue&skip=54
 */
static int nativeCreateBinaryResource(
	JNIEnv* env,
	jobject jthis,
	int resourceIndex,
	int size)
{
	int result = Base::gSyscall->loadBinaryStore(resourceIndex, size);
	return result;
}

/**
 * TODO: This function also has the JNIEnv fix.
 */
static int nativeCreatePlaceholder( JNIEnv* env, jobject jthis )
{
	int result = maCreatePlaceholder();

	return result;
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

jint gNumJavaMethods = 8;
static JNINativeMethod sMethods[] =
{
	// name, signature, funcPtr
	{ "nativeInitRuntime", "()Z", (void*)nativeInitRuntime },
	{ "nativeLoad", "(Ljava/io/FileDescriptor;JLjava/io/FileDescriptor;J)Z", (void*)nativeLoad },
	{ "nativeLoadResource", "(Ljava/nio/ByteBuffer;)Z", (void*)nativeLoadResource },
	{ "nativeLoadCombined", "(Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;", (void*)nativeLoadCombined },
	{ "nativeRun", "()V", (void*)nativeRun },
	{ "nativePostEvent", "([I)V", (void*)nativePostEvent },
	{ "nativeCreateBinaryResource", "(II)I", (void*)nativeCreateBinaryResource },
	{ "nativeCreatePlaceholder", "()I", (void*)nativeCreatePlaceholder }
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

	//Load the JavaVirtual Machine to have access to differentJNI environments
	Base::gSyscall->setJavaVM(vm);

	jniRegisterNativeMethods(
		env,
		"com/mosync/internal/android/MoSyncThread",
		sMethods,
		gNumJavaMethods);

	return JNI_VERSION_1_4;
}

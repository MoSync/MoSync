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

//(#define SYSLOG(a) __android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", a);
#define SYSLOG(...)

/*
Function that initializes the native core	
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

/*
	/return The newly created Data Section as a Direct ByteBuffer object
*/
static jboolean nativeLoad(JNIEnv* env, jobject jthis, jobject program, jlong programOffset, jobject resource, jlong resourceOffset)
{
	SYSLOG("load program and resource");

	FILE* prg = NULL;
	FILE* res = NULL;
	
	SYSLOG("get program file");
	jclass fdClass = env->FindClass("java/io/FileDescriptor");
	if (fdClass != NULL) 
	{
		jclass fdPrgClassRef = (jclass) env->NewGlobalRef(fdClass); 
		jfieldID fdClassDescriptorFieldID = env->GetFieldID(fdPrgClassRef, "descriptor", "I");
		
		if (fdClassDescriptorFieldID != NULL && program != NULL) 
		{			
			jint fd = env->GetIntField(program, fdClassDescriptorFieldID);	
			int myfd = dup(fd);
			prg = fdopen(myfd, "rb");
			fseek(prg, programOffset, SEEK_SET); 
		}
	}
	
	SYSLOG("get resource file");
	jclass fdClass2 = env->FindClass("java/io/FileDescriptor");
	if (fdClass2 != NULL) 
	{
		jclass fdResClassRef = (jclass) env->NewGlobalRef(fdClass2); 
		jfieldID fdClassDescriptorFieldID = env->GetFieldID(fdResClassRef, "descriptor", "I");
		
		if (fdClassDescriptorFieldID != NULL && resource != NULL) 
		{			
			jint fd = env->GetIntField(resource, fdClassDescriptorFieldID);	
			int myfd = dup(fd);
			res = fdopen(myfd, "rb"); 
			fseek(res, resourceOffset, SEEK_SET);
		}
	}
	
	SYSLOG("both files was obtained!");
	
	if(NULL == prg || NULL == res)
	{
		SYSLOG("seams to be something wrong here..");
		return false;
	}
	
	SYSLOG("set jni environment");
	
	gCore->mJniEnv = env; gCore->mJThis = jthis;
	Base::gSyscall->setJNIEnvironment(env, jthis);
	
	SYSLOG("call load vm app");

	if(NULL == gCore) __android_log_write(ANDROID_LOG_INFO,"JNI","gCore == NULL");
	SYSLOG("gCore!");
	if(NULL == prg) __android_log_write(ANDROID_LOG_INFO,"JNI","prg == NULL");
	SYSLOG("prg!");
	if(NULL == res) __android_log_write(ANDROID_LOG_INFO,"JNI","res == NULL");
	SYSLOG("res!");
	
	return Core::LoadVMApp(gCore, prg, res);
}

/*
*/
static jboolean nativeLoadResource(JNIEnv* env, jobject jthis, jobject resource)
{
	SYSLOG("load resource");

	char* resourceBuffer = (char*)env->GetDirectBufferAddress(resource);
	
}

/*
	/return The newly created Data Section as a Direct ByteBuffer object
*/
static jobject nativeLoadCombined(JNIEnv* env, jobject jthis, jobject combined)
{
	SYSLOG("load combined file");

	char* combinedBuffer = (char*)env->GetDirectBufferAddress(combined);
	
	//gCore->jniEnv = env;
	// do the thing
	
	return gCore->mem_ds_jobject;
}

/*
	/return The newly created Data Section as a Direct ByteBuffer object
*/
static void nativeRun(JNIEnv* env, jobject jthis)
{
	__android_log_write(ANDROID_LOG_INFO,"JNI","native run");
	
	Base::gSyscall->setJNIEnvironment(env, jthis);
	
	while(1)
	{
		Core::Run2(gCore);
		
/*
		if(gReloadHandle > 0) {
			Base::Stream* stream = Base::gSyscall->resources.extract_RT_BINARY(gReloadHandle);
			bool res = Core::LoadVMApp(gCore, *stream);
			delete stream;
			gReloadHandle = 0;
			if(!res) {
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
			}
		}
*/		
	}
	
}

static void nativePostEvent(JNIEnv* env, jobject jthis, jintArray eventBuffer)
{
	SYSLOG("JNI PostEvent");
	
	jsize len = env->GetArrayLength(eventBuffer);
	jint *intArray = env->GetIntArrayElements(eventBuffer, 0);
	
	// rebuild the event
	MAEvent event;
	
	event.type = intArray[0];
	
	if(event.type == EVENT_TYPE_POINTER_PRESSED || event.type == EVENT_TYPE_POINTER_RELEASED || event.type == EVENT_TYPE_POINTER_DRAGGED )
	{
		event.point.x = intArray[1];
		event.point.y = intArray[2];
	}
	else if(event.type == EVENT_TYPE_KEY_RELEASED || event.type == EVENT_TYPE_KEY_PRESSED )
	{
		event.key = intArray[1];
		event.nativeKey = intArray[2];
	}
	else if(event.type == EVENT_TYPE_CONN)
	{
		event.conn.handle = intArray[1];
		event.conn.opType = intArray[2];
		event.conn.result = intArray[3];
	}
	
	// release the memory used
	env->ReleaseIntArrayElements( eventBuffer, intArray, 0);
	
	Base::gSyscall->postEvent(event);
}

int jniRegisterNativeMethods( JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods )
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
		return -1;
	}
	return 0;
}

jint numJavaMethods = 6;
static JNINativeMethod sMethods[] =
{
    // name, signature, funcPtr 
   { "nativeInitRuntime", "()Z", (void*)nativeInitRuntime}, 
   { "nativeLoad", "(Ljava/io/FileDescriptor;JLjava/io/FileDescriptor;J)Z", (void*)nativeLoad},
   { "nativeLoadResource", "(Ljava/nio/ByteBuffer;)Z", (void*)nativeLoadResource},
   { "nativeLoadCombined", "(Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;", (void*)nativeLoadCombined},
   { "nativeRun", "()V", (void*)nativeRun},
   { "nativePostEvent", "([I)V", (void*)nativePostEvent},
};

jint JNI_OnLoad ( JavaVM* vm, void* reserved )
{
	JNIEnv* env = NULL;
	jint result = -1;

	SYSLOG("JNI_OnLoad");
	
	SYSLOG("Check JNI version");

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	{
		return result;
	}
	
	jniRegisterNativeMethods( env, "com/mosync/internal/android/MoSyncThread", sMethods, numJavaMethods );
	return JNI_VERSION_1_4;
}

/* Copyright (C) 2010 Mobile Sorcery AB

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

#include <android/log.h>

/*
Function that initializes the native core	
*/
static jboolean nativeInitRuntime(JNIEnv* env, jobject jthis)
{
	__android_log_write(ANDROID_LOG_INFO,"JNI","InitRuntime start");

	Base::Syscall *syscall = 0;
	syscall = new Base::Syscall();

	gCore = Core::CreateCore(*syscall);
}

/*
	/return The newly created Data Section as a Direct ByteBuffer object
*/
static jboolean nativeLoad(JNIEnv* env, jobject jthis, jobject program, int programSize, jobject resource, int resourceSize)
{
	__android_log_write(ANDROID_LOG_INFO,"JNI","load program and resource");
	
	char* programBuffer = (char*)env->GetDirectBufferAddress(program);
	if(programBuffer == NULL)
	{
		__android_log_write(ANDROID_LOG_INFO,"JNI","whops.. no program buffer");
		return false;
	}
	
	char* resourceBuffer = (char*)env->GetDirectBufferAddress(resource);
	if(resourceBuffer == NULL)
	{
		__android_log_write(ANDROID_LOG_INFO,"JNI","whops.. no resource buffer");
		return false;
	}

	__android_log_write(ANDROID_LOG_INFO,"JNI","call load vm app");
	
	gCore->mJniEnv = env; gCore->mJThis = jthis;
	Base::gSyscall->setJNIEnvironment(env, jthis);
	return Core::LoadVMApp(gCore, programBuffer, programSize, resourceBuffer, resourceSize);
}

/*
*/
static jboolean nativeLoadResource(JNIEnv* env, jobject jthis, jobject resource)
{
	__android_log_write(ANDROID_LOG_INFO,"JNI","load resource");

	char* resourceBuffer = (char*)env->GetDirectBufferAddress(resource);
	
}

/*
	/return The newly created Data Section as a Direct ByteBuffer object
*/
static jobject nativeLoadCombined(JNIEnv* env, jobject jthis, jobject combined)
{
	__android_log_write(ANDROID_LOG_INFO,"JNI","load combined file");

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
	__android_log_write(ANDROID_LOG_INFO, "JNI Syscalls", "JNI PostEvent");
	
	jsize len = env->GetArrayLength(eventBuffer);
	jint *intArray = env->GetIntArrayElements(eventBuffer, 0);
	
	// rebuild the event
	MAEvent event;
	event.type = intArray[0];
	event.point.x = intArray[1];
	event.point.y = intArray[2];
		
	// release the memory used
	env->ReleaseIntArrayElements( eventBuffer, intArray, 0);
	
	Base::gSyscall->postEvent(event);
}

int jniRegisterNativeMethods( JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods )
{
	jclass clazz;

	clazz = env->FindClass(className);
	if (clazz == NULL)
	{
		__android_log_write(ANDROID_LOG_INFO, "JNI_OnLoad", "Class not found");
		return -1;
	}
	__android_log_write(ANDROID_LOG_INFO, "JNI_OnLoad", "Class found!");
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
   { "nativeLoad", "(Ljava/nio/ByteBuffer;ILjava/nio/ByteBuffer;I)Z", (void*)nativeLoad},
   { "nativeLoadResource", "(Ljava/nio/ByteBuffer;)Z", (void*)nativeLoadResource},
   { "nativeLoadCombined", "(Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;", (void*)nativeLoadCombined},
   { "nativeRun", "()V", (void*)nativeRun},
   { "nativePostEvent", "([I)V", (void*)nativePostEvent},
};

jint JNI_OnLoad ( JavaVM* vm, void* reserved )
{
	JNIEnv* env = NULL;
	jint result = -1;

	__android_log_write(ANDROID_LOG_INFO, "JNI_OnLoad", "Check JNI version");

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	{
		return result;
	}
	__android_log_write(ANDROID_LOG_INFO, "JNI_OnLoad", "Register native methods");
	jniRegisterNativeMethods( env, "com/mosync/java/android/MoSyncThread", sMethods, numJavaMethods );
	return JNI_VERSION_1_4;
}

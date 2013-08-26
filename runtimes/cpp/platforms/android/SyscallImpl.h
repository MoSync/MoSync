/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

class Syscall {

private:
	/*
	* Load an image and store it at the given resourceIndex
	*/
	bool loadImage(int resourceIndex, int pos, int length, int binaryResourceHandle = 0);
	Image* loadSprite(void* surface, ushort left, ushort top, ushort width, ushort height, ushort cx, ushort cy);
	void loadUBinary(int resourceIndex, int offset, int size);

public:
	JNIEnv* getJNIEnvironment();
	jobject getJNIThis();

	/**
	* returns the current JavaVM
	*/
	JavaVM* getJavaVM();

	/**
	* sets the current JavaVM
	* It is set in MoSync bridge when the onload function is called
	*/
	void setJavaVM(JavaVM* jvm);
public:

	Syscall();

	int loadBinaryStore(int resourceIndex, int size);
	char* loadBinary(int resourceIndex, int size);
	bool destroyBinaryResource(int resourceIndex);
	void destroyResource(int resourceIndex);
	void setJNIEnvironment(JNIEnv* je, jobject jthis);
	void postEvent(MAEvent event);
	// TODO: This does not seem to be used. Delete it?
	//void postEvent(MAEvent event, JNIEnv *jniEnv);
	int getEventQueueSize();
	void checkAndStoreAudioResource(int resourceIndex);

	int getReloadHandle();
	int setReloadHandle(MAHandle handle);
	bool isReloading();
	void setReloading(bool state);
	void resetSyscallState();

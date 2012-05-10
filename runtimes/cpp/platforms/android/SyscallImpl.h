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

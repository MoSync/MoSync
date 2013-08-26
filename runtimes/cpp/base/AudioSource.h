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

#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include "netImpl.h"

class MutexHandler {
public:
	MutexHandler(MoSyncMutex *mtx) : mMutex(mtx) {
		mMutex->lock();
	}

	~MutexHandler() { mMutex->unlock(); }

private:
	MoSyncMutex *mMutex;

};

class AudioSource {
public:
	enum Format {
		FMT_S8,
		FMT_U8,
		FMT_S16,
		FMT_U16
	};

	// must be filled by implementation
	struct Info {
		int numChannels;
		int sampleRate;
		Format fmt;
		int bitDepth;
		int bytesPerSample;
		int bufferSize;
		bool canSeek;
	};

	AudioSource();
	virtual ~AudioSource();

	virtual int init() = 0;
	virtual void close();

	// return 0 on end reached, return amount of bytes read otherwise.
	virtual int fillBuffer() = 0;
	virtual const void* getBuffer() const = 0;
	const Info& getInfo() const;

	virtual void setPosition(int ms) = 0;
	virtual int getPosition() const = 0;
	virtual int getLength() const = 0;

	virtual void setNumLoops(int i) = 0;
	virtual int getNumLoops() = 0;
protected:
	Info info;

	MoSyncMutex mMutex;

};

#endif /* _AUDIO_SOURCE_H_ */

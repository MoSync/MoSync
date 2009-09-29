/* Copyright (C) 2009 Mobile Sorcery AB

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

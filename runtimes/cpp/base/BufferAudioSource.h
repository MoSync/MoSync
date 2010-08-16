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

#ifndef _BUFFER_AUDIO_SOURCE_H_
#define _BUFFER_AUDIO_SOURCE_H_

#include "AudioSource.h"

struct MAAudioBufferInfo;
class MoSyncSemaphore;

typedef void (*BufferRequestCallback)(void);

class BufferAudioSource : public AudioSource {
public:
	BufferAudioSource(const MAAudioBufferInfo *i, BufferRequestCallback callback);
	virtual ~BufferAudioSource();
	int init();
	void close();
	void ready();
	int fillBuffer();
	const void* getBuffer() const;
	void setPosition(int ms);
	int getPosition() const;
	int getLength() const;
	void setNumLoops(int i);
	int getNumLoops();

private:
	void* mBuffer;

	MoSyncSemaphore *mSem;

	BufferRequestCallback mCallback;
};

#endif /* _BUFFER_AUDIO_SOURCE_H_ */ 

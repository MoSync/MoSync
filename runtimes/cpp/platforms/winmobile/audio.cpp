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

#include "config_platform.h"
#include "audio.h"
#include "base_errors.h"


using namespace MoSyncError;

Channel gChannels[MAX_SOUND_CHANNELS];
int gSoundVolume = 100;
using namespace Base;
CSoundServer soundServer;

/*
int readLEInt(Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<0)|((unsigned int)data[1]<<8)|((unsigned int)data[2]<<16)|((unsigned int)data[3]<<24);
}

unsigned int readLEUnsignedInt(Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<0)|((unsigned int)data[1]<<8)|((unsigned int)data[2]<<16)|((unsigned int)data[3]<<24);
}

short readLEShort(Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<0)|((unsigned short)data[1]<<8);
}

unsigned short readLEUnsignedShort(Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<0)|((unsigned short)data[1]<<8);
}

int readBEInt(Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<24)|((unsigned int)data[1]<<16)|((unsigned int)data[2]<<8)|((unsigned int)data[3]<<0);
}

unsigned int readBEUnsignedInt(Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<24)|((unsigned int)data[1]<<16)|((unsigned int)data[2]<<8)|((unsigned int)data[3]<<0);
}

short readBEShort(Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<8)|((unsigned short)data[1]<<0);
}

unsigned short readBEUnsignedShort(Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<8)|((unsigned short)data[1]<<0);
}

int readLEInt(byte *data) {
	return ((*data)<<0)|((*(data+1))<<8)|((*(data+2))<<16)|((*(data+3))<<24);
}

int readBEInt(byte *data) {
	return ((*data)<<24)|((*(data+1))<<16)|((*(data+2))<<8)|((*(data+3))<<0);
}

unsigned int readLEUnsignedInt(byte *data) {
	return ((*data)<<0)|((*(data+1))<<8)|((*(data+2))<<16)|((*(data+3))<<24);
}

unsigned int readBEUnsignedInt(byte *data) {
	return ((*data)<<24)|((*(data+1))<<16)|((*(data+2))<<8)|((*(data+3))<<0);
}

short readLEShort(byte *data) {
	return ((*data)<<0)|((*(data+1))<<8);
}

short readBEShort(byte *data) {
	return ((*(data))<<8)|((*(data+1))<<0);
}

unsigned short readLEUnsignedShort(byte *data) {
	return ((*data)<<0)|((*(data+1))<<8);
}

unsigned short readBEUnsignedShort(byte *data) {
	return ((*(data))<<8)|((*(data+1))<<0);
}
*/

// todo optimize innerloops, by making different versions
// returns false if end is reached
#define TEMP_BUF_SIZE (DEFAULT_DECODEBUF_BYTES*2)
byte tempBuf[TEMP_BUF_SIZE];

bool readPcm(Channel *chan, Stream *sample) {

	// calculate what we need
	/*
	int tempBufSizeNeeded = ((DEFAULT_DECODEBUF_BYTES>>2)*chan->bytesPerSample*chan->numChannels*chan->sampleRate)/(44100);
	
	// reallocate if we need a larger buffer
	if(tempBufSizeNeeded>tempBufSize) {
		if(tempBuf) delete tempBuf;
		tempBuf = new byte[tempBufSizeNeeded];
		tempBufSize = tempBufSizeNeeded;
	}
	*/

	int tempBufSizeNeeded = ((DEFAULT_DECODEBUF_SAMPLES*chan->sampleRate)/44100)*chan->bytesPerSample*chan->numChannels;
	int numBytes = MIN(tempBufSizeNeeded, (chan->numSampleBytes-chan->currentSampleByte));
	if(numBytes <= 0) {
		//TODO: add_sound_event
		return false;
	}
	//int tempBufSamplesNeeded = numBytes/(chan->bytesPerSample*chan->numChannels);
	int numSamplesLeft = MIN(DEFAULT_DECODEBUF_SAMPLES, ((chan->numSampleBytes-chan->currentSampleByte)/(chan->bytesPerSample*chan->numChannels)));
	if(numSamplesLeft <= 0) {
		//TODO: add_sound_event
		return false;
	}

	sample->read(tempBuf, numBytes);
	
	
	if(chan->sampleRate == 44100 && chan->bitsPerSample == 16 && chan->numChannels == 2 && chan->littleEndian == true && chan->hasUnsignedSamples == false) {
		chan->decodedPtr = tempBuf;
		chan->decodedBytes = numBytes;
		chan->currentSampleByte+=numBytes;
		chan->samplePosition+=numBytes>>2;
		return true;
	} else {
		chan->decodedPtr = chan->decodedBuffer;
		chan->decodedBytes = numSamplesLeft*2*2; // 16-bit, stereo
	}

	short *dst = (short*)chan->decodedBuffer;
	byte *src = tempBuf;

	int localSamplePosition = chan->samplePosition;

	for(int i = 0; i < numSamplesLeft /*DEFAULT_DECODEBUF_BYTES>>2*/; i++) {
		src = &tempBuf[(((chan->samplePosition-localSamplePosition)>>8)*chan->numChannels*chan->bytesPerSample)&(TEMP_BUF_SIZE-1)];

		switch(chan->bytesPerSample) {
			case 1:
				{
					if(chan->hasUnsignedSamples) {
						*dst++ = -32768 + ((*src)<<(16-chan->bitsPerSample));
						if(chan->numChannels==2) {
							*dst++ = -32768 + ((*(src+1))<<(16-chan->bitsPerSample));
						} else {
							*dst++ = *(dst-1);
						}
					} else {
						*dst++ = ((*src)<<(16-chan->bitsPerSample));
						if(chan->numChannels==2) {
							*dst++ = ((*(src+1))<<(16-chan->bitsPerSample));
						} else {
							*dst++ = *(dst-1);
						}
					}
				}
				break;
			case 2:
				{
					if(chan->hasUnsignedSamples) {
						if(chan->littleEndian) {
							*dst++ = -32768 + ((readLEShort(src))<<(16-chan->bitsPerSample));
							if(chan->numChannels==2) {
								*dst++ = -32768 + ((readLEShort(src+2))<<(16-chan->bitsPerSample));
							} else {
								*dst++ = *(dst-1);
							}
						} else {
							*dst++ = -32768 + ((readBEShort(src))<<(16-chan->bitsPerSample));
							if(chan->numChannels==2) {
								*dst++ = -32768 + ((readBEShort(src+2))<<(16-chan->bitsPerSample));
							} else {
								*dst++ = *(dst-1);
							}
						}
					} else {
						if(chan->littleEndian) {
							*dst++ = ((readLEShort(src))<<(16-chan->bitsPerSample));
							if(chan->numChannels==2) {
								*dst++ = ((readLEShort(src+2))<<(16-chan->bitsPerSample));
							} else {
								*dst++ = *(dst-1);
							}
						} else {
							*dst++ = ((readBEShort(src))<<(16-chan->bitsPerSample));
							if(chan->numChannels==2) {
								*dst++ = ((readBEShort(src+2))<<(16-chan->bitsPerSample));
							} else {
								*dst++ = *(dst-1);
							}
						}
					}
				}
				break;
		}

		chan->samplePosition+=chan->sampleRateDelta;

		//int lastSamplePosition = chan->samplePosition>>16;
		//chan->samplePosition+=chan->sampleRateDelta;
		//if((chan->samplePosition>>16) != lastSamplePosition) {
		//	src+=(1+stereoInc)<<(chan->bytesPerSample-1); // works with 1 and 2 bytes ;)
		//}
	}

/*
	if(((chan->samplePosition-localSamplePosition)&0xff)!=0) {
		chan->hasSpillSamples = true;
		chan->spillSamples[0] = 
		chan->spillSamples[1] = 
	}
*/


	chan->currentSampleByte += numBytes;

	// only supports 44100, 16-bit, stereo, LE (wave)
	//memcpy(chan->decodedPtr, tempBuf, numBytes);
	//chan->totalSamplePosition+=numBytes>>2;

	return true;
}

int readData(Channel *chan) {
	if (chan->stopped)           
		chan->decodedBytes = 0;

	Stream *sample = chan->sound;

	// don't need to decode more data just yet
	if (chan->decodedBytes  > 0)   
		return chan->decodedBytes;

	// decode more data and return if sound hasn't reached end (handled differently depending on sound format)
	switch(chan->st) {
		case MT_MP3:
			break;
		case MT_WAVE:
		case MT_AU:
				// read big chunk of pcm samples converting them to the approprate 
				// sample rate / bit per sample at the same time.
				if(readPcm(chan, sample)) 
					return chan->decodedBytes;
			break;
	}

	if (chan->numLoops==1) return(0);
	else if(chan->numLoops>1) { chan->numLoops--;}

	// rewind
	if(!sample->seek(Base::Seek::Start, chan->soundDataStart)) {
		// something went wrong :(
		return 0;
	}

	chan->currentSampleByte = 0;

	return readData(chan);
}

// 32-bit temporary mixing buffer (clamped after every channel have been mixed down)
int tempBuffer[DEFAULT_AUDIOBUF_SAMPLES];
void soundMixer(void *pbuf, long len) {
	byte *buf = (byte*)pbuf;
	short *src, *dst;
	int *mixDst;
	int bytesWritten, copySize, i, j; 
	bool silent = true;
	
	memset(tempBuffer, 0, DEFAULT_AUDIOBUF_SAMPLES*sizeof(int));

	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		bytesWritten = 0;
		copySize = 0;

		if(!gChannels[i].active||gChannels[i].stopped)
			continue;

		silent = false;

		while(bytesWritten<len)
		{
			if (!readData(&gChannels[i])) 
			{
				// all data have been read
				gChannels[i].stopped = 1;
				break;
			} 

			copySize = len - bytesWritten;
			if(copySize > gChannels[i].decodedBytes)
			{
				copySize = gChannels[i].decodedBytes;
			}

			if (copySize > 0)
			{
				src = (short*)gChannels[i].decodedPtr; // hard coded for 16 bit sound
				mixDst = (int*)&tempBuffer[(bytesWritten>>1)];
				for(j = 0; j < copySize>>1; j++)
				{
					mixDst[j] += (((int)src[j])*gChannels[i].volume)>>16;
				}
				bytesWritten += copySize;
				gChannels[i].decodedPtr += copySize;
				gChannels[i].decodedBytes -= copySize;
				gChannels[i].samplesRead += copySize>>1;
			} 
		}
	}
	
	if(!silent) {
		dst = (short*) buf;
		for(i = 0; i < len>>1; i++)
		{
			if(tempBuffer[i]<-32768) tempBuffer[i] = -32768;
			if(tempBuffer[i]>32767) tempBuffer[i] = 32767;
			dst[i] = (short) tempBuffer[i];
		}
	} else {
		memset(buf, 0, len);
	}
}

bool openAudio(int sampleRate, int bitDepth, int numChannels, int bufferSize, USER_CALLBACK callBack) {
	if(callBack == NULL) callBack = soundMixer;
	soundServer.open(sampleRate, bitDepth, numChannels, callBack, bufferSize);

	for(int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		gChannels[i].active = false;
		gChannels[i].sound = NULL;
		gChannels[i].stopped = 1;
		gChannels[i].numLoops = 1;
		gChannels[i].decodedPtr = NULL;
		gChannels[i].decodedBytes = 0;
	}
	return true;
}

void closeAudio() {
	soundServer.close();
	for(int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if(gChannels[i].sound) delete gChannels[i].sound;
	}
//	if(tempBuf) 
//		delete tempBuf;
}

bool readAuHeader(Channel *channel, Stream *sound) {
	char  id[4];
	sound->read(&id, 4);
	if(memcmp(id, ".snd", 4) != 0) return false;
	
	channel->soundDataStart = readBEUnsignedInt(sound);
	unsigned int dataLength = readBEUnsignedInt(sound);
	unsigned int encoding = readBEUnsignedInt(sound);

/*
				      1		8-bit ISDN u-law
				      2		8-bit linear PCM [REF-PCM]
				      3		16-bit linear PCM
				      4		24-bit linear PCM
				      5		32-bit linear PCM
				      6		32-bit IEEE floating point
				      7		64-bit IEEE floating point
				     23		8-bit ISDN u-law compressed
						using the CCITT G.721 ADPCM
						voice data encoding scheme.
*/
	channel->hasUnsignedSamples = false;
	switch(encoding) {
		case 2:
			channel->bitsPerSample = 8;
			channel->bytesPerSample = 1;
			break;
		case 3:
			channel->bitsPerSample = 16;
			channel->bytesPerSample = 2;
			break;
		default:
			// unsupported encoding for now
			return false;
			break;		
	}

	channel->sampleRate = readBEUnsignedInt(sound);;
	channel->numChannels = readBEUnsignedInt(sound);;

	if(dataLength == 0xffffffff) {
		int len;
		if(!sound->length(len))
			return false;
		dataLength = len;
		dataLength-=channel->soundDataStart;
	} 
		
	channel->numSampleBytes = dataLength/*/(channel->bytesPerSample*channel->numChannels)*/;
	channel->littleEndian = false;

	if(!sound->seek(Base::Seek::Start, channel->soundDataStart)) 
		return false;

	return true;
}

bool readWaveHeader(Channel *channel, Stream *sound) {
	struct chunk_hdr {
		char  id[4];
		DWORD len;
	};

	chunk_hdr chnk;

	sound->read(&chnk, sizeof(chunk_hdr));
	if(memcmp(chnk.id, "RIFF", 4) != 0) return false;
	sound->read(chnk.id, 4);
	if(memcmp(chnk.id, "WAVE", 4) != 0) return false;

	bool foundFMT = false, foundDATA = false;

	while(1) {
		if(!sound->read(chnk.id, 4)) break;
		chnk.len = readLEInt(sound);
		if(memcmp(chnk.id, "fmt ", 4) == 0) {
			foundFMT = true;

			struct fmt_header {
				short          wFormatTag;
				unsigned short wChannels;
				unsigned long  dwSamplesPerSec;
				unsigned long  dwAvgBytesPerSec;
				unsigned short wBlockAlign;
				unsigned short wBitsPerSample;
			};
			
			fmt_header fmt;
			fmt.wFormatTag			= readLEShort(sound);
			fmt.wChannels			= readLEUnsignedShort(sound);
			fmt.dwSamplesPerSec		= readLEUnsignedInt(sound);
			fmt.dwAvgBytesPerSec	= readLEUnsignedInt(sound);
			fmt.wBlockAlign			= readLEUnsignedShort(sound);
			fmt.wBitsPerSample		= readLEUnsignedShort(sound);

			if(fmt.wFormatTag != 1) return false; // unsupported compression
			
			channel->numChannels = fmt.wChannels;
			channel->sampleRate = fmt.dwSamplesPerSec;
			channel->bitsPerSample = fmt.wBitsPerSample;
			channel->bytesPerSample = fmt.wBlockAlign / fmt.wChannels;

			// <= 8-bit pcm wave is unsigned / > 8-bit pcm wave is signed
			if(fmt.wBitsPerSample<9) channel->hasUnsignedSamples = true;
			else channel->hasUnsignedSamples = false;

		} else if(memcmp(chnk.id, "data", 4) == 0) {
			foundDATA = true;
			channel->numSampleBytes = chnk.len/*(channel->bytesPerSample*channel->numChannels)*/;
			if(!sound->tell(channel->soundDataStart)) return false;
			break;
		} else {
			sound->seek(Base::Seek::Current, chnk.len);
		}
	}

	channel->littleEndian = true;

	return foundFMT&&foundDATA; 
}

bool initChannel(int chan, Stream* sound, MimeType st) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==true)
	{
		freeChannel(chan);
	}
	gChannels[chan].st = st;
	gChannels[chan].sound = sound;
	gChannels[chan].active = true;
	gChannels[chan].numLoops = 1;
	setVolume(chan, gSoundVolume);
	
	switch(st) {
		case MT_AU:
				if(!readAuHeader(&gChannels[chan], sound)) {
					BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
				}
			break;
		case MT_WAVE:
				if(!readWaveHeader(&gChannels[chan], sound)) {
					BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
				}
			break;
	}
	
	gChannels[chan].sampleRateDelta = ((gChannels[chan].sampleRate<<8)/(44100));
	gChannels[chan].currentSampleByte = 0;
	gChannels[chan].samplePosition = 0;
	return true;
}

void freeChannel(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active == false) return; //BIG_PHAT_ERROR?
	gChannels[chan].stopped = true;
	gChannels[chan].active = false;
	if(gChannels[chan].sound!=NULL)
	{
		gChannels[chan].sound = NULL;
	}
}

void stopChannel(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	gChannels[chan].stopped = true;	
}

void startChannel(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	gChannels[chan].stopped = false;	
}

void setPosChannel(int chan, int ms) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);

}

int getPosChannel(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return gChannels[chan].samplesRead;
}

int getNumChannels() {
	return MAX_SOUND_CHANNELS;
}

int getVolume(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	//if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return (gChannels[chan].volume*100)>>16;
}

void setVolume(int chan, int volume) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	//if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	if(volume<0) volume = 0;
	if(volume>100) volume = 100;
	gSoundVolume = volume;
	gChannels[chan].volume = (volume*0xffff)/100;
}

bool soundIsPlaying(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return gChannels[chan].stopped==0;
}

int getLength(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return -1;
}

void setLoopCount(int chan, int count) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	MYASSERT(count != 0, ERR_SOUND_LOOP_COUNT_INVALID);
	gChannels[chan].numLoops = count;
}

void rewindChannel(int chan){
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	gChannels[chan].sound->seek(Base::Seek::Start, gChannels[chan].soundDataStart);
}

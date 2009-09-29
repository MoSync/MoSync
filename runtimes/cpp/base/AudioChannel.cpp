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

#include <cstdlib>
#include "AudioChannel.h"
#include "AudioSource.h"



/**
 * Constructor with initial audio source
 *
 * @param s             The sample rate of the output
 * @param audioSource   Pointer to an audio source, default is NULL
 */
AudioChannel::AudioChannel ( int s, AudioSource* audioSource )
: mActive( false ),
  mVolume( 0xffff ),
  mOutputSampleRate( s ),
  mAudioSource( audioSource ),
  mBufferedSamples( 0 ),
  mBufferedSamplePos( 0 )
{
}

/**
 * Set the channel audio source
 *
 * @param as    Pointer to an audio source
 */
void AudioChannel::setAudioSource ( AudioSource* as )
{
    mAudioSource = as;
}

/**
 * Returns the current audio source
 *
 * @return Pointer to an audio source
 */
AudioSource* AudioChannel::getAudioSource ( void )
{
    return mAudioSource;
}

/**
 * Sets whether or not the channel is active, that is
 * if it is paused or not.
 *
 * @param a     Active or not, true/false, default is true
 */
void AudioChannel::setActive ( bool a )
{
    mActive = a;
}

/**
 * Returns wether the channel is active or not.
 *
 * @return true if active, false if not
 */
bool AudioChannel::isActive ( void ) const
{
    return mActive;
}

/**
 * Sets the channel volume
 *
 * @param vol   The volume represented as [0,1) in 16.16
 *              fixed point.
 */
void AudioChannel::setVolume ( int vol )
{
    mVolume = vol;
}

/**
 * Returns the channel volume
 *
 * @return The returned value in the range [0,1) in 16.16
 *         fixed point.
 */
int AudioChannel::getVolume ( void )
{
	return mVolume;
}



/**
 * This is a template for on the fly converting and mixing from
 * any source (8/16/mono/stereo) format to 32 bit signed stereo
 *
 * @param dst           Pointer to the destination buffer
 * @param numSamples    Number of samples to mix
 * @param src           Pointer to the source buffer
 * @param pos           The position in the source buffer to start (24.8 FIXP)
 * @param delta         The increment delta (24.8 FIXP)
 * @param vol           The source volume [0-1) (16.16 FIXP)
 */
template< typename SrcT, int srcBitDepth, int srcNumChannels, bool sign >
void convert_audio ( int *dst, int numSamples, const SrcT *src,
                     int pos, int delta, int vol )
{
    int sample;

    for( int i = 0; i < numSamples; i++ )
    {
        sample = (int)src[((pos>>8)<<(srcNumChannels>>1))];
        if ( sign == false )
            sample -= ((1<<srcBitDepth)>>1);
        sample <<= 16-srcBitDepth;
        sample = (sample*vol)>>16;
        dst[i*2+0] += sample;

        if ( srcNumChannels == 2 )
        {
            sample = (int)src[((pos>>8)<<1)+1];
            if ( sign == false)
                sample -= ((1<<srcBitDepth)>>1);
            sample <<= 16-srcBitDepth;
            sample = (sample*vol)>>16;
            dst[i*2+1] += sample;
        }
        else
            dst[i*2+1] += sample;

        pos+=delta;
    }
}



/**
 * Converts and mixes the audio source to the internal buffer
 *
 * @param dst           Pointer to the internal buffer to mix in to.
 * @param numSamples    The number of samples to mix.
 *
 */
void AudioChannel::mix ( int *dst, int numSamples )
{
    if ( mAudioSource == NULL || mActive == false )
        return;

    int samplesWritten = 0;
    int srate   = mAudioSource->getInfo().sampleRate;
    int delta   = (srate<<8)/mOutputSampleRate; // amount of src samples per dst sample (in 24:8 fixed point)
    int snc     = mAudioSource->getInfo().numChannels;
    //int numSourceSamples = srate*numSamples/mOutputSampleRate; //

    AudioSource::Format sfmt = mAudioSource->getInfo().fmt;
    

    const void *src = mAudioSource->getBuffer();

    while ( samplesWritten < numSamples )
    {
        if ( mBufferedSamples == false )
        {
            if ( (mBufferedSamples=mAudioSource->fillBuffer( )) == false )
            {
                mActive = false;
                break;
            }
            mBufferedSamplePos = 0;
        }

        int copySize = numSamples - samplesWritten;
        int scaledBufferedSamples = (mBufferedSamples*mOutputSampleRate)/srate;

        if ( copySize > scaledBufferedSamples )
            copySize = scaledBufferedSamples;

        if ( copySize > 0 )
        {
            switch ( sfmt )
            {
                case AudioSource::FMT_S8:
                    if ( snc == 1 )
                        convert_audio<char, sizeof(char)<<3, 1, true>( &dst[samplesWritten<<1], copySize, (char*)src, mBufferedSamplePos<<8, delta, mVolume );
                    else
                        convert_audio<char, sizeof(char)<<3, 2, true>( &dst[samplesWritten<<1], copySize, (char*)src, mBufferedSamplePos<<8, delta, mVolume );
                    break;
                case AudioSource::FMT_S16:
                    if( snc == 1 )
                        convert_audio<short, sizeof(short)<<3, 1, true>( &dst[samplesWritten<<1], copySize, (short*)src, mBufferedSamplePos<<8, delta, mVolume );
                    else
                        convert_audio<short, sizeof(short)<<3, 2, true>( &dst[samplesWritten<<1], copySize, (short*)src, mBufferedSamplePos<<8, delta, mVolume );
                    break;
                case AudioSource::FMT_U8:
                    if ( snc == 1 )
                        convert_audio<unsigned char, sizeof(char)<<3, 1, false>( &dst[samplesWritten<<1], copySize, (unsigned char*)src, mBufferedSamplePos<<8, delta, mVolume);
                    else
                        convert_audio<unsigned char, sizeof(char)<<3, 2, false>( &dst[samplesWritten<<1], copySize, (unsigned char*)src, mBufferedSamplePos<<8, delta, mVolume);
                    break;
                case AudioSource::FMT_U16:
                    if ( snc == 1 )
                        convert_audio<unsigned short, sizeof(short)<<3, 1, false>( &dst[samplesWritten<<1], copySize, (unsigned short*)src, mBufferedSamplePos<<8, delta, mVolume );
                    else
                        convert_audio<unsigned short, sizeof(short)<<3, 2, false>( &dst[samplesWritten<<1], copySize, (unsigned short*)src, mBufferedSamplePos<<8, delta, mVolume );
                    break;
            }

            int scaledCopySize = (copySize*delta)>>8;
            if ( scaledCopySize == 0 )
                scaledCopySize = 1;

            samplesWritten      += copySize;
            mBufferedSamplePos  += scaledCopySize;
            mBufferedSamples    -= scaledCopySize;
        }
    }
}

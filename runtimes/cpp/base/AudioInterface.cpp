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

/*
 * File:   AudioInterface.cpp
 * Author: Ali Mosavian
 *
 * Created on July 16, 2009
 */
#include <cstdlib>
#include "helpers/types.h"
#include "mostl/algorithm"
#include "config_platform.h"
#include "AudioChannel.h"
#include "AudioInterface.h"
#include "thread/lock.hpp"
#include "thread/mutexfactory.hpp"
#include "allocationfailedexception.hpp"
#ifdef  __SDL__
#include "../platforms/sdl/AudioInterfaceSDL.h"
#elif defined(__WINMOBILE__)
#include "../platforms/winmobile/AudioInterfaceWinMob.h"
#else
    //#warning "No platform has been defined!"
#endif

using namespace Base;
using namespace Base::Thread;

/**
 * Initialization of static variables
 *
 */
AudioInterface* AudioInterface::m_instance = NULL;
int AudioInterface::m_audioBuffer[AUDIO_BUF_SAMPLES*2];


/**
 * Constructor, the implementation should open the audio
 * output. The interface creates the mutex.
 *
 * @param r     The sample rate of the output
 * @param c     Number of channels on the output
 * @param b     Bitrate of the output
 * @param s     Output is signed (true)/unsigned (false)
 *
 * @throws AudioOutputException if there's a failure in trying
 *         to open the audio output
 */
AudioInterface::AudioInterface ( int r, int c, int b, bool s )
{
    // Set fields
    m_outputChannels    = c;
    m_outputSampleBits  = b;
    m_outputSampleRate  = r;
	m_outputSigned		= s;

    // Allocate mutex
    m_mutex = MutexFactory::getInstance( )->createMutex( );
}

/**
 * The destructor, the implementation should close and
 * delete all live channels, delete all the audio sources
 * and then close the audio output. The destructor on the
 * interface will set the instance to NULL.
 *
 * Note: This method (delete on the instance) should
 *       only be called from MALibQuit
 */
AudioInterface::~AudioInterface ( void )
{
    m_instance = NULL;
    delete m_mutex;
}


/**
 * Removes a channel from the active channel list. This is so
 * that the constructor doesn't try to delete that channel. This
 * method is to be called from the destructor of AudioChannel.
 *
 * @param c     Pointer to the channel to remove.
 */
void AudioInterface::removeChannel ( AudioChannel *c )
{
    Lock l( m_mutex );

    m_activeChanList.remove( c );
}

/**
 * Deletes all the channels in the active channel list.
 * This method is to be called from the implementations
 * destructor AFTER closing the audio output. It alters
 * the active channel list and is thread safe.
 *
 */
void AudioInterface::deleteChannels ( void )
{
    Lock l( m_mutex );

    while ( m_activeChanList.empty( ) == false )
        delete m_activeChanList.front( );
}


/**
 * Creates a new channel which will be mixed in to the
 * output while its active.
 *
 * @return Pointer to the newly created AudioChannel
 *
 * @throws AllocationFailedException if there wasn't
 *         enough memory to create the channel.
 */
AudioChannel* AudioInterface::createChannel ( void )
{
    AudioChannel *c;

    try {
        c = new AudioChannel( m_outputSampleRate );
    } catch ( std::exception e ) {
        throw Base::AllocationFailedException( e.what( ) );
    }

    Lock l( m_mutex );
    m_activeChanList.push_back( c );
    return c;
}


/**
 * Returns the sampe rate of the output
 *
 * @return Sample rate as integer
 */
int AudioInterface::getSampleRate ( void )
{
    return m_outputSampleRate;
}


/**
 * Returns the number of channels on the output.
 *
 * @return Number of channels as integer
 */
int AudioInterface::getChannels ( void )
{
    return m_outputChannels;
}


/**
 * Returns the bitrate of the output
 *
 * @return Bitrate as integer
 */
int AudioInterface::getBitrate ( void )
{
    return m_outputSampleBits;
}





/**
 * This is a template for on the fly converting and mixing from
 * 32 bit signed stereo to any output (8/16/mono/stereo) format.
 *
 * @param dst           Pointer to the destination buffer
 * @param src           Pointer to the source buffer
 * @param numSamples    Number of samples to mix
 * @param vol           Master volume [0-1) (16.16 FIXP)
 */
template< typename dst_t, int dst_bitdepth, int dst_chan, bool sign >
void convert_audio ( dst_t *dst, const int *src, int numSamples, int vol )
{
    int samp;
    int lSamp, rSamp;

    for( int i = 0; i < numSamples; i++ )
    {
        // Fetch and adjust master volume
        lSamp = (src[i*2+0]*vol)>>16;
        rSamp = (src[i*2+1]*vol)>>16;

        if ( dst_chan == 1 )
        {
            // Clamp
            lSamp = mostd::clamp<int>( -32768, lSamp, 32767 );
            rSamp = mostd::clamp<int>( -32768, rSamp, 32767 );

            // Convert to unsigned ?
            if ( sign == false )
            {
                lSamp += ((1<<dst_bitdepth)>>1);
                rSamp += ((1<<dst_bitdepth)>>1);
            }

            // Down sample bitdepth?
            if ( dst_bitdepth < 16 )
            {
                lSamp >>= 16-dst_bitdepth;
                rSamp >>= 16-dst_bitdepth;
            }
            else if ( dst_bitdepth > 16 )
            {
                lSamp <<= dst_bitdepth-16;
                rSamp <<= dst_bitdepth-16;
            }

            // Store
            dst[i*2+0] = (dst_t)lSamp;
            dst[i*2+1] = (dst_t)rSamp;
        }
        else
        {
            // Clamp
            samp = mostd::clamp<int>( -32768, lSamp+rSamp, 32767 );

            // Convert to unsigned ?
            if ( sign == false )
                samp += ((1<<dst_bitdepth)>>1);

            // Down sample bitdepth?
            if ( dst_bitdepth < 16 )
                samp >>= 16-dst_bitdepth;
            else if ( dst_bitdepth > 16 )
                samp <<= dst_bitdepth-16;

            // Store
            dst[i] = (dst_t)samp;
        }
    }
}

/**
 * The callback used by the audio output.
 * Note: The implementation should invoke this callback when it
 *       is time to mix more samples. The method assumes that
 *       the output is 16 bit litte-endian, stereo (for now).
 *
 * @param b     Pointer to the sound buffer to fill
 * @param l     Number of bytes to fill
 */
void AudioInterface::outputCallback ( void *b, long len )
{
    Lock    lck( m_mutex );
    int     numSamples = len/((m_outputSampleBits/8)*m_outputChannels);

    // Reset audio buffer
    memset( m_audioBuffer, 0, AUDIO_BUF_SAMPLES*2*sizeof( int ) );

    // Mix in all active channels
    std::list<AudioChannel *>::iterator it;
    for ( it = m_activeChanList.begin( ); it != m_activeChanList.end( ); ++it )    
        (*it)->mix( m_audioBuffer, numSamples );

    // Convert to output format
#ifndef __SOUND_OUTPUT_USE_CONVERSION__
    s16 *out = static_cast<s16 *>( b );
    for ( int i = 0; i < numSamples; i++ )
    {
        out[i*2+0] = (s16)mostd::clamp<int>( -32768, m_audioBuffer[i*2+0], 32767 );
        out[i*2+1] = (s16)mostd::clamp<int>( -32768, m_audioBuffer[i*2+1], 32767 );
    }
#else
    switch ( m_outputSampleBits )
    {
        case 8:
            if ( m_outputSigned == false )
            {
                if ( m_outputChannels == 1 )
                    convert_audio<s8, 8, 1, false>( (s8 *)b, m_audioBuffer, numSamples, 0xffff );
                else
                    convert_audio<s8, 8, 2, false>( (s8 *)b, m_audioBuffer, numSamples, 0xffff );
            }
            else
            {
                if ( m_outputChannels == 1 )
                    convert_audio<u8, 8, 1, true>( (u8 *)b, m_audioBuffer, numSamples, 0xffff );
                else
                    convert_audio<u8, 8, 2, true>( (u8 *)b, m_audioBuffer, numSamples, 0xffff );
            }
            break;

        case 16:
            if ( m_outputSigned == false )
            {
                if ( m_outputChannels == 1 )
                    convert_audio<u16, 16, 1, false>( (u16 *)b, m_audioBuffer, numSamples, 0xffff );
                else
                    convert_audio<u16, 16, 2, false>( (u16 *)b, m_audioBuffer, numSamples, 0xffff );
            }
            else
            {
                if ( m_outputChannels == 1 )
                    convert_audio<s16, 16, 1, true>( (s16 *)b, m_audioBuffer, numSamples, 0xffff );
                else
                    convert_audio<s16, 16, 2, true>( (s16 *)b, m_audioBuffer, numSamples, 0xffff );
            }
            break;
    #ifdef __SOUND_OUTPUT_24BIT_SUPPORTED__
        case 24:
            if ( m_outputSigned == false )
            {
                if ( m_outputChannels == 1 )
                    convert_audio<u32, 24, 1, false>( (u32 *)b, m_audioBuffer, numSamples, 0xffff );
                else
                    convert_audio<u32, 24, 2, false>( (u32 *)b, m_audioBuffer, numSamples, 0xffff );
            }
            else
            {
                if ( m_outputChannels == 1 )
                    convert_audio<s32, 24, 1, true>( (s32 *)b, m_audioBuffer, numSamples, 0xffff );
                else
                    convert_audio<s32, 24, 2, true>( (s32 *)b, m_audioBuffer, numSamples, 0xffff );
            }
            break;
    #endif
        default:
            MYASSERT_EXTRA( false, MoSyncError::ERR_INTERNAL, "Sound output format not supported" );
    }
#endif
}


/**
 * Gets the instance of the correct platform implementation.
 *
 * Note: This method is not thread safe, so it should be
 *       called from the MALibInit( ) the first time
 *
 * @return Pointer to the correct instance of AudioInterface.
 *
 * @throws AudioOutputException if it fails to open the audio
 *         output. This is however only relevant the first time
 *         the method is invoked.
 */
AudioInterface* AudioInterface::getInstance ( void )
{
#ifdef  __SDL__
    return AudioInterfaceSDL::getInstance( );
#elif defined(__WINMOBILE__)
    return AudioInterfaceWinMob::getInstance( );
#else
    return NULL;
#endif
}

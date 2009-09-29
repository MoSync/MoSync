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
 * File:   AudioInterfaceWinMob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 16, 2009
 */
#include <cstring>
#include <iterator>
#include "config_platform.h"
#include "Stream.h"
#include "AudioInterfaceWinMob.h"
#include <thread/lock.hpp>
#include <AudioOutputException.h>

#ifndef FALSE
#define FALSE 0
#endif

using namespace Base::Thread;

bool AudioInterfaceWinMob::m_isActive = true;


/**
 * Constructor, Attempts to open the audio output.
 *
 * @param r     The sample rate of the output (def. 44100)
 * @param c     Number of channels on the output (def 2)
 * @param b     Bitrate of the output (def 16)
 *
 * @throws AudioOutputException if there's a failure in trying
 *         to open the audio output
 */
AudioInterfaceWinMob::AudioInterfaceWinMob ( int r, int c, int b )
: AudioInterface( r, c, b, true )
{
    // Open the sound output
    if ( m_soundOutput.open( r, b, c, callback,
                             AUDIO_BUF_SAMPLES*2*sizeof( short ) ) == FALSE )
    {
        throw Base::AudioOutputException( "Could not open audio output" );
    }

    //
    // Needed for callback to know that it should mix
    //
    m_isActive = true;
}

/**
 * The destructor, closes and delete all live channels,
 * deletes all the audio sources and then close the audio output.
 *
 * Note: This method (delete on the instance) should
 *       only be called from MALibQuit
 */
AudioInterfaceWinMob::~AudioInterfaceWinMob ( void )
{
    Lock l( m_mutex );

    m_isActive = false;
    m_soundOutput.close( );
    deleteChannels( );
}

/**
 * Creates a new audio source from a stream and its
 * mimetype.
 *
 * @param m     Asciiz string with the streams mimetype.
 * @param s     Pointer to the stream.
 *
 * @return Instance of AudioSource which represents the stream.
 *
 * @throws AllocationFailedException if it fails to allocate
 *         memory, or NotSupportedException when the mime type
 *         is not supported.
 */
AudioSource* AudioInterfaceWinMob::createAudioSource ( const char *m,
                                                       Base::Stream *s )
{
    return NULL;
}

/**
 * Gets the instance of the correct platform implementation.
 *
 * @return Pointer to the correct instance of AudioInterface.
 *
 * @throws AudioOutputException if it fails to open the audio
 *         output. This is however only relevant the first time
 *         the method is invoked.
 */
AudioInterface* AudioInterfaceWinMob::getInstance ( void )
{
    if ( m_instance == NULL )
        m_instance = new AudioInterfaceWinMob( );

    return m_instance;
}


/**
 * Stub used for the callback
 *
 * @param b     Pointer to output buffer
 * @param k     Samples to fill
 */
void AudioInterfaceWinMob::callback ( void* b, long l )
{
    if ( m_instance == NULL || m_isActive == false )
        return;

    static_cast<AudioInterfaceWinMob *>( m_instance )->outputCallback( b, l );
}
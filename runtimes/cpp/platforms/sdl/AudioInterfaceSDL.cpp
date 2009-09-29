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
 * File:   AudioInterfaceSDL.cpp
 * Author: Ali Mosavian
 *
 * Created on July 18, 2009
 */
#include "config_platform.h"
#include "AudioInterfaceSDL.h"
#include "thread/lock.hpp"
#include "AudioOutputException.h"

using namespace Base::Thread;

bool AudioInterfaceSDL::m_isActive;


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
AudioInterfaceSDL::AudioInterfaceSDL ( int r, int c, int b )
: AudioInterface( r, c, b, true )
{
    SDL_AudioSpec desired;

    // Open the sound output
    desired.freq    = r;
    desired.format  = (b == 8) ? AUDIO_S8 : AUDIO_S16;
    desired.channels= c;
    desired.samples = AUDIO_BUF_SAMPLES;
    desired.callback= callback;
    desired.userdata= NULL;

    if ( SDL_OpenAudio( &desired, &m_outputSpec ) < 0 )
        throw Base::AudioOutputException( "Sound_OpenAudio( )" );

    //
    // TODO: This will do for now, but we need to add
    //       output conversion as well.
    //
    m_outputSampleRate = m_outputSpec.freq;
    MAASSERT( desired.format == m_outputSpec.format );
    MAASSERT( desired.channels == m_outputSpec.channels );

    // Start the audio
    SDL_PauseAudio( 0 );

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
AudioInterfaceSDL::~AudioInterfaceSDL ( void )
{
    Lock l( m_mutex );

    m_isActive = false;
    SDL_CloseAudio( );
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
AudioSource* AudioInterfaceSDL::createAudioSource ( const char *m,
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
AudioInterface* AudioInterfaceSDL::getInstance ( void )
{
    if ( m_instance == NULL )
        m_instance = new AudioInterfaceSDL( );

    return m_instance;
}


/**
 * Stub used for the callback
 *
 * @param u     Pointer to user data (ignored)
 * @param b     Pointer to output buffer
 * @param k     Samples to fill
 */
void AudioInterfaceSDL::callback ( void *u, Uint8 *b, int l )
{
    if ( m_instance == NULL || m_isActive == false )
        return;

    static_cast<AudioInterfaceSDL *>( m_instance )->outputCallback( b, l );
}

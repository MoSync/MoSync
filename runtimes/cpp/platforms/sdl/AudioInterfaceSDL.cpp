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

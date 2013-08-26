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
 * File:   AudioInterfaceSDL.h
 * Author: Ali Mosavian
 *
 * Created on July 18, 2009
 */

#ifndef __AUDIOINTERFACESDL_H__
#define	__AUDIOINTERFACESDL_H__

#include <SDL/SDL_audio.h>
#include <AudioChannel.h>
#include <AudioSource.h>
#include <AudioInterface.h>


class AudioInterfaceSDL : public AudioInterface
{
private:
    static bool     m_isActive;
    SDL_AudioSpec   m_outputSpec;


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
    AudioInterfaceSDL ( int r = 44100, int c = 2, int b = 16 );

    /**
     * Stub used for the callback
     *
     * @param u     Pointer to user data (ignored)
     * @param b     Pointer to output buffer
     * @param k     Samples to fill
     */
    static void SDLCALL callback ( void *u, Uint8 *b, int l );

public:
    /**
     * The destructor, closes and delete all live channels,
     * deletes all the audio sources and then close the audio output.
     *
     * Note: This method (delete on the instance) should
     *       only be called from MALibQuit
     */
    virtual ~AudioInterfaceSDL ( void );


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
    virtual AudioSource * createAudioSource ( const char *m, Base::Stream *s );

    /**
     * Gets the instance of the correct platform implementation.
     *
     * @return Pointer to the correct instance of AudioInterface.
     *
     * @throws AudioOutputException if it fails to open the audio
     *         output. This is however only relevant the first time
     *         the method is invoked.
     */
    static AudioInterface * getInstance ( void );
};

#endif	/* __AUDIOINTERFACESDL_H__ */

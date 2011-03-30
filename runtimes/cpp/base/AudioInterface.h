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
 * File:   AudioInterface.h
 * Author: Ali Mosavian
 *
 * Created on July 16, 2009
 */

#ifndef __AUDIOINTERFACE_H__
#define	__AUDIOINTERFACE_H__

#include <list>
#include <thread/mutex.hpp>
#include "Stream.h"


class AudioChannel;
class AudioSource;

namespace Base
{
    class Stream;
};


/**
 * This class is a combined platform independent audio
 * interface and resource handler (for AudioChannel).
 *
 */
class AudioInterface
{
protected:
    static AudioInterface*      m_instance;
    Base::Thread::Mutex*        m_mutex;
    std::list<AudioChannel *>   m_activeChanList;
    std::list<AudioSource *>    m_activeSourceList;
    bool			m_outputSigned;
    int                         m_outputSampleRate;
    int                         m_outputChannels;
    int                         m_outputSampleBits;
    static const int            AUDIO_BUF_SAMPLES = 1024*2;
    static int                  m_audioBuffer[AUDIO_BUF_SAMPLES*2];

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
    AudioInterface ( int r, int c, int b, bool s );

    /**
     * Removes a channel from the active channel list. This is so
     * that the destructor doesn't try to delete that channel. This
     * method is to be called from the destructor of AudioChannel.
     *
     * @param c     Pointer to the channel to remove.
     */
    void removeChannel ( AudioChannel *c );

    /**
     * Removes an audio source from the active source list. This is so
     * that the destructor doesn't try to delete that audio source. This
     * method is to be called from the destructor of AudioSource.
     *
     * @param s     Pointer to the AudioSource to remove.
     */
    void removeAudioSource ( const AudioSource *s );

    /**
     * Deletes all the channels in the active channel list.
     * This method is to be called from the implementations
     * destructor AFTER closing the audio output. It alters
     * the active channel list and is thread safe.
     *
     */
    void deleteChannels ( void );


    /**
     * Deletes all the audio sources in the active source list.
     * This method is to be called from the implementations
     * destructor AFTER closing the audio output and deleting all
     * the channels It alters the active source list and is thread safe.
     *
     */
    void deleteAudioSources ( void );

    /**
     * The callback used by the audio output.
     * Note: The implementation should invoke this callback when it
     *       is time to mix more samples. The method assumes that
     *       the output is 16 bit litte-endian, stereo (for now).
     *
     * @param b     Pointer to the sound buffer to fill
     * @param l     Number of bytes to fill
     */
    void outputCallback ( void *b, size_t l );
    
public:
    /**
     * The destructor, the implementation should close and
     * delete all live channels, delete all the audio sources
     * and then close the audio output. The destructor on the
     * interface will set the instance to NULL.
     *
     * Note: This method (delete on the instance) should
     *       only be called from MALibQuit
     */
    virtual ~AudioInterface ( void );


    /**
     * Creates a new channel which will be mixed in to the
     * output while its active.
     *
     * @return Pointer to the newly created AudioChannel
     *
     * @throws AllocationFailedException if there wasn't
     *         enough memory to create the channel.
     */
    AudioChannel * createChannel ( void );


    /**
     * Creates a new audio source from a stream and its
     * mimetype.
     *
     * Note: This method is to be implemented in the native impl.
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
    virtual AudioSource * createAudioSource ( const char *m, Base::Stream *s ) = 0;


    /**
     * Returns the sampe rate of the output
     *
     * @return Sample rate as integer
     */
    int getSampleRate ( void );


    /**
     * Returns the number of channels on the output.
     *
     * @return Number of channels as integer
     */
    int getChannels ( void );


    /**
     * Returns the bitrate of the output
     *
     * @return Bitrate as integer
     */
    int getBitrate ( void );


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
    static AudioInterface * getInstance ( void );
};


#endif	/* __AUDIOINTERFACE_H__ */

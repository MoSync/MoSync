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

#ifndef _AUDIO_CHANNEL_H_
#define _AUDIO_CHANNEL_H_

#include <cstdlib>

class AudioSource;

/**
 * This class is an audio channel, it takes an audio source
 * as input and does on the fly conversion and mixing to
 * an internal buffer.
 *
 */
class AudioChannel
{
protected:
    bool            mActive;
    int             mVolume;

    int             mOutputSampleRate;

    AudioSource*    mAudioSource;
    int             mBufferedSamples;
    int             mBufferedSamplePos;

public:
    /**
     * Constructor with initial audio source
     *
     * @param s             The sample rate of the output
     * @param audioSource   Pointer to an audio source, default is NULL
     */
    AudioChannel ( int s, AudioSource* audioSource = NULL );

    /**
     * Set the channel audio source
     *
     * @param as    Pointer to an audio source
     */
    void setAudioSource ( AudioSource* as );

    /**
     * Returns the current audio source
     *
     * @return Pointer to an audio source
     */
    AudioSource* getAudioSource ( void );

    /**
     * Sets whether or not the channel is active, that is
     * if it is paused or not.
     *
     * @param a     Active or not, true/false, default is true
     */
    void setActive ( bool a = true );

    /**
     * Returns wether the channel is active or not.
     *
     * @return true if active, false if not
     */
    bool isActive ( void ) const;

    /**
     * Sets the channel volume
     *
     * @param vol   The volume represented as [0, 1) in 16.16
     *              fixed point.
     */
    void setVolume ( int vol );

    /**
     * Returns the channel volume
     *
     * @return The returned value in the range [0,1) in 16.16
     *         fixed point.
     */
    int getVolume ( void );

    /**
     * Converts and mixes the audio source to the internal buffer
     *
     * @param dst           Pointer to the internal buffer to mix in to.
     * @param numSamples    The number of samples to mix.
     *
     */
    virtual void mix ( int *buffer, int numSamples );
};

#endif /* _AUDIO_CHANNEL_H_ */

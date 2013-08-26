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

		virtual ~AudioChannel() {}

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

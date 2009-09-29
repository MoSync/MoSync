/*******************************************************************************
*                                                                              *
*   SDL_ffmpeg is a library for basic multimedia functionality.                *
*   SDL_ffmpeg is based on ffmpeg.                                             *
*                                                                              *
*   Copyright (C) 2007  Arjan Houben                                           *
*                                                                              *
*   SDL_ffmpeg is free software: you can redistribute it and/or modify         *
*   it under the terms of the GNU Lesser General Public License as published   *
*	by the Free Software Foundation, either version 3 of the License, or any   *
*   later version.                                                             *
*                                                                              *
*   This program is distributed in the hope that it will be useful,            *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
*   GNU Lesser General Public License for more details.                        *
*                                                                              *
*   You should have received a copy of the GNU Lesser General Public License   *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
*                                                                              *
*******************************************************************************/

#include "SDL.h"
#include "SDL_ffmpeg.h"

#include <string.h>

int audioCallback(void *udata, Uint8 *stream, int len) {

	/* unpack our void pointer */
	SDL_ffmpegFile *file = (SDL_ffmpegFile*)udata;

	int bytesUsed;
	int offset = 0;
	SDL_ffmpegAudioFrame *frame = SDL_ffmpegGetAudioFrame(file);
	if(!frame) return -1;

	while(len > 0) {

		/* check if we need a new frame */
		if(!frame->size) {
			frame = SDL_ffmpegGetAudioFrame(file);
			if(!frame) return -1;
		}

		if(frame->size <= len) {
			/* this frame is smaller or equal to the amount of data needed. */
			bytesUsed = frame->size;
		} else {
			/* this frame has more data than needed */
			bytesUsed = len;
		}

		/* copy the correct amount of data */
		memcpy(stream+offset, frame->buffer, bytesUsed);
		/* adjust the needed length accordingly */
		len -= bytesUsed;
		offset += bytesUsed;

		/* we release our audio data, so the decode thread can fill it again */
		/* we also inform this function of the amount of bytes we used, so it can */
		/* move the buffer accordingly */
		/* important! this call is paired with SDL_ffmpegGetAudio */
		SDL_ffmpegReleaseAudio(file, frame, bytesUsed);
	}

	return 0;
}

int main(int argc, char** argv) {
	SDL_ffmpegFile* film;
	int s;
	SDL_ffmpegStream *str;
	SDL_AudioSpec *specs;
	int w,h;
	int done = 0;
	SDL_Surface *screen;

	/* check if we got an argument */
	if(argc < 2) {
		printf("usage: \"%s\" \"filename\"\n", argv[0]);
		return -1;
	}
	/* standard SDL initialization stuff */
	if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_DOUBLEBUF) < 0) {
		fprintf(stderr, "problem initializing SDL: %s\n", SDL_GetError());
		return -1;
	}

	/* open file from arg[1] */
	film = SDL_ffmpegOpen(argv[1]);
	if(!film) {
		printf("error opening file\n");
		return -1;
	}

	/* print some info on detected stream to output */

	for(s = 0; s<film->AStreams; s++) {
		str = SDL_ffmpegGetAudioStream(film, s);

		printf("Info on audiostream #%i:\n", s);
		printf("\tChannels: %i\n",      str->channels);
		if(strlen(str->language)) printf("\tLanguage: %s\n",      str->language);
		printf("\tSampleRate: %i\n",    str->sampleRate);
	}

	for(s = 0; s<film->VStreams; s++) {
		str = SDL_ffmpegGetVideoStream(film, s);

		printf("Info on videostream #%i:\n", s);
		if(strlen(str->language)) printf("\tLanguage: %s\n",      str->language);
		printf("\tFrame: %ix%i\n",  str->width, str->height);
		printf("\tFrameRate: %.2ffps\n",  1.0 / (str->frameRate[0] / str->frameRate[1]));
	}

	/* select the streams you want to decode (example just uses 0 as a default) */
	SDL_ffmpegSelectVideoStream(film, 0);
	SDL_ffmpegSelectAudioStream(film, 0);

	/* get the audiospec which fits the selected videostream, if no videostream */
	/* is selected, default values are used (2 channel, 48Khz) */
	specs = SDL_ffmpegGetAudioSpec(film, 512, audioCallback);

	/* we get the size from our active video stream, if no active video stream */
	/* exists, width and height are set to default values (320x240) */
	SDL_ffmpegGetVideoSize(film, &w, &h);

	/* Open the Video device */
	screen = SDL_SetVideoMode(w, h, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
	if(!screen) {
		printf("Couldn't open video: %s\n", SDL_GetError());
		return -1;
	}

	/* Open the Audio device */
	if( SDL_OpenAudio(specs, 0) < 0 ) {
		printf("Couldn't open audio: %s\n", SDL_GetError());
		return -1;
	}

	/* we start our decode thread, this always tries to buffer in some frames */
	/* so we can enjoy smooth playback */
	SDL_ffmpegStartDecoding(film);

	/* we unpause the audio so our audiobuffer gets read */
	SDL_PauseAudio(0);


	while( !done ) {
		SDL_ffmpegVideoFrame* frame;

		/* just some standard SDL event stuff */
		SDL_Event event;
		while(SDL_PollEvent(&event)) {

			if(event.type == SDL_QUIT) {
				done = 1;
				break;
			}

			if(event.type == SDL_MOUSEBUTTONDOWN) {
				int x,y;
				int64_t time;
				SDL_PumpEvents();

				SDL_GetMouseState(&x, &y);
				/* by clicking you turn on the stream, seeking to the percentage */
				/* in time, based on the x-position you clicked on */
				time = ((double)x / w) * SDL_ffmpegGetDuration(film);

				/* we seek to time (milliseconds) */
				SDL_ffmpegSeek(film, time);

				/* by passing 0(false) as our second argument, we play the file */
				/* passing a non-zero value would mean we pause our file */
				SDL_ffmpegPause(film, 0);
			}
		}

		/* we retrieve the current image from the file */
		/* we get 0 if no file could be retrieved */
		/* important! please note this call should be paired with SDL_ffmpegReleaseVideo */
		frame = SDL_ffmpegGetVideoFrame(film);

		if(frame) {

			/* we got a frame, so we better show this one */
			SDL_BlitSurface(frame->buffer, 0, screen, 0);

			/* After releasing this frame, you can no longer use it. */
			/* you should call this function every time you get a frame! */
			SDL_ffmpegReleaseVideo(film, frame);

			/* we flip the double buffered screen so we might actually see something */
			SDL_Flip(screen);
		}

		/* we wish not to kill our poor cpu, so we give it some timeoff */
		SDL_Delay(5);
	}

	/* after all is said and done, we should call this */
	SDL_ffmpegFree(film);

	/* the SDL_Quit function offcourse... */
	SDL_Quit();

	return 0;
}

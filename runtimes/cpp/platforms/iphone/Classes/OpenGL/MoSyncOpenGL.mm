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

#import "MoSyncOpenGL.h"
#import "MoSyncUISyscalls.h"

#ifdef SUPPORT_OPENGL_ES
#define DONT_WANT_IX_OPENGL_ES_TYPEDEFS
#include <helpers/CPP_IX_OPENGL_ES_MA.h>
#include <helpers/CPP_IX_OPENGL_ES.h>
#include <helpers/CPP_IX_GL1.h>
#include <helpers/CPP_IX_GL2.h>
#include <helpers/CPP_IX_GL_OES_FRAMEBUFFER_OBJECT.h>
#endif

MAHandle sOpenGLScreen = -1;
MAHandle sOpenGLView = -1;

int maOpenGLInitFullscreen(int glApi) {
	if(sOpenGLScreen != -1) return 0;

	if(glApi == MA_GL_API_GL1)
		sOpenGLView = maWidgetCreate("GLView");
	else if(glApi == MA_GL_API_GL2)
		sOpenGLView = maWidgetCreate("GL2View");
	else
		return MA_GL_INIT_RES_UNAVAILABLE_API;

	if(sOpenGLView < 0) {
		return MA_GL_INIT_RES_UNAVAILABLE_API;
	}

	sOpenGLScreen = maWidgetCreate("Screen");
	maWidgetSetProperty(sOpenGLView, "width", "-1");
	maWidgetSetProperty(sOpenGLView, "height", "-1");
	maWidgetAddChild(sOpenGLScreen, sOpenGLView);
	maWidgetScreenShow(sOpenGLScreen);
	maWidgetSetProperty(sOpenGLView, "bind", "");
	return 1;
}

int maOpenGLCloseFullscreen() {
	if(sOpenGLScreen == -1) return 0;
	maWidgetRemoveChild(sOpenGLView);
	maWidgetDestroy(sOpenGLView);
	maWidgetDestroy(sOpenGLScreen);
	sOpenGLView = -1;
	sOpenGLScreen = -1;
	return 1;
}

int maOpenGLTexImage2D(MAHandle image) {
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(image);

	int powWidth = nextPowerOf2(1, img->width);
	int powHeight = nextPowerOf2(1, img->height);

	if(powWidth!=img->width || powHeight!=img->height) {

		//surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, surface->format->BitsPerPixel,
		//							   surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

		int bytesPerPixel = 4; // for now.

		int oldBytesPerRow = img->rowBytes;
		int newBytesPerRow = powWidth*bytesPerPixel;
		int oldActualBytesPerRow = img->width*bytesPerPixel;

		byte* data = new byte[powHeight*newBytesPerRow];

		byte* src = (byte*)img->data;
		byte* dst = data;
		for(int y = 0; y < img->height; y++) {
			memcpy(dst, src, oldActualBytesPerRow);
			src+=oldBytesPerRow;
			dst+=newBytesPerRow;
		}

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, powWidth, powHeight, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete data;

		return MA_GL_TEX_IMAGE_2D_OK;
	}

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, img->data);

	return MA_GL_TEX_IMAGE_2D_OK;
}

int maOpenGLTexSubImage2D(MAHandle image) {
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(image);

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, img->width, img->height, GL_RGBA
					, GL_UNSIGNED_BYTE, img->data);

	return MA_GL_TEX_IMAGE_2D_OK;
}

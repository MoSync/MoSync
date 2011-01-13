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

#include "GraphicsOpenGL.h"
#include <maassert.h>

#include <GLES/gl.h>

void ogl_setup(int x, int y, int w, int h);
void ogl_setClipRect(int x, int y, int w, int h);
void ogl_clearMatrix(void);
void ogl_pushMatrix(void);
void ogl_popMatrix(void);
void ogl_translate(int x, int y);
MAPoint2d ogl_getTranslation(void);
void ogl_plot(int x, int y);
void ogl_line(int x1, int y1, int x2, int y2);
void ogl_fillRect(int left, int top, int width, int height);
void ogl_drawText(int left, int top, const char* text);
void ogl_drawTextW(int left, int top, const wchar_t* text);
void ogl_drawImage(MAHandle image, int left, int top);
void ogl_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength);
void ogl_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode);
void ogl_notifyImageUpdated(MAHandle image);

static MARect sViewPort;


static MAGraphicsDriver sGLGraphicsDriver = {
	&ogl_setup,
	&ogl_setClipRect,
	&ogl_clearMatrix,
	&ogl_pushMatrix,
	&ogl_popMatrix,
	&ogl_translate,
	&ogl_getTranslation,
	&ogl_plot,
	&ogl_line,
	&ogl_fillRect,
	&ogl_drawText,
	&ogl_drawTextW,
	&ogl_drawImage,
	&ogl_drawRGB,
	&ogl_drawImageRegion,
	&ogl_notifyImageUpdated,
};

MAGraphicsDriver* Gfx_getDriverOpenGL(void) {
	return &sGLGraphicsDriver;
}

void ogl_setup(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, w, h, 0, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);	
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glEnable(GL_SCISSOR_TEST);
	
	sViewPort.left = x;
	sViewPort.top = y;
	sViewPort.width = w;
	sViewPort.height = h;	

}

void ogl_setClipRect(int x, int y, int w, int h) {
	glScissor(	x, 
	sViewPort.height-y-h, 
	w, 
	h);
}

void ogl_clearMatrix(void) {
	glLoadIdentity();
}

void ogl_pushMatrix(void) {
	glPushMatrix();
}

void ogl_popMatrix(void) {
	glPopMatrix();
}

void ogl_translate(int x, int y) {
	//glTranslatef((float)x, (float)y, 0.0f);
	glTranslatex(x<<16, y<<16, 0);
}

MAPoint2d ogl_getTranslation(void) {
	int mtx[16];
	MAPoint2d res;
	glGetIntegerv(GL_MODELVIEW_MATRIX, mtx);
	res.x = (int)mtx[12];
	res.y = (int)mtx[13];
	return res;
}



static int numTextures = 0;

typedef struct Texture_t {
	GLuint glTexture;
	MAHandle maTexture;
} Texture;

static Texture textures[256];

static GLuint getTexture(MAHandle image);
static GLuint getTexture(MAHandle image) {	
	int i;
	GLuint handle;	
	
	for(i = 0; i < numTextures; i++) {
		if(textures[i].maTexture == image) {
			return textures[i].glTexture;
		}
	}
		
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &handle);
	
	glBindTexture(GL_TEXTURE_2D, handle);
	maOpenGLTexImage2D(image);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	textures[numTextures].glTexture = handle;
	textures[numTextures].maTexture = image;
	numTextures++;
	
	return handle;
}

void drawTriangleFan(GLshort* textureCoords, GLshort* vertexCoords);
void drawTriangleFan(GLshort* textureCoords, GLshort* vertexCoords) {
	if(textureCoords)
		glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if(textureCoords)
		glTexCoordPointer(2, GL_SHORT, 0, textureCoords);
	glVertexPointer(2, GL_SHORT, 0, vertexCoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	if(textureCoords)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

unsigned int nextPowerOf2(uint minPow, uint x);
unsigned int nextPowerOf2(uint minPow, uint x) {
	unsigned int i = 1 << minPow;
	while(i < x) {
		i <<= 1;		
	}
	return i;
}


void drawImage(GLshort* textureCoords, GLshort* vertexCoords, MAHandle image);
void drawImage(GLshort* textureCoords, GLshort* vertexCoords, MAHandle image) {
	GLuint texture = getTexture(image);
	MAExtent imageSize = maGetImageSize(image);
	int imageWidth = EXTENT_X(imageSize);
	int imageHeight = EXTENT_Y(imageSize);

	imageWidth = nextPowerOf2(1, imageWidth);
	imageHeight = nextPowerOf2(1, imageHeight);
		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);	
	
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
	
	glLoadIdentity();
	//glScalef(1.0f / (float)imageWidth, 1.0f / (float)imageHeight, 1.0f);
	glScalex(0x10000/imageWidth, 0x10000/imageHeight, 0x10000);
	
	glMatrixMode(GL_MODELVIEW);

	drawTriangleFan(textureCoords, vertexCoords);
	
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	
}


void ogl_plot(int x, int y) {
}

void ogl_line(int x1, int y1, int x2, int y2) {
}

void ogl_fillRect(int left, int top, int width, int height) {
	
	GLshort vertexCoords[] = {
		left, top,
		left+width, top,
		left+width, top+height,
		left, top+height
	};
	
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	
	glDisable(GL_TEXTURE_2D);
		
	drawTriangleFan(NULL, vertexCoords);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
	
}

void ogl_drawText(int left, int top, const char* text) {
}

void ogl_drawTextW(int left, int top, const wchar_t* text) {
}

void ogl_drawImage(MAHandle image, int left, int top) {
	MAExtent imageSize = maGetImageSize(image);
	int imageWidth = EXTENT_X(imageSize);
	int imageHeight = EXTENT_Y(imageSize);
	

	GLshort textureCoords[] = {
		0, 0,
		imageWidth, 0,
		imageWidth, imageHeight,
		0, imageHeight
	};
	
	GLshort vertexCoords[] = {
		left, top,
		left+imageWidth, top,
		left+imageWidth, top+imageHeight,
		left, top+imageHeight
	};

	drawImage(textureCoords, vertexCoords, image);				
}

void ogl_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength) {
}

void ogl_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode) {
	GLshort textureCoords[] = {
		(srcRect->left), (srcRect->top),
		(srcRect->left + srcRect->width), (srcRect->top),
		(srcRect->left + srcRect->width), (srcRect->top + srcRect->height),
		(srcRect->left), (srcRect->top + srcRect->height)
	};
	
	
	GLshort vertexCoords[] = {
		dstPoint->x, dstPoint->y,
		dstPoint->x+srcRect->width, dstPoint->y,
		dstPoint->x+srcRect->width, dstPoint->y+srcRect->height,
		dstPoint->x, dstPoint->y+srcRect->height
	};

	drawImage(textureCoords, vertexCoords, image);
}

void ogl_notifyImageUpdated(MAHandle image) {
	int i;
	GLuint handle;
	
	for(i = 0; i < numTextures; i++) {
		if(textures[i].maTexture == image) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[i].glTexture);
			maOpenGLTexImage2D(image);
			return;
		}
	}
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	maOpenGLTexImage2D(image);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	textures[numTextures].glTexture = handle;
	textures[numTextures].maTexture = image;
	numTextures++;
	
}


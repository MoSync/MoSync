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
#include <IX_WIDGET.h>

static void ogl_setup(int x, int y, int w, int h);
static void ogl_setClipRect(int x, int y, int w, int h);
static void ogl_clearMatrix(void);
static void ogl_pushMatrix(void);
static void ogl_popMatrix(void);
static void ogl_translate(int x, int y);
static MAPoint2d ogl_getTranslation(void);
static void ogl_scale(MAFixed x, MAFixed y);
static void ogl_plot(int x, int y);
static void ogl_line(int x1, int y1, int x2, int y2);
static void ogl_fillRect(int left, int top, int width, int height);
static void ogl_drawText(int left, int top, const char* text);
static void ogl_drawTextW(int left, int top, const wchar_t* text);
static void ogl_drawImage(MAHandle image, int left, int top);
static void ogl_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength);
static void ogl_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode);
static void ogl_notifyImageUpdated(MAHandle image);
static void ogl_beginRendering(void);
static void ogl_updateScreen(void);
static void ogl_setClearColor(int r, int g, int b);
static void ogl_setColor(int r, int g, int b);
static void ogl_setAlpha(int a);

static MARect sViewPort;


static MAGraphicsDriver sGLGraphicsDriver = {
	&ogl_setup,
	&ogl_setClipRect,
	&ogl_clearMatrix,
	&ogl_pushMatrix,
	&ogl_popMatrix,
	&ogl_translate,
	&ogl_getTranslation,
	&ogl_scale,
	&ogl_plot,
	&ogl_line,
	&ogl_fillRect,
	&ogl_drawText,
	&ogl_drawTextW,
	&ogl_drawImage,
	&ogl_drawRGB,
	&ogl_drawImageRegion,
	&ogl_notifyImageUpdated,
	&ogl_beginRendering,
	&ogl_updateScreen,
	&ogl_setClearColor,
	&ogl_setColor,
	&ogl_setAlpha
};

static int sNativeUIOpenGLView = -1;

//MAGraphicsDriver* Gfx_getDriverOpenGL(void) {
void Gfx_useDriverOpenGL(int nativeUIOpenGLView) {
	sNativeUIOpenGLView = nativeUIOpenGLView;
	Gfx_useDriver(&sGLGraphicsDriver);
}

static void ogl_setup(int x, int y, int w, int h) {
	glViewport(x, y, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, (GLfloat)w, (GLfloat)h, 0, 0, 1);

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

static void ogl_setClipRect(int x, int y, int w, int h) {
	glScissor(	x, 
		sViewPort.height-y-h, 
		w, 
		h);
}

static void ogl_clearMatrix(void) {
	glLoadIdentity();
}

static void ogl_pushMatrix(void) {
	glPushMatrix();
}

static void ogl_popMatrix(void) {
	glPopMatrix();
}

static void ogl_translate(int x, int y) {
	//glTranslatef((float)x, (float)y, 0.0f);
	glTranslatex(x<<16, y<<16, 0);
}

static MAPoint2d ogl_getTranslation(void) {
	int mtx[16];
	MAPoint2d res;
	glGetIntegerv(GL_MODELVIEW_MATRIX, mtx);
	res.x = (int)mtx[12];
	res.y = (int)mtx[13];
	return res;
}

static void ogl_scale(MAFixed x, MAFixed y) {
	glScalex(x, y, 0x10000);
}

static int numTextures = 0;

typedef struct Texture_t {
	GLuint glTexture;
	MAHandle maTexture;

	int imageWidth;
	int imageHeight;
	int textureWidth;
	int textureHeight;

	GLfixed textureWidthInv;
	GLfixed textureHeightInv;

	GLshort textureCoords[8]; 

} Texture;

static Texture textures[256];

static unsigned int nextPowerOf2(uint minPow, uint x) {
	unsigned int i = 1 << minPow;
	while(i < x) {
		i <<= 1;		
	}
	return i;
}

static Texture* addTexture(MAHandle image, GLuint handle) {
	int imageWidth = EXTENT_X(maGetImageSize(image));
	int imageHeight = EXTENT_Y(maGetImageSize(image));

	textures[numTextures].glTexture = handle;
	textures[numTextures].maTexture = image;
	textures[numTextures].imageWidth = imageWidth;
	textures[numTextures].imageHeight = imageHeight;
	textures[numTextures].textureWidth = nextPowerOf2(1, imageWidth);
	textures[numTextures].textureHeight = nextPowerOf2(1, imageHeight);
	textures[numTextures].textureWidthInv = 0x10000/textures[numTextures].textureWidth;
	textures[numTextures].textureHeightInv = 0x10000/textures[numTextures].textureHeight;

	// cache texture coordinates (for the most common case, drawImage).
	textures[numTextures].textureCoords[0] = 0;
	textures[numTextures].textureCoords[1] = 0;
	textures[numTextures].textureCoords[2] = imageWidth;
	textures[numTextures].textureCoords[3] = 0;
	textures[numTextures].textureCoords[4] = imageWidth;
	textures[numTextures].textureCoords[5] = imageHeight;
	textures[numTextures].textureCoords[6] = 0;
	textures[numTextures].textureCoords[7] = imageHeight;

	numTextures++;

	//printf("Added texture. Num textures: %d\n", numTextures);

	return &textures[numTextures-1];
}

static Texture* getTexture(MAHandle image) {	
	int i;
	GLuint handle;	

	for(i = numTextures-1; i >= 0; i--) {

		// should check more things than handle here (handle may be reused with placeholder pool)
		if(textures[i].maTexture == image) {
			return &textures[i];
		}
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	maOpenGLTexImage2D(image);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return addTexture(image, handle);	
}

static void drawTriangleFan(GLshort* textureCoords, GLshort* vertexCoords) {
	if(textureCoords)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	if(textureCoords)
		glTexCoordPointer(2, GL_SHORT, 0, textureCoords);
	glVertexPointer(2, GL_SHORT, 0, vertexCoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	if(textureCoords)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

static void drawImage(GLshort* textureCoords, GLshort* vertexCoords, Texture* texture) {		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->glTexture);	

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();

	//glLoadIdentity();
	// maybe save this matrix for each texture...
	glScalex(texture->textureWidthInv, texture->textureHeightInv, 0x10000);

	glMatrixMode(GL_MODELVIEW);

	drawTriangleFan(textureCoords, vertexCoords);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


static void ogl_plot(int x, int y) {
}

static void ogl_line(int x1, int y1, int x2, int y2) {
}

static void ogl_fillRect(int left, int top, int width, int height) {

	GLshort vertexCoords[] = {
		left, top,
		left+width, top,
		left+width, top+height,
		left, top+height
	};

	//glDisable(GL_BLEND);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_TEXTURE_2D);

	drawTriangleFan(NULL, vertexCoords);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	

	//glEnable(GL_BLEND);


}

static void ogl_drawText(int left, int top, const char* text) {
}

static void ogl_drawTextW(int left, int top, const wchar_t* text) {
}

static void ogl_drawImage(MAHandle image, int left, int top) {
	Texture* texture = getTexture(image);

	GLshort vertexCoords[] = {
		left, top,
		left+texture->imageWidth, top,
		left+texture->imageWidth, top+texture->imageHeight,
		left, top+texture->imageHeight
	};

	//	if(left>sViewPort.width || left+texture->imageWidth<0 ||
	//		top>sViewPort.height || top+texture->imageHeight<0)
	//		return;	

	drawImage(texture->textureCoords, vertexCoords, texture);				
}

static void ogl_drawRGB(const MAPoint2d *dstPoint, const void *src, const MARect *srcRect, int scanlength) {
}

static void ogl_drawImageRegion(MAHandle image, const MARect *srcRect, const MAPoint2d *dstPoint, int transformMode) {	
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

	Texture* texture = getTexture(image);	

	drawImage(textureCoords, vertexCoords, texture);
}

static void ogl_notifyImageUpdated(MAHandle image) {
	int i;
	GLuint handle;

	for(i = 0; i < numTextures; i++) {
		if(textures[i].maTexture == image) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[i].glTexture);
			maOpenGLTexSubImage2D(image);
			return;
		}
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	maOpenGLTexImage2D(image);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	addTexture(image, handle);
}

static void ogl_beginRendering(void) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

static void ogl_updateScreen(void) {
	if(sNativeUIOpenGLView == -1)
		maUpdateScreen();
	else
		maWidgetSetProperty(sNativeUIOpenGLView, "invalidate", "");
}

static int sColorR = 255, sColorG = 255, sColorB = 255, sAlpha = 255;

static void ogl_setClearColor(int r, int g, int b) {
	glClearColorx(r<<8, g<<8, b<<8, 255<<8);
}

static void ogl_setColor(int r, int g, int b) {
	sColorR = r;
	sColorG = g;
	sColorB = b;
	glColor4x(sColorR<<8, sColorG<<8, sColorB<<8, sAlpha<<8);
}

static void ogl_setAlpha(int a) {
	sAlpha = a;
	glColor4x(sColorR<<8, sColorG<<8, sColorB<<8, sAlpha<<8);	
}

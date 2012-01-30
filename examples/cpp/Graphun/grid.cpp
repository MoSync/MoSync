/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include <MAUtil/Moblet.h>
#include "OpenGLHelpers.h"
#include "Grid.h"
#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>
#include "MAHeaders.h"
#include <math.h>
#include "Camera.h"

using namespace MAUtil;

#define TEXTURE_WIDTH 256
#define TEXTURE_HEIGHT 256

void Grid::initTexture() {
	glEnable( GL_TEXTURE_2D);
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameterx(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterx(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D,GL_GENERATE_MIPMAP, GL_TRUE);
	maOpenGLTexImage2D( R_TEXTURE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Grid::initGrid(int gx, int gy) {
	gridSizeX = gx;
	gridSizeY = gy;

	mVertices.clear();
	mColors.clear();
	mTexCoords.clear();

	for (int j = 0; j < gridSizeY; j += 1) {
		for (int i = 0; i <= gridSizeX; i += 1) {
			GLfixed x1 = i;
			GLfixed y1 = j;
			GLfixed x2 = i;
			GLfixed y2 = j + 1;

			x1 <<= 16;
			y1 <<= 16;
			x2 <<= 16;
			y2 <<= 16;

			mVertices.add(-(0x10000/2) + x1 / gridSizeX);
			mVertices.add(-(0x10000/2) + y1 / gridSizeY);
			mVertices.add(0);
			mVertices.add(-(0x10000/2) + x2 / gridSizeX);
			mVertices.add(-(0x10000/2) + y2 / gridSizeY);
			mVertices.add(0);

			mColors.add(0xffff);
			mColors.add(0xffff);
			mColors.add(0xffff);
			mColors.add(0xffff);

			mColors.add(0xffff);
			mColors.add(0xffff);
			mColors.add(0xffff);
			mColors.add(0xffff);

			/*
			mTexCoords.add((x1) / gridSizeX);
			mTexCoords.add((y1) / gridSizeY);
			mTexCoords.add((x2) / gridSizeX);
			mTexCoords.add((y2) / gridSizeY);
			*/
			mTexCoords.add((x1) / 32);
			mTexCoords.add((y1) / 32);
			mTexCoords.add((x2) / 32);
			mTexCoords.add((y2) / 32);

		}
	}
}

double Grid::getRotation() {
	return mRotation;
}

void Grid::setRotation(double rot) {
	mRotation = rot;
}


Grid::Grid(int x, int y, int w, int h) : expression(scope) {

	scope.setVariable("pi", 3.14159);
	scope.setVariable("x", 1.0);
	scope.setVariable("y", 2.0);
	scope.setFunction("sin", (void*)sinf, 1);
	scope.setFunction("cos", (void*)cosf, 1);
	scope.setFunction("tan", (void*)tanf, 1);
	scope.setFunction("abs", (void*)fabsf, 1);
	scope.setFunction("sqrt", (void*)sqrtf, 1);

	width = w;
	height = h;
	initGrid();
	//initTexture();
	//initGL();
	//resizeWindow(0, 0, width, height);

	validExpression = false;
	mRotation = 0.0;

	setExpression("0");

	/*
	glViewport(x, y, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, 1, 1, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	mShadingEnabled = true;
}

void Grid::initContext(int w, int h) {
	width = w;
	height = h;
	initTexture();
	initGL();
	resizeWindow(0, 0, width, height);
}
	
void Grid::setShadingEnabled(bool enabled) {
	mShadingEnabled = enabled;
}

bool Grid::getShadingEnabled() const {
	return mShadingEnabled;
}

bool Grid::setExpression(const String& exp) {
	if(expression.compile(exp.c_str())) {
		validExpression = true;
		//updateGrid();
	}
	else
		return false;

	return true;
}

void Grid::render() {
	glClearColorx(0, 0, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if(validExpression)
		updateGrid();

	glEnable( GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glLoadIdentity();

	Camera camera(vec3(cos(mRotation)*1.25, sin(mRotation)*1.25, -1), vec3(0, 0, 0), vec3(0, 0, -1));
	camera.setCameraMatrix();

	glScalex(0xffff, 0xffff, (0xffff>>2));

	glColor4x(0xffff, 0xffff, 0xffff, 0xffff);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY);
	glEnableClientState( GL_VERTEX_ARRAY);
	if(mShadingEnabled)
		glEnableClientState( GL_COLOR_ARRAY );

	int numElementsPerStrip = (gridSizeX+1) * 6;
	int numVerticesPerStrip = numElementsPerStrip / 3;
	glTexCoordPointer(2, GL_FIXED, 0, &mTexCoords[0]);
	glVertexPointer(3, GL_FIXED, 0, &mVertices[0]);
	if(mShadingEnabled)
		glColorPointer(4, GL_FIXED, 0, &mColors[0]);

	int startIndex = 0;
	for (int j = 0; j < gridSizeY; j += 1) {
		glDrawArrays(GL_TRIANGLE_STRIP, startIndex, numVerticesPerStrip);
		startIndex += numVerticesPerStrip;
	}

	if(mShadingEnabled)
		glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void Grid::updateGrid() {

	float x, y = 0.0f;
	int index = 0;
	float deltaX = 1.0f/(float)gridSizeX;
	float deltaY = 1.0f/(float)gridSizeY;
	scope.setVariable("time", (double)maGetMilliSecondCount()*0.001);
	int colorIndex = 0;

	for (int j = 0; j < gridSizeY; j += 1) {
		x = 0.0f;
		for (int i = 0; i <= gridSizeX; i += 1) {
			GLfixed x1 = i;
			GLfixed y1 = j;
			GLfixed x2 = i;
			GLfixed y2 = j + 1;

			x1 <<= 16;
			y1 <<= 16;
			x2 <<= 16;
			y2 <<= 16;

			scope.setVariable("x", x-0.5);

			if(j == 0) {
				scope.setVariable("y", y-0.5);
				float z1 = -expression.evaluate();

				GLfixed zfx1 = ((GLfixed)(z1*65536.0f));
				if(zfx1<-0xffff) zfx1 = -0xffff;
				if(zfx1>0xffff) zfx1 = 0xffff;
				mVertices[index++] = (-(0x10000/2) + x1 / gridSizeX);
				mVertices[index++] = (-(0x10000/2) + y1 / gridSizeY);
				mVertices[index++] = zfx1;

				if(mShadingEnabled) {
					zfx1 = (0xffff+(0xffff>>2)-zfx1)>>1;
					mColors[colorIndex++] =zfx1;
					mColors[colorIndex++] =zfx1;
					mColors[colorIndex++] =zfx1;
					colorIndex++;
				}
			} else {
				mVertices[index] = mVertices[index-(gridSizeX+1)*6+3];
				index++;
				mVertices[index] = mVertices[index-(gridSizeX+1)*6+3];
				index++;
				mVertices[index] = mVertices[index-(gridSizeX+1)*6+3];
				index++;

				if(mShadingEnabled) {
					mColors[colorIndex] = mColors[colorIndex - (gridSizeX+1)*8 + 4];
					colorIndex++;
					mColors[colorIndex] = mColors[colorIndex - (gridSizeX+1)*8 + 4];
					colorIndex++;
					mColors[colorIndex] = mColors[colorIndex - (gridSizeX+1)*8 + 4];
					colorIndex++;
					colorIndex++;
				}
			}

			scope.setVariable("y", y+deltaY-0.5);
			float z2 = -expression.evaluate();

			GLfixed zfx2 = ((GLfixed)(z2*65536.0f));
			if(zfx2<-0xffff) zfx2 = -0xffff;
			if(zfx2>0xffff) zfx2 = 0xffff;
			mVertices[index++] = (-(0x10000/2) + x2 / gridSizeX);
			mVertices[index++] = (-(0x10000/2) + y2 / gridSizeY);
			mVertices[index++] = zfx2;

			if(mShadingEnabled) {
				zfx2 = (0xffff+(0xffff>>2)-zfx2)>>1;
				mColors[colorIndex++] = zfx2;
				mColors[colorIndex++] = zfx2;
				mColors[colorIndex++] = zfx2;
				colorIndex++;
			}

			x+=deltaX;
		}
		y+=deltaY;
	}
}


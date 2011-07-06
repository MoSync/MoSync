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

#ifndef _GRID_H_
#define _GRID_H_

#include "OpenGLHelpers.h"
#include "eval.h"

/**
 * Grid renderer. Renders a grid on a plane with an isometric camera perspective.
 * For each point on the plane an expression is evaluated to calculate the z value.
 */

class Grid {
private:
	GLuint mTexture;
	Vector<GLfixed> mVertices;
	Vector<GLfixed> mTexCoords;
	Vector<GLfixed> mColors;

	int gridSizeX, gridSizeY;
	int width, height;

	void initTexture();
	Expression expression;
	Scope scope;

	void updateGrid();

	double mRotation;
	bool validExpression;
	bool mShadingEnabled;

public:
	Grid(int x, int y, int w, int h);
	void initContext(int w, int h);

	void initGrid(int gridSizeX=8, int gridSizeY=8);
	void setShadingEnabled(bool enabled);
	bool getShadingEnabled() const;
	bool setExpression(const String& exp);
	void render();
	double getRotation();
	void setRotation(double rot);
};

#endif

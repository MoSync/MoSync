/*
 * Copyright (C) 2013 MoSync AB

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

#ifndef AXISMGR_H_
#define AXISMGR_H_

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shaders.h"
#include "IRender.h"

namespace MoGraph
{

class Scene;


/**
 * \brief Line struct Grid/Axis/Line display in graph,
 * axis aligned, but could be rotated at will
 */

struct Line
{
	/**
	 * \brief TextEnum
	 * where the origo is relative inside the text string should be.
	 */
	enum LineEnum{ X_AXIS_ALIGN=0, Y_AXIS_ALIGN, Z_AXIS_ALIGN };
	enum LineType{ AXIS_LINE, GRID_LINE_Y, GRID_LINE_X };

	LineEnum		mAxisAlign;		// Use Line with axis alignment of X,Y,Z
	LineType		mType;			// Line Type to be used, as AXIS, as GRIDS etc..
	glm::vec3 		mPos;			// Text position in 3d space
	glm::vec4 		mColor;			// Text color
	glm::vec3		mScale;			// Line scale
	glm::vec3		mRotate;		// Rotation in degrees. prio Yaw Pitch Roll
	int				mWidth;			// Line Width (Thickness)
};

/**
 * \brief Handles the AXIS lines includeing grid marks to show the steps for a value.
 *
 */
class Axis
{
protected:

	std::vector<glm::vec3> mVertices;
public:

	/**
	 * \brief vertices, contains 2 points to form one line.
	 * @return reference of vertices,
	 */
	std::vector<glm::vec3>& vertices()	{return mVertices;}

	/**
	 * \brief size, size of vertex array
	 * @return size
	 */
	int size() 							{return mVertices.size();}
};


/**
 * \brief AxisMgr class handles several Axis lines for the graph display
 */
class AxisMgr : public Render
{
protected:
	std::vector<Axis> 	mAxisArray;	// value and time  can support up to 3D X,Y,Z
	std::vector<Line>	mLineArray;	// array of lines.
	LineShader			mShader; 	// Shader to be used for rendering the lines.
	float				mGridStep;	// Step of each grid bars in height. (note total height is amount*step)
	int 				mGridLines;	// Grid lines for graph bar in Y... stepped with mStep, to get height grid,
	void create3D();
	void handleGridLines(int startOffset, Line &ln, float centerX, float centerZ);

public:
	enum {AXIS_X,AXIS_Y,AXIS_Z};

	/**
	 * \brief AxisMgr, Constructor
	 * @param scene, input reference to its scene
	 */
	AxisMgr(Scene *scene) : Render(scene), mGridStep(1.0f), mGridLines(2) {}

	/**
	 * \brief AxisMgr, Destructor
	 */
	virtual ~AxisMgr() {}

	/**
	 * \brief getLine
	 * @param i,	input index entry
	 * @return Line output element
	 */
	Line &getLine(int i) 				{return mLineArray[i];}

	/**
	 * \brief getLineArray
	 * @return	reference of LineArray
	 */
	std::vector<Line>& getLineArray() 	{return mLineArray;}

	/**
	 * \brief size, size of LineArray
	 * @return size
	 */
	int size() 							{return mLineArray.size();}

	/**
	 * \brief getShader , get Shader of Axis lines
	 * @return LineShader
	 */
	LineShader &getShader() 			{return mShader;}

	/**
	 * \brief addAxis, add Axis x,y,z to axis array, see enumeration for order
	 * @param axis input enumaration of AXIS_X ..
	 */
	void addAxis(int axis);

	/**
	 * \brief setGridStep, sets the line grid step in Y
	 * @param step, input float
	 */
	void setGridStep(float step)		{mGridStep  = step;}

	/**
	 * \brief setGridLines,	set the amount of grid lines i Y
	 * @param nLines
	 */
	void setGridLines(int nLines)		{mGridLines = nLines;}

	/**
	 * \brief getGridStep, gets the grid step for Y between lines.
	 * @return grid step in Y
	 */
	float getGridStep()					{return mGridStep;}

	/**
	 * \brief getGridLines,	get the amount of lines in Y grid
	 * @return
	 */
	int	getGridLines()					{return mGridLines;}

	/**
	 * \brief init, initiate AxisMgr,
	 * \note using virtuals from Render class
	 */
	virtual void init();

	/**
	 * \brief draw, render all grid lines and axis lines.
	 * \note using virtuals from Render class
	 */
	virtual void draw();
};

}


#endif /* AXISMGR_H_ */

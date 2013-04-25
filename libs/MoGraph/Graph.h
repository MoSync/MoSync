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

#ifndef GRAPH_H_
#define GRAPH_H_
#include "IGraph.h"
#include "GFont.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shaders.h"
#include "RenderText.h"
#include "DTime.h"
#include "Touch.h"
#include "IRender.h"
#include "Scene.h"
#include "AxisMgr.h"
#include "BarMgr.h"
#include "TextMgr.h"
#include "GraphDesc.h"


// during development I like to keep all data public. cleanup will be done later.

/**
 * \brief MoGraph namespace
 * contains all graph and input of data classes into the same namespace..
 */

namespace MoGraph
{

class Scene;

// TODO use a screen class. for handling view port and offset positions so that we can support sub windows for rendering multiple views. (graphs)

/**
 * \brief Graph
 * This is the main interfaced class for the Graph lib.
 */

class Graph : public IGraph /* should include a interface here. */
{
protected:
	Scene 	mScene;				// Scene class, contains all scene objects and math
	IFont 	*mFont;				// Font to be used for text rendering
	int 	mWidth;				// screen width
	int 	mHeight;			// screen height
	int		mGridX;				// grid in X (graph area)
	int 	mGridZ;				// grid in Z (graph area)
	GraphDesc mDesc;			// copy of the description struct
	DTime	mDeltaTime;			// Delta time class (handles time in between ticks)
	Time	mTime;				// Elapsed time class (accumulated time since constr)
	glm::vec4 mBGColor;			// background color for graph
	glm::vec2 mViewPos;			// View Port Position for view port. \note glViewPort uses this and mWidth mHeigh for positioning the view.
	TouchInput mTouch;			// helper class for handling touch swipes and scales.
	RenderText	mRenderText;	// Text Render class thats fixes texts..

	/**
	 * \brief drawBars, draw all graph bars in scene
	 * @param tick, elapsed time tick
	 */
	void drawBars(float tick)	{mScene.getBarMgr().draw();}

	/**
	 * \brief drawAxis, draw all grid lines and axis in scene
	 * @param tick, elapsed time tick
	 */
	void drawAxis(float tick)	{mScene.getAxisMgr().draw();}

	/**
	 * \brief drawText, render all text in the scene
	 * @param tick, elapsed time tick
	 */
	void drawText(float tick);

	/**
	 * \brief initShaderLines, initiate shader for lines
	 */
	void initShaderLines()		{mScene.getAxisMgr().init();}

	/**
	 * \brief initShaderBars,	initiate shader for bars
	 */
	void initShaderBars()		{mScene.getBarMgr().init();}

	/**
	 * \brief initShaderText,	initiate shader for Text
	 */
	void initShaderText()		{mScene.getTextMgr().init();}

	/**
	 * \brief initGL,	initiate OpenGL basic stuff that required for MoGraph
	 * @return
	 */
	int  initGL();

public:
	/**
	 * \brief Graph,	Constructor
	 */
	Graph() :
		mFont(0),
		mWidth(1),
		mHeight(1),
		mGridX(1),
		mGridZ(1),
		mBGColor(0.0f,0.0f,0.0f,1.0f),
		mViewPos(0.0f,0.0f)
	{}

	/**
	 * \brief ~Graph,	Destructor
	 */
	virtual ~Graph() {}

	/**
	 * \brief GraphDesc, using the Graph Description struct
	 * to fill in the input values of the Graph
	 * @param desc, see GraphDesc struct
	 * @return
	 */
	virtual int init(GraphDesc *desc);

	/**
	 * \brief draw,	render entire scene
	 */
	virtual void draw();

	/**
	 * \brief setValues,	set the bar height value array
	 * @param values,	input pointer to a float array
	 * @param sz,	input size of array
	 */
	virtual void setValues(float *values,int sz) 			{mScene.setValues(values,sz);}

	/**
	 * \brief setColor,	set the bar color array
	 * @param colors,	input pointer to a glm::vec4 color array
	 * @param sz,	input size of color array
	 */
	virtual void setColors(glm::vec4 *colors, int sz) 		{mScene.setColors(colors,sz);}

	/**
	 * \brief setBGColor
	 * @param color
	 */
	virtual void setBGColor(glm::vec4 &color) 				{mBGColor = color;}

	/**
	 * \brief getScene,	get the reference to the scene class
	 * @return	scene
	 */
	virtual Scene &getScene() 								{return mScene;}

	/**
	 * \brief getTouch,	get the reference to the TouchInput helper class
	 * @return touch
	 */
	virtual TouchInput &getTouch()							{return mTouch;}
};

}
#endif /* GRAPH_H_ */

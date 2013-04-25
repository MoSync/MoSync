/*
 * BarMgr.h
 *
 *  Created on: Mar 14, 2013
 *      Author: CodeArt
 */

#ifndef BARMGR_H_
#define BARMGR_H_

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "IRender.h"
#include "Shaders.h"

namespace MoGraph
{
/**
 * \brief Bar class handles one single bar used by BarMgr for rendering
 */
	class scene;

	class Bar
	{
	protected:
		float 		mValue;
		glm::vec4 	mColor;

	public:
		/**
		 * \brief Bar Constructor
		 */
		Bar() : mValue(1.0f), mColor(1.0f,1.0f,1.0f,1.0f)	{}

		/**
		 * \brief setColor,	sets the r,g,b,a as float color component for the bar.
		 * @param r
		 * @param g
		 * @param b
		 * @param a
		 */
		void setColor(float r,float g,float b, float a)		{mColor.x = r; mColor.y = g; mColor.z = b; mColor.w = a;}

		/**
		 * \brief setColor, with glm::vec4 as input.
		 * @param col
		 */
		void setColor(glm::vec4 &col)						{mColor = col;}

		/**
		 * \brief setValue, bar height value in float
		 * @param val
		 */
		void setValue(float val) 							{mValue = val;}

		/**
		 * \brief getValue, get the bar height value
		 * @return output height value
		 */
		float getValue() 									{return mValue;}

		/**
		 * \brief getColor, get the bar color value
		 * @return output the bar color
		 */
		glm::vec4 & getColor() 								{return mColor;}
	};


	/**
	 * \brief BarMgr handles all the bars that are being displayed
	 *	using Render base class for the render calls init() & draw()
	 */

	class BarMgr : public Render
	{
	protected:
		std::vector<Bar> 			mBarArray;		// question absolute amount of bars or display fromc..to
		std::vector<glm::vec3> 		mVertices;
		std::vector<unsigned short> mFaces;
		BarShader					mShader;

		/**
		 * \brief create3D
		 * helper function to set the vertex array & index array
		 * Creates default vertex buffer & index buffer
		 */

		void create3D();			// create 3D obj. one instance of a cube with origo at bottom with indices
	public:

		/**
		 * \brief BarMgr Constructor
		 * @param scene input reference for the scene
		 */
		BarMgr(Scene *scene) : Render(scene) 	{create3D();}

		/**
		 * \brief ~BarMgr Destructor
		 */
		virtual ~BarMgr()						{}

		/**
		 * \brief addBars, 	set up amount of bars that shall be used for the graph
		 * @param sz		input amount of bars
		 */
		void addBars(int sz) 					{mBarArray.resize(sz);}

		/**
		 * \brief addbar,	add one bar to the table
		 * @param bar , adds this bar to the mBarArray table as last entry
		 */
		void addBar(Bar &bar) 					{mBarArray.push_back(bar);}

		/**
		 * \brief getBars,	get the table of all bars.
		 * @return
		 */
		std::vector<Bar> &getBars() 			{return mBarArray;}

		/**
		 * \brief size,	get the bars table size
		 * @return size of table
		 */
		int size()								{return mBarArray.size();}

		/**
		 * \brief getBar, get a element from the Bars array
		 * @param i,	element index
		 * @return Bar entry
		 */
		Bar &getBar(int i) 						{return mBarArray[i];}

		/**
		 * \brief common vertices used for all bars
		 * @return vector of vertices
		 */
		std::vector<glm::vec3>& vertices() 		{return mVertices;}

		/**
		 * \brief common index table of bars
		 * @return vector of faces
		 */
		std::vector<unsigned short>& faces() 	{return mFaces;}

		/**
		 * \brief getShader,
		 * @return
		 */
		BarShader &getShader() 					{return mShader;}

		// virtuals from render class
		/**
		 * \brief init, initiate bar manager class to set up the graph bars
		 * \note see virtuals from render class
		 */
		virtual void init();

		/**
		 * \brief draw, render bars on screen
		 * \note see virtuals from render class
		 */
		virtual void draw();
	};

}


#endif /* BARMGR_H_ */

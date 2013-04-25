/*
 * Scene.h
 *
 *  Created on: Mar 14, 2013
 *      Author: CodeArt
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "AxisMgr.h"
#include "BarMgr.h"
#include "TextMgr.h"
#include "GraphDesc.h"

namespace MoGraph{

/**
 * \brief Scene handling all object in scene.
 * holds scene matrices like Perspective Projection, View and World Matrices
 */
	class Scene
	{
	protected:
		bool 		mFitToScreen;		// fit to screen or use a std fixed view matrix.
		int 		mGridX;				// Bar grid X
		int 		mGridZ;				// Bar grid Z note mGridX*mGridZ = total amount of bars being used.
		glm::vec3	mGrid;				// floating point version of the Grid
		int			mWidth;
		int			mHeight;
		float 		mElapsedTime;		// Elapsed time in ms
		float 		mDistToCam;			// internally calculated for fit to screen.
		GraphDesc	mDesc;				// keep copy of the graph description
		glm::mat4 	mProjection;		// Projection Matrix
		glm::mat4 	mView;				// View Matrix
		glm::mat4 	mWorld;				// World Matrix
		glm::mat4 	mPVW;				// Projection * View * World Matrix

		AxisMgr 	mAxisMgr;			// Axis mangaer contains all Axis and Grid lineups dependent on graph type
		BarMgr  	mBarMgr;			// Bar manager contains all bars and vertices common for them
		TextMgr 	mTextMgr;			// Text manager handles fonts for text printouts.

		float		*mValues;			// Bar values
		int			mValuesSz;

		glm::vec4 	*mColors;			// Color values
		int			mColorsSz;

		glm::vec4 	mDefaultBarColor;			// default green color

		std::vector<unsigned short> mIndices;	// Index list of faces

	public:

		/**
		 * \brief Scene, Construction
		 */
		Scene();

		/**
		 * \brief create, whole scene by using Axis,Bars,Text
		 * @param GraphDesc, Graph Description struct contains all prefs for the graph
		 */
		void 	create(GraphDesc *desc);

		BarMgr 	&getBarMgr() 			{return mBarMgr;}
		AxisMgr &getAxisMgr()			{return mAxisMgr;}
		TextMgr &getTextMgr()			{return mTextMgr;}
		int 	getGridX()				{return mGridX;}
		int		getGridZ() 				{return mGridZ;}
		glm::vec3 &getGrid()			{return mGrid;}
		float	getCx()					{return -mGrid.x*0.5f;}	// GridX & GridZ needs a float type due to its conversion
		float	getCz()					{return mGrid.z*0.5f;}
		glm::mat4 & getProjectionMat() 	{return mProjection;}
		glm::mat4 & getViewMat()		{return mView;}
		glm::mat4 & getWorldMat()		{return mWorld;}
		glm::mat4 & getPVWMat()			{return mPVW;}
		void setWorldMat(glm::mat4 &m) 	{mWorld = m;}
		std::vector<unsigned short> &getIndices() {return mIndices;}
		void	setElapsedTime(float time)		{mElapsedTime = time;}
		float	getElapsedTime()		{return mElapsedTime;}
		void	setWidth(int w)			{mWidth = w;}
		int		getWidth()				{return mWidth;}
		void	setHeight(int h)		{mHeight = h;}
		int		getHeight()				{return mHeight;}
		void 	setDefaultBarColor(glm::vec4 color) {mDefaultBarColor = color;}
		const 	GraphDesc&	getGraphDesc()		{return mDesc;}
		float	getScaleFactor() 		 {return mGridX/500.0f;}
		/**
		 * \brief BoundScale, to get slightly larger bounds for lines or text
		 * in order not to be inside the graph objects.
		 * \note Axis/grid lines uses this scale for the lines.
		 * @return 1.01f
		 */
		float getBoundScale() const			{return 1.01f;}


		/**
		 * \brief updateMatrix, prepares a Perspective View World = PVW matrix used for the vertex shader.
		 */
		void updateMatrix();

		/**
		 * \brief updateCamera , update the ViewMatrix and PerspectiveProjection
		 * with new values like distance to the Camera
		 * @param scale
		 */
		void updateCamera(float scale);

		/**
		 * \brief setValues, set the height values pointer to an array
		 * \note data shall be contained according the the grid set up
		 * e.g. gridX = 10 gridZ=2 => first 10 entries are first row.
		 * and last 10 entries are second row in the graph like striding data
		 * gridX*z+x = index  (stride in width is same as gridX)
		 * @param values , graph height values
		 * @param sz , input size of array
		 */
		void setValues(float *values,int sz) 		{ mValues = values; mValuesSz = sz;}

		/**
		 * \brief setColors, set the color data for each bar.
		 * \note see setValues how data shall be stored.
		 * @param colors
		 * @param sz
		 */
		void setColors(glm::vec4 *colors, int sz) 	{mColors = colors; mColorsSz = sz;}

		/**
		 * \brief getValue, get a specific height value from the table
		 * @param i , input index of the values to get.
		 * @return value, height of bar
		 */
		float getValue(int i) 						{return (mValues)? mValues[i]: 1.0f;}

		/**
		 * \brief getColor, get a color from a specific bar
		 * @param i, input index entry for the bar to get the color from
		 * @return color object
		 */
		glm::vec4 & getColor(int i) 				{return (mColors)? mColors[i]: mDefaultBarColor;}

	};
}


#endif /* SCENE_H_ */

/*
 * Scene.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: CodeArt
 */

#include "Scene.h"
#include <mavsprintf.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include "Shaders.h"
#include "RenderText.h"
#include "GraphDesc.h"

namespace MoGraph
{

	/**
	 * \brief Scene, Construction
	 */

	Scene::Scene() :
			mFitToScreen(true),
			mGridX(1),
			mGridZ(1),
			mWidth(1),
			mHeight(1),
			mDistToCam(1.0f),
			mAxisMgr(this),
			mBarMgr(this),
			mTextMgr(this),
			mDefaultBarColor(0.25f,1.0f,0.25f,1.0f)
	{
	}

	/**
	 * \brief updateMatrix, prepares a Perspective View World = PVW matrix used for the vertex shader.
	 */
	void Scene::updateMatrix()
	{
		mPVW =  mProjection * mView * mWorld;
	}

	/**
	 * \brief create, whole scene by using Axis,Bars,Text
	 * @param gridX	,create amount of bars in X
	 * @param gridZ	,create amount of bars in Y
	 * @param lines	,amount lines in Y used for grid.
	 * @param step	,grid steps in Y
	 * @param bFitToScreen	,fit to screen flag, whole bars.
	 */
//	void Scene::create(int gridX, int gridZ, int lines, float step ,bool bFitToScreen)
	void Scene::create(GraphDesc *desc)
	{
lprintfln("flagGridLines=%d",desc->flagGridLines);
		mDesc				= *desc;			// store a copy of the graph description struct.
lprintfln("flagGridLines=%d",mDesc.flagGridLines);
		mFitToScreen 		= desc->bFitScreen;
		mGridX 				= desc->gridX;
		mGridZ 				= desc->gridZ;
		mGrid.x				= desc->gridX;
		mGrid.y				= 1.0f;
		mGrid.z				= desc->gridZ;
		int newSize 		= mGridX*mGridZ;

		lprintfln("Scene::create: %i*%i=%i",mGridX,mGridZ,newSize);
		mBarMgr.addBars(newSize);
		lprintfln("vector<Bar> bars, size() = %i == %i", mBarMgr.size(),newSize);

		int axis = ((mGridX>1)&&(mGridZ>1))?3:2;	// how many axis should be displayed 2 or 3 dependent on layout... 2d => 2 => 3d => 3
		mAxisMgr.addAxis(axis);
		mAxisMgr.setGridLines(desc->gridYLines);
		mAxisMgr.setGridStep(desc->gridStepYLines);
		mWorld = glm::mat4(1.0f);				// set up an identity matrix
		updateCamera(1.0f);
	}

	/**
	 * \brief updateCamera , update the ViewMatrix and PerspectiveProjection
	 * with new values like distance to the Camera
	 * @param scale
	 */
	void Scene::updateCamera(float scale)
	{
		const float width 	= (float)mWidth;	//EXTENT_X(maGetScrSize());
		const float height 	= (float)mHeight;	//EXTENT_Y(maGetScrSize());
		const float aspect 	= width/height;		// Net to calculate this this is HardCoded. 2.0f/3.0f;//1.0f/
		const float cx 		= getCx();
		const float cz 		= getCz();
		const float res 	= 1.0f/scale * glm::sqrt(cx*cx+cz*cz);
		mDistToCam 			= res;
		mProjection 		= glm::perspective(45.0f, aspect, 0.1f, res*5.0f); 		// Projection matrix : 45¡ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

		if (mFitToScreen == true)
		{												// Camera matrix
			mView       = glm::lookAt(
			glm::vec3(0.0f,res*0.5f,res*2.0f*(1.0f/aspect)), 	// Camera is at (0,x/2,x*(1/aspec)), in World Space
			glm::vec3(0,0,0), 							// and looks at the origin
			glm::vec3(0,1,0)  							// Head is up (set to 0,-1,0 to look upside-down)
			);
		}
		else											// set up a default camera matrix
		{
			mView       = glm::lookAt(
			glm::vec3(0.0f,10.0f,20.0f), 				// Camera is at (0,10,20), in World Space
			glm::vec3(0,0,0), 							// and looks at the origin
			glm::vec3(0,1,0)  							// Head is up (set to 0,-1,0 to look upside-down)
			);
		}
	}

}

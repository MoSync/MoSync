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

#include "AxisMgr.h"
#include "Graph.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <mavsprintf.h>

namespace MoGraph
{


	/**
	 * \brief AxisMgr::create3D  creates default vertex buffer
	 */
	void AxisMgr::create3D()
	{
		const float v[] =
		{
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		// set up 2D axis X and Y
		mAxisArray[0].vertices().push_back(glm::vec3(v[0],v[1],v[2]));
		mAxisArray[0].vertices().push_back(glm::vec3(v[3],v[4],v[5]));
		mAxisArray[1].vertices().push_back(glm::vec3(v[6],v[7],v[8]));
		mAxisArray[1].vertices().push_back(glm::vec3(v[9],v[10],v[11]));

		if(mAxisArray.size()>2)		// 3D Graph use a 3rd axis in Z.
		{
			mAxisArray[2].vertices().push_back(glm::vec3(v[12],v[13],v[14]));
			mAxisArray[2].vertices().push_back(glm::vec3(v[15],v[16],v[17]));
		}
		else
		{
			lprintfln("AxisMgr::create3d: ERROR Axis system got Graph bars can either be 2D or 3D hence 2 or 3 as input");   			// error not declared.
		}
	}

	/**
	 * \brief AxisMgr::addAxis
	 * @param int n, 	add new axis 1,2,3 dimensions. x,y,z
	 */
	void AxisMgr::addAxis(int n)
	{
		mAxisArray.resize(n);
		create3D();
	}

	void AxisMgr::handleGridLines(int startOffset, Line &ln, float centerX, float centerZ)
	{
		const GraphDesc &desc = mScene->getGraphDesc();
		ln.mWidth = 1;
		ln.mColor = glm::vec4(0.25f,0.25f,0.25f,1.0f);

		for (int l=startOffset;l<mGridLines;l++)
		{
			float gridY = static_cast<float>(l) * mGridStep;
			ln.mPos = glm::vec3(centerX, gridY, centerZ);

			switch (desc.flagGridLines)
			{
//				case DEFAULT_GRIDS:
//					break;
				case MIRRORED_GRIDS:
					mLineArray.push_back(ln);
					ln.mPos.y = -ln.mPos.y;
					mLineArray.push_back(ln);
					break;
				case OFFSET_GRIDS:
					ln.mPos.y = desc.gridOffsetStartLine + desc.gridStepYLines * l; //desc.gridYLines;
					mLineArray.push_back(ln);
					break;
				default:
					mLineArray.push_back(ln);
					break;
			}
		}
	}


	/**
	 * \brief AxisMgr::init
	 * initiates buffers for openGL for all Axis
	 */
	void AxisMgr::init()
	{
		mShader.init();
		for (size_t i=0; i<mAxisArray.size();i++)
		{
			Axis &axis = mAxisArray[i];								// get reference of obj.
			glGenBuffers(1, &mShader.mVertexbuffer[i]);				// Generate a vertex buffer for all axis (line)
			glBindBuffer(GL_ARRAY_BUFFER, mShader.mVertexbuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, axis.size() * sizeof(glm::vec3), &axis.vertices()[0], GL_STATIC_DRAW);
		}
		// build up the line structs in order to be able to let users add and change the grids by them selves.
		Line ln;
		glm::vec3 sv(1.0f,1.0f,1.0f);

		const float centerX = mScene->getCx()*mScene->getBoundScale();
		const float centerZ = mScene->getCz()*mScene->getBoundScale();
		const GraphDesc &desc = mScene->getGraphDesc();
		for(size_t i=0; i<mAxisArray.size(); i++)
		{
			float totGridHeight = desc.gridStepYLines * desc.gridYLines;
			ln.mAxisAlign 	= static_cast<Line::LineEnum>(i);
			ln.mType 		= Line::AXIS_LINE;
			ln.mWidth 		= 2;
			ln.mColor 		= glm::vec4(0.5f,0.5f,0.5f,1.0f);
			ln.mScale 		= glm::vec3(-centerX*2.0f,totGridHeight,-centerZ*2.0f);	// length is always abs
			ln.mPos 		= glm::vec3(centerX, 0.0f,centerZ);
			ln.mRotate		= glm::vec3(0.0f,0.0f,0.0f);
			int startOffset = 1;


			switch (desc.flagGridLines)
			{
//				case DEFAULT_GRIDS:
//					break;
				case MIRRORED_GRIDS:
					mLineArray.push_back(ln);
					ln.mScale.y = -ln.mScale.y;
					mLineArray.push_back(ln);
					startOffset = 1;
					break;
				case OFFSET_GRIDS:
					ln.mScale.y = desc.gridStepYLines * desc.gridYLines;
					if (i==1)
						ln.mPos.y	= desc.gridOffsetStartLine;
					mLineArray.push_back(ln);
					startOffset = 0;
					break;
				default:
					mLineArray.push_back(ln);
					startOffset = 1;
					break;
			}

			// Set up grid lines on height for X-Axis
			if (i == 0)
			{
				handleGridLines(startOffset, ln, centerX, centerZ);
			}
			else if (i==2)	// Set up grid lines in height for Z-Axis
			{
				handleGridLines(startOffset, ln, centerX, centerZ);
			}
		}
	}

	/**
	 * \brief AxisMgr::draw,	render axis Axis lines,
	 */
	void AxisMgr::draw()
	{
		const float tick = mScene->getElapsedTime();
		LineShader &shader 	= getShader();
		glDisable(GL_CULL_FACE);
		glUseProgram(shader.mShader);
		checkGLError("glUseProgram");

		glm::vec3 sv(1.0f,1.0f,1.0f);

		// Update variables to the shader, that is only updated commonly for all bars once per frame such as ParojactionMatrix, ViewMatrix, should be World Matrix aswell
		// projectionMatrix and viewMatrix tick time, resolution constants for pixel shader that are identical trough out the obj calls. hence update only once.
		glUniform1f(shader.mTimeLoc, tick);
		checkGLError("glUniform1f");
		glUniform2f(shader.mResolutionLoc, 1.0f/(float)mScene->getWidth(), 1.0f/(float)mScene->getHeight());
		checkGLError("glUniform2f");
		glUniformMatrix4fv(shader.mMatrixPVW, 1, GL_FALSE, &mScene->getPVWMat()[0][0]);
		checkGLError("glUniformMatrix4fv");
		glUniform3fv(shader.mScaleV,1, (float *)&sv.x);				// mScale location => variable "ScaleV" in vertex shader
		checkGLError("glUniform3fv");

		glEnableVertexAttribArray(shader.mAttribVtxLoc);

		for(size_t i = 0; i<mLineArray.size(); i++)
		{
			Line &ln = mLineArray[i];
			glBindBuffer(GL_ARRAY_BUFFER, shader.mVertexbuffer[ln.mAxisAlign]);
			glVertexAttribPointer(
				shader.mAttribVtxLoc,      // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
//			checkGLError("glEnableVertexAttribArray");
			glLineWidth(ln.mWidth);

			glm::vec4 tpos = glm::vec4(ln.mPos.x, ln.mPos.y, ln.mPos.z, 1.0f);
			glUniform4fv(shader.mTPos,1, (float *)&tpos.x);
			glUniform4fv(shader.mColor,1, (float *)&ln.mColor.x);
			glUniform3fv(shader.mLength,1, (float *)&ln.mScale.x);				// mScale location => variable "ScaleV" in vertex shader
			glDrawArrays(GL_LINES, 0, 2);
		}

		glDisableVertexAttribArray(shader.mAttribVtxLoc);
		glBindBuffer(GL_ARRAY_BUFFER,0);

		// Clean-up

		glUseProgram(0);
	}

}

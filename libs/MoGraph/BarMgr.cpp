/*
 * BarMgr.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: CodeArt
 */


#include "BarMgr.h"
#include "Graph.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <mavsprintf.h>

namespace MoGraph
{
	/**
	 * \brief create3D
	 * helper function to set the vertex array & index array
	 * Creates default vertex buffer & index buffer
	 */
	void BarMgr::create3D()
	{
			// Define quad vertices. origo Y = 0
		 const float Vertices[] = {
				1.0f, 	0.0f, 	1.0f,
				1.0f, 	1.0f, 	1.0f,
				-1.0f, 	1.0f, 	1.0f,
				-1.0f, 	0.0f, 	1.0f,
				1.0f, 	0.0f, 	-1.0f,
				1.0f, 	1.0f, 	-1.0f,
				-1.0f, 	1.0f, 	-1.0f,
				-1.0f, 	0.0f, 	-1.0f,
		};

		const unsigned short Indices[] = {
		    // Front
		    0, 1, 2,
		    2, 3, 0,
		    // Back
		    4, 6, 5,
		    4, 7, 6,
		    // Left
		    2, 7, 3,
		    6, 7, 2,
		    // Right
		    0, 4, 1,
		    1, 4, 5,
		    // Top
		    6, 2, 1,
		    5, 6, 1,
		    // Bottom
		    0, 3, 7,
		    0, 7, 4
		};

		for(int i=0; i<8*3; i+=3)
		{
			mVertices.push_back(glm::vec3(Vertices[i],Vertices[i+1],Vertices[i+2]));
		}

		for(int i=0; i<12*3; i++)
		{
			mFaces.push_back(Indices[i]);
		}
	}

	/**
	 * \brief BarMgr::init	prepare vertex buffers for openGL ES 2.0
	 */
	void BarMgr::init()
	{
		std::vector<unsigned short> & indices = getScene()->getIndices();	// Index list of faces

		// make a copy for now because we might need to add multiple indicies to this obj (there is only one index list)
		for(size_t i=0;i<mFaces.size();i++)
		{
			indices.push_back(mFaces[i]);
		}

		mShader.init();

		// Generate a buffer for the vertices
		glGenBuffers(1, &mShader.mVertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mShader.mVertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);

		// Generate a buffer for the indices
		glGenBuffers(1, &mShader.mElementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mShader.mElementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	}

	/**
	 * \brief BarMgr::draw,		render all bars for this frame
	 */
	void BarMgr::draw()
	{
		float tick = mScene->getElapsedTime();

		glEnable(GL_CULL_FACE);
		BarShader &shader = getShader();
		std::vector<unsigned short> &indices = mScene->getIndices();	// Index list of faces

		// Use the program object   shader and its specific arguments
		glUseProgram(shader.mShader);
		checkGLError("glUseProgram");

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(shader.mAttribVtxLoc);
		glBindBuffer(GL_ARRAY_BUFFER, shader.mVertexbuffer);
		glVertexAttribPointer(
			shader.mAttribVtxLoc,     	// attribute
			3,                  		// size
			GL_FLOAT,           		// type
			GL_FALSE,           		// normalized?
			0,                  		// stride
			(void*)0            		// array buffer offset
		);

		checkGLError("glEnableVertexAttribArray");

		// bind the Index buffer with vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.mElementbuffer);

		// Update variables to the shader, that is only updated commonly for all bars once per frame such as ParojactionMatrix, ViewMatrix, should be World Matrix aswell
		// projectionMatrix and viewMatrix tick time, resolution constants for pixel shader that are identical trough out the obj calls. hence update only once.
		glUniform1f(shader.mTimeLoc, tick);
		checkGLError("glUniform1f");
		glUniform2f(shader.mResolutionLoc, 1.0f/(float)mScene->getWidth(), 1.0f/(float)mScene->getHeight());
		checkGLError("glUniform2f");
		glUniformMatrix4fv(shader.mMatrixPVW, 1, GL_FALSE, &mScene->getPVWMat()[0][0]);

		// setting up a 2D grid.  prepare const variable for a tight loop
		const int iGridX	= mScene->getGridX();
		const int iGridZ	= mScene->getGridZ();
		const float centerX = mScene->getCx();
		const float centerZ = mScene->getCz();
		glm::vec3 sv(0.5f,0.5f,0.5f);
		glm::vec4 tpos(0.0f,0.0f,0.0f,1.0f);
		for(int j=0; j<iGridZ; j++)
		{
			for(int i=0; i<iGridX; i++)
			{
				const int id = j*iGridX+i;
				Bar &bar = getBar(id);
				tpos.x 	= (centerX+i)+0.5f;
				tpos.z 	= (centerZ-j)-0.5f;
				sv.y 	= bar.getValue();
				bar.setColor(mScene->getColor(id));				//	bar.setColor(i/30.0f,0.0f,j/30.0f,1.0f);
				bar.setValue(mScene->getValue(id));

				// upload our obj matrix to the vertex shader.
				glUniform4fv(shader.mTPos,		1, (float *)&tpos.x);
				glUniform3fv(shader.mScaleV,	1, (float *)&sv.x);				// mScale location => variable "ScaleV" in vertex shader
				glUniform4fv(shader.mColor,		1, (float *)&bar.getColor().x);
				glDrawElements(
					 GL_TRIANGLES,      	// mode
					 indices.size(),    	// count
					 GL_UNSIGNED_SHORT,   	// type
					 (void*)0           	// element array buffer offset
				 );
			}
		}

		// Clean-up
		glDisableVertexAttribArray(shader.mAttribVtxLoc);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}
}

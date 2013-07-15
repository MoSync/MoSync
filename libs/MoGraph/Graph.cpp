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

#include <mavsprintf.h>
#include "Graph.h"
#include "GFont.h"
#include "RenderText.h"
#include <maapi.h>
#include "GraphDesc.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// Name space Graph
namespace MoGraph
{

	/**
	 * Graph::draw,	render whole scene.
	 */
	void Graph::draw()
	{
		static int cnt = 0;
		cnt++;

		glViewport((int)mViewPos.x, (int)mViewPos.y, mWidth, mHeight);
		checkGLError("glViewport");
		mDeltaTime.tick();

		float tick = static_cast<float>(mTime.update()) * 0.001f;
		mScene.setElapsedTime(tick);

		// Clear the color buffer
		glClearColor(mBGColor.r,mBGColor.g,mBGColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkGLError("glClear");

		mTouch.update();
		mScene.updateCamera(mTouch.getScale());

		// working with angular speed
		const glm::vec3 rotSpeed = mTouch.getAngularSpeed();
		glm::vec3 axisY(0.0f,1.0f,0.0f);
		glm::mat4 mY = glm::rotate(rotSpeed.x*30.0f,axisY);
		glm::vec3 axisX(1.0f,0.0f,0.0f);
		glm::mat4 mX = glm::rotate(rotSpeed.y*30.0f,axisX);
		glm::vec3 axisZ(0.0f,0.0f,1.0f);
		glm::mat4 mZ = glm::rotate(rotSpeed.z*30.0f,axisZ);
		glm::mat4 mW = mScene.getWorldMat();
		glm::mat4 m = mZ*mY*mX*mW;
		mScene.setWorldMat( m );
		mScene.updateMatrix();		// need to update the PVW Matrix, Projection * View * World.

		glEnable(GL_DEPTH_TEST);

		drawBars(tick);
		drawAxis(tick);
		drawText(tick);

	}

	/**
	 * \brief GraphDesc, using the Graph Description struct
	 * to fill in the input values of the Graph
	 * @param desc, see GraphDesc struct
	 * @return
	 */

	int Graph::init(GraphDesc *desc)
	{
		mWidth 	= desc->scrWidth;
		mHeight = desc->scrHeight;
		mGridX	= desc->gridX;
		mGridZ	= desc->gridZ;
		mFont	= desc->font;
		mDesc	= *desc;	// keep copy of the description struct to keep track of all other init settings
		mScene.create(desc);
		mScene.setWidth(mWidth);
		mScene.setHeight(mHeight);
		mTouch.init(mWidth,mHeight);
		return initGL();
	}

	/**
	 * Graph::initGL,	initiates rendering system for the graph
	 * @return TRUE, TODO fix error check
	 */
	int Graph::initGL()
	{
		// Set up common gl options
		glViewport(0, 0, mWidth, mHeight);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// Enable back face culling
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		// set up clear color
		glClearColor(mBGColor.r,mBGColor.g,mBGColor.b, 1.0f);
		mRenderText.init(mWidth,mHeight,mFont);

		// create a braph with grid times grid
		initShaderBars();
		initShaderLines();
		initShaderText();

		return TRUE;
	}

/**
 * \brief Graph::drawText,	Text rendering for the Graph using TextMgr
 * @param tick, 	elapsed time since start in ms
 */
	void Graph::drawText(float tick)
	{
		TextMgr &textMgr = mScene.getTextMgr();
		TextProperty prop;

		for (int i=0; i<textMgr.size(); i++)
		{
			Text &text = textMgr.getText(i);
			mRenderText.setScale(text.mScale.x,text.mScale.y);
			glm::vec3 pos = text.mPos;
			switch (text.mTextFlagX)
			{
				case Text::CENTER_X:
					pos.x -= 0.5f * mRenderText.getTextWidth(text.mText.c_str());	// this function is expensive.
					break;
				case Text::CENTER_RIGHT:
					pos.x -= mRenderText.getTextWidth(text.mText.c_str());
					break;
				case Text::CENTER_LEFT:		// obsolete because it is by default
					break;
				default:
					break;
			}

			// Note the text height will be scaled with Y at shader level.
			switch (text.mTextFlagY)
			{
				case Text::CENTER_Y:
					pos.y -= 0.5f * mRenderText.getTextProperty(text.mText.c_str(),&prop);
					break;
				case Text::CENTER_BOTTOM:
					pos.y -= mRenderText.getTextProperty(text.mText.c_str(),&prop);
					break;
				case Text::CENTER_TOP:		// obsolete because it is by default
					break;
				default:
					break;
			}
			if (text.mRotate.x == 0.0f && text.mRotate.y == 0.0f && text.mRotate.z == 0.0f)
			{
				mRenderText.drawText3D(text.mText.c_str(), pos, text.mColor, mScene.getPVWMat(), mScene.getElapsedTime(), true);
			}
			else
			{
				glm::vec3 axis(text.mRotate.x?1.0f:0.0f, text.mRotate.y?1.0f:0.0f, text.mRotate.z?1.0f:0.0f);
				glm::mat4 m = mScene.getPVWMat() * glm::rotate(text.mRotate.y,axis);
				mRenderText.drawText3D(text.mText.c_str(), pos, text.mColor, m, mScene.getElapsedTime(), true);
			}
		}
	}
}

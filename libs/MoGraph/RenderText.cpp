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
#include "RenderText.h"
#include "Graph.h"
#include "GFont.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


/**
 * \brief RenderText,	Constructor
 */
RenderText::RenderText()
:	mFont(0),
	mWidth(0),
	mHeight(0),
	mScaleX(0),
	mScaleY(0),
	mBlendType(BL_ADDITIVE)
{
}

/**
 * \brief RenderText,	Destructor
 */
RenderText::~RenderText()
{
	release();
}

/**
 * \brief release, 	remove text vertex buffer cache if existing
 */
void RenderText::release()
{
	TextCacheTable::iterator it;
	for(it=mTextCache.begin();it!=mTextCache.end();it++)
	{
		VertStore &vstore = it->second;
		delete [] static_cast<glm::vec4 *>(vstore.mVertices);
		vstore.mVertices = 0;
	}
}

/**
 * \brief init,	initiate the text shader and parameters
 * @param width,	input screen width
 * @param height,	input screen height
 * @param font,		input useing font
 * @return bool true/false,	true=success false=failed (not it use)
 */

bool RenderText::init(float width, float height, IFont *font)
{
	mWidth		= width;
	mHeight		= height;
	mFont		= font;
	mTextShader.init();
	return true;
}

/**
 * \brief setFont,	set the desired for to be used for RenderText class
 * @param font
 */
void RenderText::setFont(IFont *font)
{
	mFont = font;
}

IFont *RenderText::getFont()
{
	return mFont;
}

/**
 * \brief getVertices,	get vertices for desired string,
 * this could either be created or reusing cached vertices
 * depends in the input parameters given
 * \note use releaseVertices when done
 * @param str,			string that we want to get or create vertex buffer from
 * @param bUseCache,	flag use cached vertexbuffer true=yes (reuse), false=no(recreate a new vertex buffer)
 * @param width,		output result width of the whole string in vertex buffer.
 * @return glm::vert4*  returns vertex buffer
 */
glm::vec4 *RenderText::getVertices(const char *str, bool bUseCache, float *width)
{
	glm::vec4 *vertices = 0;
	int num = 0;
	if (bUseCache)
	{
		std::string key = str;
		num				= key.size();
		TextCacheTable::iterator it = mTextCache.find(key);
		if (it == mTextCache.end())
		{										// doesn't exists in tabler, cache it.
			vertices = new glm::vec4[6*num];  	// create vertex array
			if(!vertices)						// check if memory alocation was ok
				maPanic(1,"RenderText: Failed to allocate vertex buffer");

			*width = mFont->BuildVertexArray(vertices, key.c_str(), mOPos.x, mOPos.y, mScaleX, mScaleY);			// get vertex array from string,

			glm::vec2 scaleXZ(mScaleX,mScaleY);
			VertStore vstore(vertices,*width,scaleXZ);

			if ( (mTextCache.insert(TextCachePair(key,vstore))).second == false )
			{
				// warn in log double insertion failed to insert this one.
				lprintfln("RenderText::TextCache insertion double entry detected of key=\"%s\"",key.c_str());
			}
		}
		else	// exists re use vertex buffer from table
		{
			VertStore &vstore = it->second;
			vertices = static_cast<glm::vec4 *>(vstore.mVertices);
			*width	 = vstore.mWidth;

			// may need to check current scale state or restore system with it.
			mScaleX = vstore.mScaleXZ.x;
			mScaleY = vstore.mScaleXZ.y;
		}
	}
	else	// Not using cache create one
	{
		num			= strlen(str);						// get number chars
		vertices	= new glm::vec4[6*num];
		if(!vertices)
			maPanic(1,"RenderText: Failed to allocate vertex buffer");

		*width = mFont->BuildVertexArray(vertices, str, mOPos.x, mOPos.y, mScaleX, mScaleY);			// get vertex array from string,
	}
	return vertices;
}

/**
 * \brief releaseVertices, use this function when vertexbuffer was retreived from getVertices
 * @param vertices,		vertex buffer to release
 * @param bUseCache,	if useing cache, true=useing cache (do nothing), false=no cache(delete vertex buffer)
 */
void RenderText::releaseVertices(glm::vec4 *vertices, bool bUseCache)
{
	if (bUseCache == false)
		delete [] vertices;
}

/**
 * \brief drawText3D,	render text in 3D space
 * @param str,			string to render
 * @param pos,			3D positions
 * @param rgba,			text color
 * @param pvw,			Perspective projection*View*World Matrix
 * @param tick			elapsed time
 * @param bUseCache		is useing cache for vertex buffer true/false
 * @return float width of the text
 */
float RenderText::drawText3D(const char*str, glm::vec3 &pos, glm::vec4 &rgba, glm::mat4 &pvw, float tick, bool bUseCache)
{
//	checkGLError("RenderText::DrawText   Should be ok!");

	float width = 0;
	mOPos 		= glm::vec3(0.0f,0.0f,0.0f);//pos;
	mPos		= pos;
	int num 	= strlen(str);
	glm::vec4 *vertices = 0;
	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	// Create the vertex array.

	// note uses m_pos creates or retrieves existing vertex buffer from a cache table
	vertices = getVertices(str, bUseCache, &width);

	TextShader &shader= mTextShader;

	glEnable (GL_BLEND);
	mBlendType = BL_ADDITIVE;
	switch (mBlendType)
	{
	case BL_ADDITIVE:					// Additive
		glBlendFunc(GL_ONE, GL_ONE);
//		glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR );	// very good additive
		break;
	case BL_TRANSLUCENT:				// Requires alpha
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Texture uses alpha
		break;
	case BL_SUBTRACTIVE:
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glBlendFunc( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA ); //subtractive black => max
//		glBlendFunc(GL_ZERO, GL_SRC_ALPHA);		// needs alpha
//		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		break;
	case BL_TRANSPARENT:
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		break;
	}

	// Use the program object
	glUseProgram(shader.mShader);
	checkGLError("RenderText::DrawText   glUseProgram (Text)");

	// 1. create buffers tri array for simplicity? so that every tri can be handled separately or with a new index buffer could be expensive though???
	// Load the vertex data
	glVertexAttribPointer(shader.mAttribVtxLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, &vertices[0].x);
	checkGLError("RenderText::DrawText   glVertexAttribPointer (V)");
	glEnableVertexAttribArray(shader.mAttribVtxLoc);
	checkGLError("RenderText::DrawText   glEnableVertexAttribArray (V)");
	glActiveTexture(GL_TEXTURE0 + 0);
	checkGLError("RenderText::DrawText   glActiveTexture");
	glBindTexture(GL_TEXTURE_2D, mFont->GetTexture(0));
	checkGLError("glBindTexture");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGLError("glTexParameteri");
	glUniform1i(shader.mTexture, 0 ); // Texture Unit 0
	checkGLError("RenderText::DrawText   glUniform1i");

	// Once per frame
	glUniform1f(shader.mTimeLoc, tick);
	checkGLError("RenderText::DrawText   glUniform1f");
	glUniform2f(shader.mResolutionLoc, 1.0f/(float)mWidth, 1.0f/(float)mHeight);
	checkGLError("RenderText::DrawText   glUniform2f");
	glUniformMatrix4fv(shader.mMatrixPVW, 1, GL_FALSE, &pvw[0][0]);
	checkGLError("RenderText::DrawText   glUniformMatrix4fv");
	glm::vec3 sv(1.0f, 1.0f, 1.0f);
	glUniform3fv(shader.mScaleV,1, (float *)&sv.x);				// mScale location => variable "ScaleV" in vertex shader
	checkGLError("RenderText::DrawText   glUniformMatrix3fv");
	glm::vec4 color = rgba;
	glUniform4fv(shader.mColor,1, (float *)&color.x);
	checkGLError("RenderText::DrawText   glUniformMatrix4fv");
	glm::vec4 TPos(mPos.x,mPos.y,mPos.z, 1.0f);
	glUniform4fv(shader.mTPos, 1, (float *)&TPos.x);
	checkGLError("RenderText::DrawText   glUniformMatrix4fv");
	glDrawArrays(GL_TRIANGLES, 0, 6*num);
	checkGLError("RenderText::DrawText   glDrawArrays");

	// Clean-up
	glDisableVertexAttribArray(shader.mAttribVtxLoc);
	checkGLError("RenderText::DrawText   glDisableVertexAttribArray (Vtx)");
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	checkGLError("RenderText::DrawText   glUseProgram(0)");

	releaseVertices(vertices, bUseCache);

	glDisable(GL_BLEND);

	return width;			// Generate a buffer for the vertices
}

/**
 * \brief drawText,		render 2D text on the screen (Orthogonal Projection)
 * @param str,			text string.
 * @param pos,			2D position on screen,
 * @param rgba,			text color
 * @param tick			time elapsed
 * @return float width 	of text string.
 */
float RenderText::drawText(const char*str, glm::vec3 &pos, glm::vec4 &rgba, float tick)
{
//	checkGLError("RenderText::DrawText   Should be ok!");
	float width = 0;
	mOPos = glm::vec3(0.0f,0.0f,0.0f);
	mOPos = pos;
	int num = strlen(str);
	glm::vec4 *vertices = 0;

	glDisable(GL_DEPTH_TEST);
	glm::mat4 m = glm::ortho(0.0f, static_cast<float>(mWidth),static_cast<float>(mHeight),0.0f,-200.0f,1000.0f);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	// Create the vertex array.

	mScaleY = -mScaleY;
	vertices = getVertices(str, false, &width);		// note uses m_pos creates or retrieves existing vertex buffer from a cache table
	mScaleY = -mScaleY;

	TextShader &shader= mTextShader;
	glEnable (GL_BLEND);
	mBlendType = BL_ADDITIVE;
	switch (mBlendType)
	{
	case BL_ADDITIVE:					// Additive
		glBlendFunc(GL_ONE, GL_ONE);
//		glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR );	// very good additive
		break;
	case BL_TRANSLUCENT:				// Requires alpha
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Texture uses alpha
		break;
	case BL_SUBTRACTIVE:
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glBlendFunc( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA ); //subtractive black => max
//		glBlendFunc(GL_ZERO, GL_SRC_ALPHA);		// needs alpha
//		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		break;
	case BL_TRANSPARENT:
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		break;
	}

	// Use the program object
	glUseProgram(shader.mShader);
	checkGLError("RenderText::DrawText   glUseProgram (Text)");

	// 1. create buffers tri array for simplicity? so that every tri can be handled separately or with a new index buffer could be expensive though???
	// Load the vertex data
	glVertexAttribPointer(shader.mAttribVtxLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, &vertices[0].x);
	checkGLError("RenderText::DrawText   glVertexAttribPointer (V)");
	glEnableVertexAttribArray(shader.mAttribVtxLoc);
	checkGLError("RenderText::DrawText   glEnableVertexAttribArray (V)");
	glActiveTexture(GL_TEXTURE0 + 0);
	checkGLError("RenderText::DrawText   glActiveTexture");
	glBindTexture(GL_TEXTURE_2D, mFont->GetTexture(0));
	checkGLError("glBindTexture");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGLError("glTexParameteri");
	glUniform1i(shader.mTexture, 0 ); // Texture Unit 0
	checkGLError("RenderText::DrawText   glUniform1i");

	// Once per frame
	glUniform1f(shader.mTimeLoc, tick);
	checkGLError("RenderText::DrawText   glUniform1f");
	glUniform2f(shader.mResolutionLoc, 1.0f/(float)mWidth, 1.0f/(float)mHeight);
	checkGLError("RenderText::DrawText   glUniform2f");
	glUniformMatrix4fv(shader.mMatrixPVW, 1, GL_FALSE, &m[0][0]);
	checkGLError("RenderText::DrawText   glUniformMatrix4fv");
	glm::vec3 sv(1.0f, 1.0f, 1.0f);
	glUniform3fv(shader.mScaleV,1, (float *)&sv.x);				// mScale location => variable "ScaleV" in vertex shader
	checkGLError("RenderText::DrawText   glUniformMatrix3fv");
	glm::vec4 color = rgba;
	glUniform4fv(shader.mColor,1, (float *)&color.x);
	checkGLError("RenderText::DrawText   glUniformMatrix4fv");
	glm::vec4 TPos(mPos.x,mPos.y,mPos.z, 1.0f);
	glUniform4fv(shader.mTPos, 1, (float *)&TPos.x);
	checkGLError("RenderText::DrawText   glUniformMatrix4fv");
	glDrawArrays(GL_TRIANGLES, 0, 6*num);
	checkGLError("RenderText::DrawText   glDrawArrays");

	// Clean-up
	glDisableVertexAttribArray(shader.mAttribVtxLoc);
	checkGLError("RenderText::DrawText   glDisableVertexAttribArray (Vtx)");
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	checkGLError("RenderText::DrawText   glUseProgram(0)");

	releaseVertices(vertices, false);
	glDisable(GL_BLEND);

	return width;			// Generate a buffer for the vertices
}

/**
 * \brief getTextWidth, get a text width from string
 *
 * @param str,	input string
 * @return float width,	text width
 */
float RenderText::getTextWidth(const char *str)
{
	TextProperty property;
	return mFont->getTextProperties(str, 0.0f, 0.0f, mScaleX, mScaleY,&property);		// get vertex array from string,
}

/**
 * \brief getTextHeight, get a text Height from string
 *
 * @param str,	input string
 * @return float height,	text width
 */
float RenderText::getTextHeight(const char *str)
{
	TextProperty property;
	mFont->getTextProperties(str, 0.0f, 0.0f, mScaleX, mScaleY,&property);				// get vertex array from string,
	return property.sHeight;
}

/**
 * \brief getTextProperty, get text property containing width height of the text and the current text line height scale. from string
 *
 * @param str,	input string
 * @param property, output property struct containing width height line height.
 * @return float text line height
 */
float RenderText::getTextProperty(const char *str, TextProperty *property)
{
	mFont->getTextProperties(str, 0.0f, 0.0f, mScaleX, mScaleY,property);				// get vertex array from string,
	return property->sLineHeight;
}

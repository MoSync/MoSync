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

#ifndef TEXT_H_
#define TEXT_H_

#include "GFont.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include "Shaders.h"
#include "Utils.h"

struct VertStore
{
	VertStore(void *vertices, float width, glm::vec2 &scaleXZ) : mVertices(vertices), mWidth(width), mScaleXZ(scaleXZ) {};

	void 		*mVertices;
	float		mWidth;
	glm::vec2 	mScaleXZ;
};

typedef std::pair<std::string, VertStore > 	TextCachePair;
typedef std::hash_map<std::string,VertStore > 	TextCacheTable;

enum BlendType{ BL_ADDITIVE, BL_TRANSLUCENT, BL_SUBTRACTIVE, BL_TRANSPARENT};

/**
 * \brief RenderText class generic text rendering class based on the output
 * \note RenderText Class can handle one Texture for the whole font.
 * This is due to speedup of the rendering system to avoid texture changes and multiple texture switching within one string.
 */

class RenderText
{
public:
	/**
	 * \brief RenderText,	Constructor
	 */
	RenderText();
	/**
	 * \brief ~RenderText,	Destructor
	 */
	virtual ~RenderText();
	/**
	 * \brief init,	initiate the RenderText
	 * @param screenWidth
	 * @param screenHeight
	 * @param font,			pointer to the font to use, for rendering
	 * @return
	 */
	virtual bool init(float screenWidth, float screenHeight,IFont *font=0);
	/**
	 * \brief setFont,	set the pointer for the font to be used
	 * @param font,	input pointer to font
	 */
	virtual void setFont(IFont *font);
	/**
	 * \brief getFont,	get's the currently used font for the RenderText.
	 * @return
	 */
	virtual IFont *getFont();
	/**
	 * \brief drawText3D,	draws text in the graph (3d space)
	 * @param str,			output string
	 * @param pos,			position in 3D space
	 * @param rgba,			color
	 * @param projectionViewWorldMat,	combined matrix containing Projection View World
	 * @param tick,			Time tick elapsed time
	 * @param bUseCache,	Use cacheing of vertex buffer for string, string is key for lookup the vertex buffer.
	 * @return float width of the text
	 */
	virtual float drawText3D(const char*str,glm::vec3 &pos, glm::vec4 &rgba, glm::mat4 &projectionViewWorldMat, float tick = 0, bool bUseCache = false );
	/**
	 * \brief drawText,		render 2D text on the screen (Orthogonal Projection)
	 * @param str,			text string.
	 * @param pos,			2D position on screen, TODO support Z as priority for future reference, but for now not used
	 * @param rgba,			text color
	 * @param tick			time elapsed
	 * @return float width 	of text string.
	 */
	virtual float drawText(const char *str,glm::vec3 &pos, glm::vec4 &rgba, float tick = 0.0f);
	/**
	 * \brief getTextWidth, get a text width from string
	 * \note recreates a temporary vertex buffer due to the scaling factors could be different.
	 * expensive function
	 *
	 * @param str,	input string
	 * @return float width,	text width
	 */
	virtual float getTextWidth(const char *str);
	/**
	 * \brief getTextHeight, get a text Height from string
	 *
	 * @param str,	input string
	 * @return float height,	text width
	 */
	virtual float getTextHeight(const char *str);
	/**
	 * \brief getTextProperty, get text property containing width height of the text and the current text line height scale. from string
	 *
	 * @param str,	input string
	 * @param property, output property struct containing width height line height.
	 * @return text line height
	 */
	virtual float getTextProperty(const char *str, TextProperty *property);
	/**
	 * \brief setScale,	scales the text.
	 * @param x,
	 * @param y,
	 */
	virtual void setScale(float x,float z) {mScaleX = x; mScaleY = z;}
	/**
	 * \brief getScale, get the used scale for text render
	 * @return glm::vec2, (x,y)
	 */
	virtual glm::vec2 getScale() {return glm::vec2(mScaleX,mScaleY);}
	/**
	 * \brief getLineHeight, gets the scaled height of one text line (general height for font)
	 * use full for use to set next line, by adding return value with a pos Y
	 * @return lineHeight,  fonts line height * scale of Y for render text
	 */
	virtual float getLineHeight()  {return (mFont)? mFont->getCommon()->m_lineHeight * mScaleY: 0.0f;}

protected:
	/**
	 * \brief release, internal function for releasing the vertex buffered cache.
	 */
	void release();
	/**
	 * \brief getVertices,	get the vertex buffer for the a string
	 * @param str,			string used as a look up key
	 * @param bUseCache,	using cache vertex buffers ends up in a hash_map and will be reused, if not it will be recreated every time
	 * @param width,		output: width of the text.
	 * @return vertex buffer
	 */
	glm::vec4 *getVertices(const char *str, bool bUseCache, float *width);	// helper function to create or retrieved cached buffer
	/**
	 * \brief releaseVertices, 	when using getVertices after usage a release Vertices has to be called.
	 * @param vertices,			input: vertex buffer to be released
	 * @param bUseCache			input: using cache boolean true/false
	 */
	void releaseVertices(glm::vec4 *vertices, bool bUseCache);

	IFont			*mFont;		// current pointer for the font to use
	float			mWidth;		// screen width
	float			mHeight;		// screen height
	float 			mScaleX;		// used for scaling text in X
	float			mScaleY;		// used for scaling text in Y
	glm::vec3		mOPos;			// position of text. (origo), disposition the origo within the text.
	glm::vec3		mPos;			// position of text in 3D space
	TextShader		mTextShader;	// Text shader to be used for the rendering
	TextCacheTable	mTextCache;	// using text cache re using vertex buffers for specific text.
	BlendType		mBlendType;	// blend parameters for th text.
};

#endif /* TEXT_H_ */

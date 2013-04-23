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

#ifndef IFONT_H_
#define IFONT_H_

#include <glm/glm.hpp>
#include <vector>

struct BMCommon;
struct BMInfo;

struct TextProperty
{
	float sWidth;
	float sHeight;
	float sLineHeight;
};


/**
 * \brief Interface for any Font class to use.
 */
class IFont
{
public:
	/**
	 * \brief ~IFont, Destructor
	 */
	virtual ~IFont() {};
	/**
	 * \brief init,	initiate the font system with resources
	 * for a font format and array of pictures
	 * \note at this point we only support one texture per font,
	 * but prepared for future support. all other entries will be ignored.
	 * @param fontResource,		resource for a font specification,
	 * a file that describes the font could include its size
	 * texture coordinates spacing kernings etc.
	 * @param textureResources, array of resource for texture as any valid picture format.
	 * @return bSuccess, true/false
	 */
	virtual bool Init(MAHandle fontResource, std::vector<MAHandle> &textureResources) = 0;

	/**
	 * \brief Clear,
	 */
	virtual void Clear() = 0;

	/**
	 * \brief GetTexture,	get shader texture entry id
	 * @param i,	input index to array
	 * @return	GLuint shader entry id.
	 */
	virtual GLuint GetTexture(int i=0) = 0;

	/**
	 * \brief getTextProperties calculates the vertex array from any given string containing this BMFont.
	 * @param sentence char* string, to build a vertex table from
	 * @param drawX, position in X
	 * @param drawY, position in Y
	 * @param scaleX, scale in X
	 * @param scaleY, scale in Y
	 * @param property, output vec2 => width , height of text.
	 * @return string width
	 */
	virtual float getTextProperties(const char* sentence, float drawX, float drawY, float scaleX, float scaleY,TextProperty *property) = 0;

	/**
	 * \brief BuildVertexArray, builds up the vertex array from text.
	 * @param vertexPtr
	 * @param str
	 * @param dX
	 * @param dY
	 * @param scaleX
	 * @param scaleY
	 * @return width, output width of the string from vertices .
	 */
	virtual float BuildVertexArray(glm::vec4 * vertexPtr, const char * str, float dX, float dY, float scaleX, float scaleY) = 0;

	/**
	 * \brief getCommon information for the font
	 * @return struct of BMCommon
	 */
	virtual const BMCommon *getCommon() = 0;

	/**
	 * \brief getInfo information
	 * @return struct BMInfo
	 */
	virtual const BMInfo *getInfo() = 0;
};

#endif /* IFONT_H_ */

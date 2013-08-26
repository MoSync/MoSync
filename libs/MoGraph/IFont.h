/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

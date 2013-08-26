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

#ifndef SHADERS_H_
#define SHADERS_H_

#include <GLES2/gl2.h>


/**
 * \brief, base class for Shader
 */
class Shader
{
public:
	/**
	 * \brief ~Shader,	Destuctor
	 */
	virtual ~Shader(){}
	/**
	 * \brief init,	initiate loads shader and setsup each parameter locations
	 */
	virtual void init() 		= 0;	// initiate loads shader
};

/**
 * \brief BarShader class, Shader class for holding its specific parameters.
 */
struct BarShader : public Shader
{
	/**
	 * \brief BarShader, Constructor
	 */
	BarShader() {}				// NOTE common problem do not initiate in constr.
	/**
	 * \brief ~BarShader, Destructor
	 */
	virtual ~BarShader(){}

	GLuint 	mShader;			// shader for bars
	GLuint 	mAttribVtxLoc;		// Attribute to the vertex shader of vpos location
	GLuint 	mTimeLoc;			// time tick variable for shaders (fragment)
	GLuint 	mResolutionLoc;		// screen resulution
	GLuint 	mMatrixPVW;			// Shader Perspective Projection
	GLuint	mTPos;				// Translate position
	GLuint 	mScaleV;			// Scale vector of bars. its height...
	GLuint	mColor;				// individual color of a bar.
	GLuint 	mElementbuffer;		// Element buffer holding the index buffer for bars
	GLuint 	mVertexbuffer;		// Vertex buffer for bars. (cube)

	/**
	 * \brief,	init, initiate loads shader and setsup each parameter locations
	 */
	virtual void init();
};

/**
 * \brief LineShader class
 */
struct LineShader : public Shader
{
	/**
	 * \brief LineShader, Constructor
	 */
	LineShader() {}
	/**
	 * \brief ~LineShader, Destructor
	 */
	virtual ~LineShader(){}

	// Line location parameters
	GLuint 	mShader;			// shader for bars
	GLuint 	mAttribVtxLoc;		// Attribute to the vertex shader of vpos location
	GLuint 	mTimeLoc;			// time tick variable for shaders (fragment)
	GLuint 	mResolutionLoc;		// screen resulution
	GLuint 	mMatrixPVW;			// Shader Perspective * View * World
	GLuint  mTPos;				// Translation position
	GLuint 	mScaleV;			// Scale vector of bars. its height...
	GLuint	mColor;				// individual color of a bar.
	GLuint	mLength;			// Shader Line Length (line scales up 0..LineLength)
	GLuint 	mVertexbuffer[3];		// Vertex buffer for Axis aligned lines. (world)
	/**
	 * \brief,	init, initiate loads shader and setsup each parameter locations
	 */
	virtual void init();
};

/**
 * \brief TextShader class
 */
struct TextShader : public Shader
{
	/**
	 * \brief TextShader, Constructor
	 */
	TextShader() {}
	/**
	 * \brief ~TextShader, Destructor
	 */
	virtual ~TextShader(){}

	GLuint 	mShader;			// shader for Text
	GLuint 	mAttribVtxLoc;		// Attribute to the vertex shader of vpos location
	GLuint 	mTimeLoc;			// time tick variable for shaders (fragment)
	GLuint 	mResolutionLoc;		// screen resulution
	GLuint 	mMatrixPVW;			// Shader Perspective Projection
	GLuint  mTPos;				// Translation position
	GLuint 	mScaleV;			// Scale vector of bars. its height...
	GLuint	mColor;				// individual color of a Text.
	GLuint	mTexture;			// Texture for the font
	/**
	 * \brief,	init, initiate loads shader and setsup each parameter locations
	 */
	virtual void init();
};

// Helper functions for common use.
/**
 * \brief checkGLError, checks basic gl error message.
 * @param where, input string from the system
 */
void checkGLError(const char* where);									// Common GL error handling

/**
 * \brief loadShader, load shader function
 * @param shaderSrc
 * @param type
 * @return
 */
GLuint loadShader(const char *shaderSrc, GLenum type);					// Loads one type of shader

/**
 * \brief loadShaders, mother function that handles both vertex and fragment shader.
 * @param shader_vtx
 * @param shader_frg
 * @return
 */
GLuint loadShaders(const char *shader_vtx, const char *shader_frg);		// Loads both vertex and fragment shader


#endif /* SHADERS_H_ */

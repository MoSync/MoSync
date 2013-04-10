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
#include <maheap.h>

#include "Shaders.h"

#define STRINGIFY(A)  #A

//--------------------------------------------------------------------------------------
// BARS SHADERS
//--------------------------------------------------------------------------------------
/**
 * \brief fragmentShaderBars, 	pixel shader for rendering bars
 */
// BARS FRAGMENT SHADER
	char fragmentShaderBars[]=STRINGIFY(
		precision lowp 	float;
		varying vec4 	v_color;
		uniform vec2 	resolution;
		uniform float 	time;
		void main( void )
		{
			gl_FragColor = v_color;
		}
	);

/**
 * \brief vertexShaderBars,		vertex shader for building up the bars in 3D space.
 */
	// BARS VERTEX SHADER
	char vertexShaderBars[]=STRINGIFY(
		attribute vec4 vPosition;
		uniform mat4 ProjViewWorld;
		uniform vec4 TPos;
		uniform vec3 ScaleV;
		uniform vec4 Color;
		varying vec4 v_color;
		void main( void )
		{
			mat4 sm  	= mat4(1.0);
			sm[3][0]	= TPos.x;
			sm[3][1]	= TPos.y;
			sm[3][2]	= TPos.z;
			if (ScaleV.y >= 0.0)
			{
				sm[0][0] 	= ScaleV.x;
				sm[1][1] 	= ScaleV.y;
				sm[2][2] 	= ScaleV.z;
			}
			else
			{
				sm[0][0] 	= -ScaleV.x;
				sm[1][1] 	= ScaleV.y;
				sm[2][2] 	= ScaleV.z;
			}
			v_color  	= vec4(Color.x*vPosition.y, Color.y*vPosition.y, Color.z*vPosition.y, Color.w);
			gl_Position = (ProjViewWorld * sm) * vPosition;
		}
	);

//--------------------------------------------------------------------------------------
// LINES SHADERS
//--------------------------------------------------------------------------------------
	/**
	 * \brief fragmentShaderLines,		pixel shader for handling the Axis/Lines
	 */
	char fragmentShaderLines[]=STRINGIFY(
		precision lowp 	float;
		varying vec4 	v_color;
		uniform vec2 	resolution;
		uniform float 	time;
		void main( void )
		{
			gl_FragColor = v_color;
		}
	);

	/**
	 * \brief vertexShaderLines,	vertex shader for handling vertex positions in 3D space.
	 */
	char vertexShaderLines[]=STRINGIFY(
		attribute vec4 vPosition;
		uniform mat4 ProjViewWorld;
		uniform vec4 TPos;
		uniform vec3 ScaleV;
		uniform vec4 Color;
		uniform vec3 Length;
		varying vec4 v_color;
		void main( void )
		{
			mat4 sm  	= mat4(1.0);
			sm[3][0]	= TPos.x;
			sm[3][1]	= TPos.y;
			sm[3][2]	= TPos.z;
			sm[0][0] 	= ScaleV.x;
			sm[1][1] 	= ScaleV.y;
			sm[2][2] 	= ScaleV.z;

			v_color = Color;
			gl_Position = (ProjViewWorld * sm) * (vPosition * vec4(Length.xyz,1.0));
		}
	);


//--------------------------------------------------------------------------------------
// LINES SHADERS
//--------------------------------------------------------------------------------------
	/**
	 * fragmentShaderText,		pixel shader for text, using texture
	 */
	char fragmentShaderText[]=STRINGIFY(
		precision highp 	float;
		varying highp vec2	v_tex;
		varying vec4 		v_color;
		uniform vec2 		resolution;
		uniform float 		time;
		uniform sampler2D 	myTexture;
		void main( void )
		{
			vec4 texval 	= texture2D(myTexture, v_tex);
			gl_FragColor 	= texval*v_color;
		}
	);

	/**
	 * vertexShaderText,	vertex shader handling vertex positions in 3D space
	 */
	char vertexShaderText[]=STRINGIFY(
		attribute vec4 	vPosition;
		uniform mat4 	ProjViewWorld;
		uniform vec4 	TPos;
		uniform vec3 	ScaleV;
		uniform vec4 	Color;
		varying highp 	vec2 v_tex;
		varying vec4 	v_color;
		void main( void )
		{
			mat4 sm  	= mat4(1.0);
			sm[3][0]	= TPos.x;
			sm[3][1]	= TPos.y;
			sm[3][2]	= TPos.z;
			sm[0][0] 	= ScaleV.x;
			sm[1][1] 	= ScaleV.y;
			sm[2][2] 	= ScaleV.z;

			v_tex	 	= vPosition.zw;
			v_color 	= Color;
			gl_Position = (ProjViewWorld * sm) * vec4(vPosition.xy, 0.0, 1.0);

		}
	);


	// HELPER FUNCTIONS FOR LOADING SHADER & Error detection
	//----------------------------------------------------------------

	/**
	 * \brief checkGLError,	general helper function to print visually the error msg in debug window
	 * @param where
	 */
	void checkGLError(const char* where)
	{
		GLenum err = glGetError();
		switch (err)
		{
			case GL_NO_ERROR: break;
			case GL_INVALID_ENUM:  lprintfln("%s: glGetError returned %x = GL_INVALID_ENUM", where, err);				break;
			case GL_INVALID_VALUE: lprintfln("%s: glGetError returned %x = GL_INVALID_VALUE", where, err);				break;
			case GL_INVALID_OPERATION:  lprintfln("%s: glGetError returned %x = GL_INVALID_OPERATION", where, err);		break;
			case GL_OUT_OF_MEMORY:  lprintfln("%s: glGetError returned %x = GL_OUT_OF_MEMORY", where, err);				break;
			default: lprintfln("%s: glGetError returned %x", where, err); 												break;
		}
	}


	/**
	 * \brief loadShader helper function that loads a shader and compiles it.
	 * @param shaderSrc,	input string containing shader source
	 * @param type,			type of shader to compile
	 * @return shader index, 0=fail
	 */
	GLuint loadShader(const char *shaderSrc, GLenum type) {
		lprintfln("loading shader: %s", shaderSrc);
		checkGLError("begin loading shader");

		GLuint shader;
		GLint compiled;
		// Create the shader object
		shader = glCreateShader(type);
		checkGLError("glCreateShader");

		if (shader == 0) {
			lprintfln("Invalid shader handle!");
			return 0;
		}
		// Load the shader source
		glShaderSource(shader, 1, &shaderSrc, NULL);
		checkGLError("glShaderSource");

		// Compile the shader
		glCompileShader(shader);
		checkGLError("glCompileShader");

		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		checkGLError("glGetShaderiv");

		if (!compiled) {
			lprintfln("Error compiling shader");
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen == 0) // android bug.
				infoLen = 1024;
			if (infoLen > 1) {
				char* infoLog = (char*) malloc(sizeof(char) * infoLen);
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				lprintfln("Error compiling shader:\n%s\n", infoLog);
				free(infoLog);
			}
			glDeleteShader(shader);
			return 0;
		}

		checkGLError("Shader compiled");

		return shader;
	}

	/**
	 * \brief loadShaders,	Loads both Vertex and Fragment Shader into one shader setup, and link to a shader program
	 * @param shader_vtx	input string of vertex shader
	 * @param shader_frg	input string of fragment shader
	 * @param isUsingTexCoord	input bool ctrl flag for if using texture coord. requires separate init
	 * @return program object index, 0 = failure
	 */
	GLuint loadShaders(const char *shader_vtx, const char *shader_frg, bool isUsingTexCoord)
	{
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint programObject;
		GLint linked;


		// Load the vertex/fragment shaders
		vertexShader = loadShader(shader_vtx, GL_VERTEX_SHADER);
		checkGLError("Load vertex shader");

		fragmentShader = loadShader(shader_frg, GL_FRAGMENT_SHADER);
		checkGLError("Load fragment shader");

		// Create the program object
		programObject = glCreateProgram();
		if (programObject == 0) {
			lprintfln("Could not create program!");
			return FALSE;
		}
		checkGLError("Create program");

		glAttachShader(programObject, vertexShader);
		checkGLError("Attach vertex shader");

		glAttachShader(programObject, fragmentShader);
		checkGLError("Attach fragment shader");

		// Bind vPosition to attribute 0
		glBindAttribLocation(programObject, 0, "vPosition");
		checkGLError("Bind 0 vPosition to vertex shader");

		if (isUsingTexCoord)
		{
			glBindAttribLocation(programObject, 1, "vTexCoord");
			checkGLError("Bind 1 vTexCoord to vertex shader");
		}

		// Link the program
		glLinkProgram(programObject);

		// Check the link status
		glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
		if (!linked) {
			lprintfln("Failed to link shader!");
			GLint infoLen = 0;
			glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen == 0) // android bug.
				infoLen = 1024;
			if (infoLen > 1) {
				char* infoLog = (char*) malloc(sizeof(char) * infoLen);
				glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
				//esLogMessage("Error linking program:\n%s\n", infoLog);
				lprintfln("Error linking program:\n%s\n", infoLog);
				free(infoLog);
			}
			glDeleteProgram(programObject);
			return FALSE;
		}
		// Store the program object
		return programObject;
	}

/**
 * \brief LineShader::init,  initiates by loading vertex & fragment shader.
 * Contains all indexed references to locations of parameters.
 */
void LineShader::init()
{
	mShader 		= loadShaders(vertexShaderLines,fragmentShaderLines,false);
	if (mShader == 0)
	{
		checkGLError("loadShader LineShader");
		lprintfln("LineShader::init: failed");
		maPanic(1,"Failed to init LineShader");
	}

	mTimeLoc 		= glGetUniformLocation(mShader, "time");
	mResolutionLoc 	= glGetUniformLocation(mShader, "resolution");
	mMatrixPVW 		= glGetUniformLocation(mShader, "ProjViewWorld");
	mTPos			= glGetUniformLocation(mShader, "TPos");
	mScaleV			= glGetUniformLocation(mShader, "ScaleV");
	mColor			= glGetUniformLocation(mShader, "Color");			// Color of line (vertex shader)
	mLength			= glGetUniformLocation(mShader, "Length");
	mAttribVtxLoc	= glGetAttribLocation( mShader, "vPosition");
	lprintfln("LineShader::init: initiate");
}

/**
 * \brief BarShader::init() initiates by loading vertex & fragment shader for Bars.
 * contains all indexed references to locations of parameters.
 */
void BarShader::init()
{
	mShader 		= loadShaders(vertexShaderBars,fragmentShaderBars,false);
	if (mShader == 0)
	{
		checkGLError("loadShader BarShader");
		lprintfln("BarShader::init: failed");
		maPanic(1,"Failed to init BarShader");
	}
	mTimeLoc 		= glGetUniformLocation(mShader, "time");			// time tick variable (fragment)
	mResolutionLoc 	= glGetUniformLocation(mShader, "resolution");		// constant resolution of screen (fragment)
	mMatrixPVW 		= glGetUniformLocation(mShader, "ProjViewWorld");	// Projection*View*World Matrix
	mTPos			= glGetUniformLocation(mShader, "TPos");			// Translate vector postion
	mScaleV			= glGetUniformLocation(mShader, "ScaleV");			// scale vector (height of bar)
	mColor			= glGetUniformLocation(mShader, "Color");			// Color of one bar (vertex shader)
	mAttribVtxLoc	= glGetAttribLocation( mShader, "vPosition");		// input vertex attrib
	lprintfln("BarShader::init: initiate");
}

/**
 * \brief TextShader::init() ,initiates by loading vertex & fragment shader for Text.
 * contains all indexed references to locations of parameters.
 */
void TextShader::init()
{
	mShader 		= loadShaders(vertexShaderText,fragmentShaderText,true);
	if (mShader == 0)
	{
		checkGLError("loadShader TextShader");
		lprintfln("TextShader::init: failed");
		maPanic(1,"Failed to init TextShader");
	}
	mTimeLoc 		= glGetUniformLocation(mShader, "time");			// time tick variable (fragment)
	mResolutionLoc 	= glGetUniformLocation(mShader, "resolution");		// constant resolution of screen (fragment)
	mMatrixPVW 		= glGetUniformLocation(mShader, "ProjViewWorld");	// Projection Matrix
	mTPos			= glGetUniformLocation(mShader, "TPos");			// Projection*View*World Matrix
	mScaleV			= glGetUniformLocation(mShader, "ScaleV");			// scale vector (height of bar)
	mColor			= glGetUniformLocation(mShader, "Color");			// Color of one bar (vertex shader)
	mTexture		= glGetUniformLocation(mShader, "myTexture");
	mAttribVtxLoc	= glGetAttribLocation( mShader, "vPosition");		// input stream vertex attrib
	lprintfln("TextShader::init: initiate");
}

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

#include <MAUtil/GLMoblet.h>
#include <GLES2/gl2.h>
#include <maheap.h>
#include <conprint.h>

using namespace MAUtil;

/**
 * A OpenGL|ES 2.0 GLMoblet application that
 * draws a quad covering the whole screen with
 * a fragment shader that generates a trippy
 * pattern. Shows how to load and create
 * shaders, do more advanced error handling,
 * draw primitives using OpenGL|ES 2.0.
 * A pretty complex shader is run per pixel on the screen
 * and might be slow on some devices with older gpu:s.
 */

void checkGLError(const char* where) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		lprintfln("%s: glGetError returned %x", where, err);
	}
}

///
// Create a shader object, load the shader source, and
// compile the shader.
//
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
			//printf("Error compiling shader:\n%s\n", infoLog);
			lprintfln("Error compiling shader:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

class MyGLMoblet: public GLMoblet {
private:
	GLuint mShader;
	GLuint mTimeLoc;
	GLuint mResolutionLoc;
	int mWidth, mHeight;

public:
	MyGLMoblet() :
		GLMoblet(GLMoblet::GL2) {
	}

	int initGL() {

		char vertexShaderSource[] =
		"attribute vec4 vPosition;   \n"
		"void main()                 \n"
		"{                           \n"
		"   gl_Position = vPosition; \n"
		"}                           \n";

		char fragmentShaderSource[] =
		"precision mediump float;\n"
		"uniform vec2 resolution;\n"
		"uniform float time;\n"
		"void main(void)\n"
		"{\n"
		"float x = gl_FragCoord.x*(resolution.x);\n"
		"float y = gl_FragCoord.y*(resolution.y);\n"
		"float c1 = sin((cos(x*6.0+sin(time+y)*7.0))*3.14159+cos(time))*0.5+0.5;\n"
		"gl_FragColor = vec4( c1*0.2,c1*0.5,c1*0.9,1.0);\n"
		"}\n";

		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint programObject;
		GLint linked;

		// Load the vertex/fragment shaders
		vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
		checkGLError("Load vertex shader");

		fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
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
		checkGLError("Bind vPosition to vertex shader");

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
		mShader = programObject;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		mTimeLoc = glGetUniformLocation(mShader, "time");
		mResolutionLoc = glGetUniformLocation(mShader, "resolution");

		return TRUE;
	}

	void init() {
		mWidth = EXTENT_X(maGetScrSize());
		mHeight = EXTENT_Y(maGetScrSize());

		if(!initGL())
			maPanic(1, "Failed to initialize!");
	}

	void draw() {
		GLfloat vVertices[] = { -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
				-1.0f, 0.0f, -1.0f, -1.0f, 0.0f };

		// Set the viewport
		glViewport(0, 0, mWidth, mHeight);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the program object
		glUseProgram(mShader);

		glUniform1f(mTimeLoc, maGetMilliSecondCount() * 0.001f);
		glUniform2f(mResolutionLoc, 1.0f/(float)mWidth, 1.0f/(float)mHeight);

		// Load the vertex data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain() {
	Moblet::run(new MyGLMoblet());
	return 0;
}

#ifndef _OPENGL_HELPERS_H_
#define _OPENGL_HELPERS_H_

#include <ma.h>
#include <IX_WIDGET.h>
#include <GLES/gl.h>
//#include "glutes.h"

void MoSync_InitFullscreenGL();
void MoSync_UpdateScreenGL();
void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
void resizeWindow(int x, int y, int width, int height);
void initGL(GLvoid);
void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
          GLfloat centerx, GLfloat centery, GLfloat centerz,
          GLfloat upx, GLfloat upy, GLfloat upz);
#endif

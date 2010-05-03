#ifndef _DGL_H_
#define _DGL_H_

#include "GLES/gl.h"

/* Definitions not in OpenGL ES but in normal OpenGL */

typedef double GLdouble; /* double precision float */
typedef double GLclampd; /* double precision float in [0,1] */

#define GL_INT					0x1404
#define GL_DOUBLE				0x140A

#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862

/* Desktop OpenGL functions */

#ifdef WIN32
# define CALLTYPE __stdcall
#else
# define CALLTYPE
#endif

struct DGL
{
	void (CALLTYPE *glClearColor)( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );

	void (CALLTYPE *glClear)( GLbitfield mask );

	void (CALLTYPE *glIndexMask)( GLuint mask );

	void (CALLTYPE *glColorMask)( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );

	void (CALLTYPE *glAlphaFunc)( GLenum func, GLclampf ref );

	void (CALLTYPE *glBlendFunc)( GLenum sfactor, GLenum dfactor );

	void (CALLTYPE *glLogicOp)( GLenum opcode );

	void (CALLTYPE *glCullFace)( GLenum mode );

	void (CALLTYPE *glFrontFace)( GLenum mode );

	void (CALLTYPE *glPointSize)( GLfloat size );

	void (CALLTYPE *glLineWidth)( GLfloat width );

	void (CALLTYPE *glPolygonOffset)( GLfloat factor, GLfloat units );

	void (CALLTYPE *glScissor)( GLint x, GLint y, GLsizei width, GLsizei height);

	void (CALLTYPE *glClipPlane)( GLenum plane, const GLdouble *equation );

	void (CALLTYPE *glGetClipPlane)( GLenum plane, GLdouble *equation );

	void (CALLTYPE *glEnable)( GLenum cap );

	void (CALLTYPE *glDisable)( GLenum cap );

	GLboolean (CALLTYPE *glIsEnabled)( GLenum cap );

	void (CALLTYPE *glEnableClientState)( GLenum cap );

	void (CALLTYPE *glDisableClientState)( GLenum cap );

	void (CALLTYPE *glGetBooleanv)( GLenum pname, GLboolean *params );

	void (CALLTYPE *glGetDoublev)( GLenum pname, GLdouble *params );

	void (CALLTYPE *glGetFloatv)( GLenum pname, GLfloat *params );

	void (CALLTYPE *glGetIntegerv)( GLenum pname, GLint *params );

	GLenum (CALLTYPE *glGetError)( void );

	const GLubyte* (CALLTYPE *glGetString)( GLenum name );

	void (CALLTYPE *glFinish)( void );

	void (CALLTYPE *glFlush)( void );

	void (CALLTYPE *glHint)( GLenum target, GLenum mode );

	void (CALLTYPE *glClearDepth)( GLclampd depth );

	void (CALLTYPE *glDepthFunc)( GLenum func );

	void (CALLTYPE *glDepthMask)( GLboolean flag );

	void (CALLTYPE *glDepthRange)( GLclampd near_val, GLclampd far_val );

	void (CALLTYPE *glMatrixMode)( GLenum mode );

	void (CALLTYPE *glOrtho)( GLdouble left, GLdouble right,
			GLdouble bottom, GLdouble top,
			GLdouble near_val, GLdouble far_val );

	void (CALLTYPE *glFrustum)( GLdouble left, GLdouble right,
			GLdouble bottom, GLdouble top,
			GLdouble near_val, GLdouble far_val );

	void (CALLTYPE *glViewport)( GLint x, GLint y,
			GLsizei width, GLsizei height );

	void (CALLTYPE *glPushMatrix)( void );

	void (CALLTYPE *glPopMatrix)( void );

	void (CALLTYPE *glLoadIdentity)( void );

	void (CALLTYPE *glLoadMatrixd)( const GLdouble *m );
	void (CALLTYPE *glLoadMatrixf)( const GLfloat *m );

	void (CALLTYPE *glMultMatrixd)( const GLdouble *m );
	void (CALLTYPE *glMultMatrixf)( const GLfloat *m );

	void (CALLTYPE *glRotated)( GLdouble angle,
			GLdouble x, GLdouble y, GLdouble z );
	void (CALLTYPE *glRotatef)( GLfloat angle,
			GLfloat x, GLfloat y, GLfloat z );

	void (CALLTYPE *glScaled)( GLdouble x, GLdouble y, GLdouble z );
	void (CALLTYPE *glScalef)( GLfloat x, GLfloat y, GLfloat z );

	void (CALLTYPE *glTranslated)( GLdouble x, GLdouble y, GLdouble z );

	void (CALLTYPE *glTranslatef)( GLfloat x, GLfloat y, GLfloat z );

	void (CALLTYPE *glBegin)( GLenum mode );

	void (CALLTYPE *glEnd)( void );

#if 0
	void glVertex2d( GLdouble x, GLdouble y );
	void glVertex2f( GLfloat x, GLfloat y );
	void glVertex2i( GLint x, GLint y );
#endif
	void (CALLTYPE *glVertex2s)( GLshort x, GLshort y );

#if 0
	void glVertex3d( GLdouble x, GLdouble y, GLdouble z );
	void glVertex3f( GLfloat x, GLfloat y, GLfloat z );
	void glVertex3i( GLint x, GLint y, GLint z );
#endif
	void (CALLTYPE *glVertex3s)( GLshort x, GLshort y, GLshort z );

#if 0
	void glVertex4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
	void glVertex4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
	void glVertex4i( GLint x, GLint y, GLint z, GLint w );
#endif
	void (CALLTYPE *glVertex4s)( GLshort x, GLshort y, GLshort z, GLshort w );

#if 0
	void glVertex2dv( const GLdouble *v );
	void glVertex2fv( const GLfloat *v );
	void glVertex2iv( const GLint *v );
	void glVertex2sv( const GLshort *v );

	void glVertex3dv( const GLdouble *v );
	void glVertex3fv( const GLfloat *v );
	void glVertex3iv( const GLint *v );
	void glVertex3sv( const GLshort *v );

	void glVertex4fv( const GLfloat *v );
	void glVertex4iv( const GLint *v );
	void glVertex4sv( const GLshort *v );


	void glNormal3b( GLbyte nx, GLbyte ny, GLbyte nz );
#endif
	void (CALLTYPE *glVertex4dv)( const GLdouble *v );

	void (CALLTYPE *glNormal3d)( GLdouble nx, GLdouble ny, GLdouble nz );
	void (CALLTYPE *glNormal3f)( GLfloat nx, GLfloat ny, GLfloat nz );
	void (CALLTYPE *glNormal3fv)( const GLfloat *v );

#if 0
	void glNormal3i( GLint nx, GLint ny, GLint nz );
	void glNormal3s( GLshort nx, GLshort ny, GLshort nz );

	void glNormal3bv( const GLbyte *v );
	void glNormal3dv( const GLdouble *v );
	void glNormal3iv( const GLint *v );
	void glNormal3sv( const GLshort *v );

	void glColor3b( GLbyte red, GLbyte green, GLbyte blue );
	void glColor3d( GLdouble red, GLdouble green, GLdouble blue );
	void glColor3f( GLfloat red, GLfloat green, GLfloat blue );
	void glColor3i( GLint red, GLint green, GLint blue );
	void glColor3s( GLshort red, GLshort green, GLshort blue );
	void glColor3ub( GLubyte red, GLubyte green, GLubyte blue );
	void glColor3ui( GLuint red, GLuint green, GLuint blue );
	void glColor3us( GLushort red, GLushort green, GLushort blue );

	void glColor4b( GLbyte red, GLbyte green,
			GLbyte blue, GLbyte alpha );
#endif
	void (CALLTYPE *glColor4fv)( const GLfloat *red );

	void (CALLTYPE *glColor4d)( GLdouble red, GLdouble green,
			GLdouble blue, GLdouble alpha );

	void (CALLTYPE *glColor4f)( GLfloat red, GLfloat green,
			GLfloat blue, GLfloat alpha );

#if 0
	void glColor4i( GLint red, GLint green,
			GLint blue, GLint alpha );
	void glColor4s( GLshort red, GLshort green,
			GLshort blue, GLshort alpha );
#endif

	void (CALLTYPE *glColor4ub)( GLubyte red, GLubyte green,
			GLubyte blue, GLubyte alpha );

#if 0
	void glColor4ui( GLuint red, GLuint green,
			GLuint blue, GLuint alpha );
	void glColor4us( GLushort red, GLushort green,
			GLushort blue, GLushort alpha );


	void glColor3bv( const GLbyte *v );
	void glColor3dv( const GLdouble *v );
	void glColor3fv( const GLfloat *v );
	void glColor3iv( const GLint *v );
	void glColor3sv( const GLshort *v );
	void glColor3ubv( const GLubyte *v );
	void glColor3uiv( const GLuint *v );
	void glColor3usv( const GLushort *v );

	void glColor4bv( const GLbyte *v );
	void glColor4dv( const GLdouble *v );
	void glColor4fv( const GLfloat *v );
	void glColor4iv( const GLint *v );
	void glColor4sv( const GLshort *v );
	void glColor4ubv( const GLubyte *v );
	void glColor4uiv( const GLuint *v );
	void glColor4usv( const GLushort *v );


	void glTexCoord1d( GLdouble s );
	void glTexCoord1f( GLfloat s );
	void glTexCoord1i( GLint s );
	void glTexCoord1s( GLshort s );

	void glTexCoord2d( GLdouble s, GLdouble t );
	void glTexCoord2f( GLfloat s, GLfloat t );
	void glTexCoord2i( GLint s, GLint t );
	void glTexCoord2s( GLshort s, GLshort t );

	void glTexCoord3d( GLdouble s, GLdouble t, GLdouble r );
	void glTexCoord3f( GLfloat s, GLfloat t, GLfloat r );
	void glTexCoord3i( GLint s, GLint t, GLint r );
	void glTexCoord3s( GLshort s, GLshort t, GLshort r );

	void glTexCoord4d( GLdouble s, GLdouble t, GLdouble r, GLdouble q );
	void glTexCoord4f( GLfloat s, GLfloat t, GLfloat r, GLfloat q );
	void glTexCoord4i( GLint s, GLint t, GLint r, GLint q );
	void glTexCoord4s( GLshort s, GLshort t, GLshort r, GLshort q );

	void glTexCoord1dv( const GLdouble *v );
	void glTexCoord1fv( const GLfloat *v );
	void glTexCoord1iv( const GLint *v );
	void glTexCoord1sv( const GLshort *v );

	void glTexCoord2dv( const GLdouble *v );
	void glTexCoord2fv( const GLfloat *v );
	void glTexCoord2iv( const GLint *v );
	void glTexCoord2sv( const GLshort *v );

	void glTexCoord3dv( const GLdouble *v );
	void glTexCoord3fv( const GLfloat *v );
	void glTexCoord3iv( const GLint *v );
	void glTexCoord3sv( const GLshort *v );

	void glTexCoord4dv( const GLdouble *v );
	void glTexCoord4fv( const GLfloat *v );
	void glTexCoord4iv( const GLint *v );
	void glTexCoord4sv( const GLshort *v );


	void glRasterPos2d( GLdouble x, GLdouble y );
	void glRasterPos2f( GLfloat x, GLfloat y );
	void glRasterPos2i( GLint x, GLint y );
	void glRasterPos2s( GLshort x, GLshort y );

	void glRasterPos3d( GLdouble x, GLdouble y, GLdouble z );
	void glRasterPos3f( GLfloat x, GLfloat y, GLfloat z );
	void glRasterPos3i( GLint x, GLint y, GLint z );
	void glRasterPos3s( GLshort x, GLshort y, GLshort z );

	void glRasterPos4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
	void glRasterPos4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
	void glRasterPos4i( GLint x, GLint y, GLint z, GLint w );
	void glRasterPos4s( GLshort x, GLshort y, GLshort z, GLshort w );

	void glRasterPos2dv( const GLdouble *v );
	void glRasterPos2fv( const GLfloat *v );
	void glRasterPos2iv( const GLint *v );
	void glRasterPos2sv( const GLshort *v );

	void glRasterPos3dv( const GLdouble *v );
	void glRasterPos3fv( const GLfloat *v );
	void glRasterPos3iv( const GLint *v );
	void glRasterPos3sv( const GLshort *v );

	void glRasterPos4dv( const GLdouble *v );
	void glRasterPos4fv( const GLfloat *v );
	void glRasterPos4iv( const GLint *v );
	void glRasterPos4sv( const GLshort *v );
#endif

	void (CALLTYPE *glVertexPointer)( GLint size, GLenum type,
			GLsizei stride, const GLvoid *ptr );

	void (CALLTYPE *glNormalPointer)( GLenum type, GLsizei stride,
			const GLvoid *ptr );

	void (CALLTYPE *glColorPointer)( GLint size, GLenum type,
			GLsizei stride, const GLvoid *ptr );

#if 0
	void glIndexPointer( GLenum type, GLsizei stride,
			const GLvoid *ptr );
#endif

	void (CALLTYPE *glTexCoordPointer)( GLint size, GLenum type,
			GLsizei stride, const GLvoid *ptr );

	void (CALLTYPE *glDrawElements)( GLenum mode, GLsizei count,
			GLenum type, const GLvoid *indices );

	void (CALLTYPE *glShadeModel)( GLenum mode );

	void (CALLTYPE *glLightf)( GLenum light, GLenum pname, GLfloat param );
	void (CALLTYPE *glLightfv)( GLenum light, GLenum pname, const GLfloat *params );

	void (CALLTYPE *glGetLightfv)( GLenum light, GLenum pname,
			GLfloat *params );

	void (CALLTYPE *glLightModelf)( GLenum pname, GLfloat param );
	void (CALLTYPE *glLightModelfv)( GLenum pname, const GLfloat *params );

	void (CALLTYPE *glMaterialf)( GLenum face, GLenum pname, GLfloat param );
	void (CALLTYPE *glMaterialfv)( GLenum face, GLenum pname, const GLfloat *params );

	void (CALLTYPE *glGetMaterialfv)( GLenum face, GLenum pname, GLfloat *params );

	void (CALLTYPE *glPixelStorei)( GLenum pname, GLint param );

	void (CALLTYPE *glReadPixels)( GLint x, GLint y,
			GLsizei width, GLsizei height,
			GLenum format, GLenum type,
			GLvoid *pixels );

#if 0
	void glDrawPixels( GLsizei width, GLsizei height,
			GLenum format, GLenum type,
			const GLvoid *pixels );
#endif

	void (CALLTYPE *glStencilFunc)( GLenum func, GLint ref, GLuint mask );

	void (CALLTYPE *glStencilMask)( GLuint mask );

	void (CALLTYPE *glStencilOp)( GLenum fail, GLenum zfail, GLenum zpass );

	void (CALLTYPE *glClearStencil)( GLint s );

	void (CALLTYPE *glTexEnvf)( GLenum target, GLenum pname, GLfloat param );
	void (CALLTYPE *glTexEnvi)( GLenum target, GLenum pname, GLint param );

	void (CALLTYPE *glTexEnvfv)( GLenum target, GLenum pname, const GLfloat *params );
	void (CALLTYPE *glTexEnviv)( GLenum target, GLenum pname, const GLint *params );

	void (CALLTYPE *glGetTexEnvfv)( GLenum target, GLenum pname, GLfloat *params );
	void (CALLTYPE *glGetTexEnviv)( GLenum target, GLenum pname, GLint *params );

	void (CALLTYPE *glTexParameterf)( GLenum target, GLenum pname, GLfloat param );
	void (CALLTYPE *glTexParameteri)( GLenum target, GLenum pname, GLint param );

	void (CALLTYPE *glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
	void (CALLTYPE *glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);

#if 0
	void glGetTexLevelParameterfv( GLenum target, GLint level,
			GLenum pname, GLfloat *params );
	void glGetTexLevelParameteriv( GLenum target, GLint level,
			GLenum pname, GLint *params );
#endif

	void (CALLTYPE *glTexImage2D)( GLenum target, GLint level,
			GLint internalFormat,
			GLsizei width, GLsizei height,
			GLint border, GLenum format, GLenum type,
			const GLvoid *pixels );

	void (CALLTYPE *glGenTextures)( GLsizei n, GLuint *textures );

	void (CALLTYPE *glDeleteTextures)( GLsizei n, const GLuint *textures);

	void (CALLTYPE *glBindTexture)( GLenum target, GLuint texture );

	GLboolean (CALLTYPE *glIsTexture)( GLuint texture );

	void (CALLTYPE *glTexSubImage2D)( GLenum target, GLint level,
			GLint xoffset, GLint yoffset,
			GLsizei width, GLsizei height,
			GLenum format, GLenum type,
			const GLvoid *pixels );

	void (CALLTYPE *glCopyTexImage2D)( GLenum target, GLint level,
			GLenum internalformat,
			GLint x, GLint y,
			GLsizei width, GLsizei height,
			GLint border );

	void (CALLTYPE *glCopyTexSubImage2D)( GLenum target, GLint level,
			GLint xoffset, GLint yoffset,
			GLint x, GLint y,
			GLsizei width, GLsizei height );

	void (CALLTYPE *glFogi)( GLenum pname, GLint param );
	void (CALLTYPE *glFogf)( GLenum pname, GLfloat param );
	void (CALLTYPE *glFogfv)( GLenum pname, const GLfloat *params );

	void (CALLTYPE *glActiveTexture)( GLenum texture );

	void (CALLTYPE *glClientActiveTexture)( GLenum texture );

	void (CALLTYPE *glMultiTexCoord4f)( GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
	void (CALLTYPE *glMultiTexCoord4d)( GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );

	void (CALLTYPE *glSampleCoverage)( GLclampf value, GLboolean invert );

	void (CALLTYPE *glPointParameterf)(GLenum, GLfloat);
	void (CALLTYPE *glPointParameterfv)(GLenum, const GLfloat *);
};

void *_dgles_platform_open();
void _dgles_platform_close(void *);
void *_dgles_platform_sym(void *, const char *);

#endif

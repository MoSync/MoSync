#ifndef __glext_h_
#define __glext_h_

/* $Revision: 13240 $ on $Date:: 2010-12-17 15:16:00 -0800 #$ */

#include "gl.h"

#ifdef MAPIP
#include <maapi_defs.h>
#include <IX_GL_OES_FRAMEBUFFER_OBJECT.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GL_OES_framebuffer_object                               1

/*------------------------------------------------------------------------*
 * OES extension tokens
 *------------------------------------------------------------------------*/
#if GL_OES_framebuffer_object
#define GL_FRAMEBUFFER_OES                                      0x8D40
#define GL_RENDERBUFFER_OES                                     0x8D41
#define GL_RGBA4_OES                                            0x8056
#define GL_RGB5_A1_OES                                          0x8057
#define GL_RGB565_OES                                           0x8D62
#define GL_DEPTH_COMPONENT16_OES                                0x81A5
#define GL_RENDERBUFFER_WIDTH_OES                               0x8D42
#define GL_RENDERBUFFER_HEIGHT_OES                              0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_OES                     0x8D44
#define GL_RENDERBUFFER_RED_SIZE_OES                            0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_OES                          0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_OES                           0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_OES                          0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_OES                          0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_OES                        0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES               0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES               0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES             0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES     0x8CD3
#define GL_COLOR_ATTACHMENT0_OES                                0x8CE0
#define GL_DEPTH_ATTACHMENT_OES                                 0x8D00
#define GL_STENCIL_ATTACHMENT_OES                               0x8D20
#define GL_FRAMEBUFFER_COMPLETE_OES                             0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES                0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES        0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES                0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES                   0x8CDA
#define GL_FRAMEBUFFER_UNSUPPORTED_OES                          0x8CDD
#define GL_FRAMEBUFFER_BINDING_OES                              0x8CA6
#define GL_RENDERBUFFER_BINDING_OES                             0x8CA7
#define GL_MAX_RENDERBUFFER_SIZE_OES                            0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION_OES                    0x0506
#endif

/*------------------------------------------------------------------------*
 * OES extension functions
 *------------------------------------------------------------------------*/
#ifndef MAPIP

#if GL_OES_framebuffer_object

#undef GL_APIENTRY
#define GL_APIENTRY __attribute((gccxml("group", "GL_OES_framebuffer_object")))

GL_API GLboolean GL_APIENTRY glIsRenderbufferOES (GLuint renderbuffer);
GL_API GLvoid GL_APIENTRY glBindRenderbufferOES (GLenum target, GLuint renderbuffer);
GL_API GLvoid GL_APIENTRY glDeleteRenderbuffersOES (GLsizei n, _use_type_(true, false) const GLuint* renderbuffers);
GL_API GLvoid GL_APIENTRY glGenRenderbuffersOES (GLsizei n, GLuint* renderbuffers);
GL_API GLvoid GL_APIENTRY glRenderbufferStorageOES (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GL_API GLvoid GL_APIENTRY glGetRenderbufferParameterivOES (GLenum target, GLenum pname, GLint* params);
GL_API GLboolean GL_APIENTRY glIsFramebufferOES (GLuint framebuffer);
GL_API GLvoid GL_APIENTRY glBindFramebufferOES (GLenum target, GLuint framebuffer);
GL_API GLvoid GL_APIENTRY glDeleteFramebuffersOES (GLsizei n, _use_type_(true, false) const GLuint* framebuffers);
GL_API GLvoid GL_APIENTRY glGenFramebuffersOES (GLsizei n, GLuint* framebuffers);
GL_API GLenum GL_APIENTRY glCheckFramebufferStatusOES (GLenum target);
GL_API GLvoid GL_APIENTRY glFramebufferRenderbufferOES (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GL_API GLvoid GL_APIENTRY glFramebufferTexture2DOES (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GL_API GLvoid GL_APIENTRY glGetFramebufferAttachmentParameterivOES (GLenum target, GLenum attachment, GLenum pname, GLint* params);
GL_API GLvoid GL_APIENTRY glGenerateMipmapOES (GLenum target);

#endif

#endif // ifndef MAPIP

#ifdef __cplusplus
}
#endif

#endif /* __glext_h_ */


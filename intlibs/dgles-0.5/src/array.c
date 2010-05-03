#include "api.h"
#include <stdio.h>
#include <stdlib.h>

static int get_type_size(GLenum type)
{
	switch (type)
	{
	default:
		assert(!"bad");
		return 0;
	case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
	case GL_BYTE: return sizeof(GLbyte);
	case GL_UNSIGNED_SHORT: return sizeof(GLushort);
	case GL_SHORT: return sizeof(GLshort);
	case GL_FIXED: return sizeof(GLfixed);
	case GL_FLOAT: return sizeof(GLfloat);
	}
}

void GL_APIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ENTER();

	if (type != GL_UNSIGNED_BYTE && type != GL_FIXED && type != GL_FLOAT)
		ERROR(GL_INVALID_ENUM);

	if (size != 4)
		ERROR(GL_INVALID_VALUE);

	if (stride < 0)
		ERROR(GL_INVALID_VALUE);

	state.cptr.size = size;
	state.cptr.type = type;
	state.cptr.stride = stride;
	state.cptr.pointer = pointer;

	if (stride == 0)
		state.cptr.rstride = size * get_type_size(type);
	else
		state.cptr.rstride = stride;

#if 0
	if (type != GL_FIXED)
		dgl.glColorPointer(size, type, stride, pointer);
#endif

	state.cptr.buffer = state.array_buffer;

	LEAVE();
}

void GL_APIENTRY glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ENTER();

	if (type != GL_BYTE && type != GL_SHORT && type != GL_FIXED &&
	    type != GL_FLOAT)
		ERROR(GL_INVALID_ENUM);

	if (stride < 0)
		ERROR(GL_INVALID_VALUE);

	state.nptr.size = 3;
	state.nptr.type = type;
	state.nptr.stride = stride;
	state.nptr.pointer = pointer;

	if (stride == 0)
		state.nptr.rstride = 3 * get_type_size(type);
	else
		state.nptr.rstride = stride;

#if 0
	if (type != GL_FIXED)
		dgl.glNormalPointer(type, stride, pointer);
#endif

	state.nptr.buffer = state.array_buffer;

	LEAVE();
}

void GL_APIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ENTER();

	if (size != 2 && size != 3 && size != 4)
		ERROR(GL_INVALID_VALUE);

	if (type != GL_BYTE && type != GL_SHORT && type != GL_FIXED &&
	    type != GL_FLOAT)
		ERROR(GL_INVALID_VALUE);

	if (stride < 0)
		ERROR(GL_INVALID_VALUE);

	state.vptr.size = size;
	state.vptr.type = type;
	state.vptr.stride = stride;
	state.vptr.pointer = pointer;

	if (stride == 0)
		state.vptr.rstride = size * get_type_size(type);
	else
		state.vptr.rstride = stride;

#if 0
	/* GL_BYTE must be converted to GL_SHORT when drawing */
	if (type != GL_BYTE && type != GL_FIXED)
		dgl.glVertexPointer(size, type, stride, pointer);
#endif

	state.vptr.buffer = state.array_buffer;

	LEAVE();
}

void GL_APIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ENTER();

	if (size != 2 && size != 3 && size != 4)
		ERROR(GL_INVALID_VALUE);

	if (type != GL_BYTE && type != GL_SHORT && type != GL_FIXED &&
	    type != GL_FLOAT)
		ERROR(GL_INVALID_ENUM);

	if (stride < 0)
		ERROR(GL_INVALID_VALUE);

	state.tptr[state.active_texture].size = size;
	state.tptr[state.active_texture].type = type;
	state.tptr[state.active_texture].stride = stride;
	state.tptr[state.active_texture].pointer = pointer;

	if (stride == 0)
		state.tptr[state.active_texture].rstride = size * get_type_size(type);
	else
		state.tptr[state.active_texture].rstride = stride;

#if 0
	/* GL_BYTE must be converted to GL_SHORT when drawing */
	if (type != GL_BYTE && type != GL_FIXED)
		dgl.glTexCoordPointer(size, type, stride, pointer);
#endif

	state.tptr[state.active_texture].buffer = state.array_buffer;

	LEAVE();
}

/*
 * OES_point_size_array
 */

void GL_APIENTRY glPointSizePointerOES (GLenum type, GLsizei stride,
                                        const GLvoid *pointer)
{
	ENTER();

	if (type != GL_FIXED && type != GL_FLOAT)
		ERROR(GL_INVALID_ENUM);

	if (stride < 0)
		ERROR(GL_INVALID_VALUE);

	state.psptr.size = 1;
	state.psptr.type = type;
	state.psptr.stride = stride;
	state.psptr.pointer = pointer;

	if (stride == 0)
		state.psptr.rstride =
			(type == GL_FIXED) ? sizeof(GLfixed) : sizeof(GLfloat);
	else
		state.psptr.rstride = stride;

	state.psptr.buffer = state.array_buffer;

	LEAVE();
}

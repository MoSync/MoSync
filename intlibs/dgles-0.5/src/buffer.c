#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* TODO: linked list is not very efficient way to store buffers, fix that */

static struct DGLES_Buffer *new_buffer()
{
	struct DGLES_Buffer *buf;

	buf = malloc(sizeof(struct DGLES_Buffer));
	if (buf == NULL)
		return NULL;

	buf->name = 0;
	buf->size = 0;
	buf->data = NULL;
	buf->usage = GL_STATIC_DRAW;
	buf->next = NULL;

	return buf;
}

static void delete_buffer(struct DGLES_Buffer *buf)
{
	unsigned int i;

	if (state.vptr.buffer == buf)
		state.vptr.buffer = NULL;

	if (state.nptr.buffer == buf)
		state.nptr.buffer = NULL;

	if (state.cptr.buffer == buf)
		state.cptr.buffer = NULL;

	for (i = 0; i < state.max_texture_units; i++)
		if (state.tptr[i].buffer == buf)
			state.tptr[i].buffer = NULL;

	if (state.array_buffer == buf)
		state.array_buffer = NULL;

	if (state.element_buffer == buf)
		state.element_buffer = NULL;

	free(buf->data);
	free(buf);
}

struct DGLES_Buffer *find_buffer(GLuint name)
{
	struct DGLES_Buffer *buf;
	
	for (buf = state.first_buffer; buf != NULL; buf = buf->next)
		if (buf->name == name)
			return buf;

	return NULL;
}

void GL_APIENTRY glGenBuffers (GLsizei n, GLuint *buffers)
{
	int i;
	struct DGLES_Buffer **bufs;
	struct DGLES_Buffer *buf;
	GLuint name;

	ENTER();

	if (n < 0)
		ERROR(GL_INVALID_VALUE);

	/* Allocate memory */

	bufs = malloc(sizeof(struct DGLES_Buffer *) * n);
	if (bufs == NULL)
		ERROR(GL_OUT_OF_MEMORY);

	for (i = 0; i < n; i++)
	{
		bufs[i] = new_buffer();
		if (bufs[i] == NULL)
		{
			int j;
			for (j = 0; j < i; j++)
				delete_buffer(bufs[j]);
			free(bufs);

			ERROR(GL_OUT_OF_MEMORY);
		}
	}

	/* Find next name */
	/* TODO: handle name overflow */

	name = 1;
	for (buf = state.first_buffer; buf; buf = buf->next)
	{
		if (buf->name >= name)
			name = buf->name + 1;
	}

	/* Link and assign names */

	for (i = 0; i < n; i++)
	{
		buffers[i] = name + i;
		bufs[i]->name = name + i;

		if (state.first_buffer)
		{
			bufs[i]->next = state.first_buffer;
			state.first_buffer = bufs[i];
		}
		else
			state.first_buffer = bufs[i];
	}

	/* Clean up */

	free(bufs);

	LEAVE();
}

void GL_APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers)
{
	int i;

	ENTER();

	if (n < 0)
		ERROR(GL_INVALID_VALUE);

	for (i = 0; i < n && state.first_buffer; i++)
	{
		if (buffers[i])
		{
			struct DGLES_Buffer *buf;

			if (state.first_buffer->name == buffers[i])
			{
				buf = state.first_buffer->next;
				delete_buffer(state.first_buffer);
				state.first_buffer = buf;
			}
			else
			{
				for (buf = state.first_buffer; buf->next; buf = buf->next)
				{
					if (buf->next->name == buffers[i])
					{
						struct DGLES_Buffer *b = buf->next->next;
						delete_buffer(buf->next);
						buf->next = b;
						break;
					}
				}
			}
		}
	}

	LEAVE();
}

void GL_APIENTRY glBindBuffer (GLenum target, GLuint buffer)
{
	struct DGLES_Buffer *buf;

	ENTER();

	if (buffer == 0)
		buf = NULL;
	else
	{
		buf = find_buffer(buffer);

		if (buf == NULL)
		{
			buf = new_buffer();
			buf->name = buffer;
			buf->next = state.first_buffer;
			state.first_buffer = buf;
		}
	}

	switch (target)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_ARRAY_BUFFER:
		state.array_buffer = buf;
		break;
	case GL_ELEMENT_ARRAY_BUFFER:
		state.element_buffer = buf;
		break;
	}

	LEAVE();
}

void GL_APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
	GLvoid *d;
	struct DGLES_Buffer *buf;

	ENTER();

	switch (target)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_ARRAY_BUFFER:
		buf = state.array_buffer;
		break;
	case GL_ELEMENT_ARRAY_BUFFER:
		buf = state.element_buffer;
		break;
	}

	switch (usage)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_DYNAMIC_DRAW:
	case GL_STATIC_DRAW:
		break;
	}

	if (buf == NULL)
		ERROR(GL_INVALID_OPERATION);

	d = malloc(size);
	if (d == NULL)
		ERROR(GL_OUT_OF_MEMORY);

	free(buf->data);

	if (data)
		memcpy(d, data, size);
	else
		/* Fill with some weird byte to avoid people from assuming it to
		 * be zero */
		memset(d, 0xAB, size);

	buf->size = size;
	buf->data = d;
	buf->usage = usage;

	LEAVE();
}

void GL_APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
	struct DGLES_Buffer *buf;

	ENTER();

	switch (target)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_ARRAY_BUFFER:
		buf = state.array_buffer;
		break;
	case GL_ELEMENT_ARRAY_BUFFER:
		buf = state.element_buffer;
		break;
	}

	if (buf == NULL)
		ERROR(GL_INVALID_OPERATION);

	if (offset < 0 || size < 0 || offset + size > buf->size)
		ERROR(GL_INVALID_VALUE);

	memcpy((char *) buf->data + offset, data, size);

	LEAVE();
}

void GL_APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params)
{
	struct DGLES_Buffer *buf;

	ENTER();

	switch (target)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_ARRAY_BUFFER:
		buf = state.array_buffer;
		break;
	case GL_ELEMENT_ARRAY_BUFFER:
		buf = state.element_buffer;
		break;
	}

	if (buf == NULL)
		ERROR(GL_INVALID_OPERATION);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_BUFFER_SIZE:
		params[0] = buf->size;
		break;
	case GL_BUFFER_USAGE:
		params[0] = buf->usage;
		break;
	case GL_BUFFER_ACCESS:
		params[0] = GL_WRITE_ONLY;
		break;
	}

	LEAVE();
}

GLboolean GL_APIENTRY glIsBuffer (GLuint buffer)
{
	GLboolean ret;
	ENTER();
	ret = find_buffer(buffer) != NULL;
	LEAVE2(ret);
}

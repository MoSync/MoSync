#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Drawing
 */

static int is_valid_primitive(GLenum mode)
{
	switch (mode)
	{
	default:
		return 0;
	case GL_POINTS:
	case GL_LINES:
	case GL_LINE_STRIP:
	case GL_LINE_LOOP:
	case GL_TRIANGLES:
	case GL_TRIANGLE_STRIP:
	case GL_TRIANGLE_FAN:
		return 1;
	}
}

static void convert_byte_to_short_array(const struct DGLES_Array *ary,
                                        int n, void **dest)
{
	int i, j;

	assert(ary->type == GL_BYTE);

	/* Allocate memory */

	*dest = malloc(n * ary->size * sizeof(GLshort));
	if (*dest == NULL)
		return;

	/* Convert */

	if (ary->buffer)
	{
		/* TODO: handle buffer overrun */
		for (i = 0; i < n; i++)
			for (j = 0; j < ary->size; j++)
				((GLshort *) *dest)[i * ary->size + j] =
					((const GLbyte *)(ary->buffer->data) + (i)*ary->rstride)[j];
	}
	else
	{
		for (i = 0; i < n; i++)
			for (j = 0; j < ary->size; j++)
				((GLshort *) *dest)[i * ary->size + j] = ARRAY_PTR(ary, i)[j];
	}
}

static void convert_fixed_to_float_array(const struct DGLES_Array *ary,
                                         int n, void **dest)
{
	int i, j;

	assert(ary->type == GL_FIXED);

	/* Allocate memory */

	*dest = malloc(n * ary->size * sizeof(GLfloat));
	if (*dest == NULL)
		return;

	/* Convert */

	if (ary->buffer)
	{
		/* TODO: handle buffer overrun */
		for (i = 0; i < n; i++)
			for (j = 0; j < ary->size; j++)
			{
				GLfixed f =
					((const GLfixed *) (ary->buffer->data) +
					 (i)*ary->rstride)[j];

				((GLfloat *) *dest)[i * ary->size + j] = X_TO_F(f);
			}
	}
	else
	{
		for (i = 0; i < n; i++)
			for (j = 0; j < ary->size; j++)
				((GLfloat *) *dest)[i * ary->size + j] =
					X_TO_F(((const GLfixed *) ARRAY_PTR(ary, i))[j]);
	}
}

static void draw_points(GLsizei count, GLenum type, const GLvoid *indices)
{
	int i;
	double vv[4];
	float orig_ps;

	dgl.glGetFloatv(GL_POINT_SIZE, &orig_ps);

	/* Defaults */
	vv[2] = 0.0;
	vv[3] = 1.0;

	for (i = 0; i < count; i++)
	{
		int index = type == GL_UNSIGNED_BYTE ?
			((const GLubyte *) indices)[i] :
			((const GLushort *) indices)[i];
		int j;
		const char *ptr;

		/* Point size */

		if ((state.array_flags & POINT_SIZE_ARRAY) && state.psptr.pointer)
		{
			ptr = (const char *) state.psptr.pointer +
				index * state.psptr.rstride;

			dgl.glPointSize(
					(state.psptr.type == GL_FIXED) ?
						X_TO_F(* (const GLfixed *) ptr) :
						* (const GLfloat *) ptr);
		}

		/* Color */

		if ((state.array_flags & COLOR_ARRAY) && state.cptr.pointer)
		{
			float cv[4];
			ptr = ARRAY_PTR(&state.cptr, index);

			switch (state.cptr.type)
			{
			case GL_UNSIGNED_BYTE:
				for (j = 0; j < 4; j++)
					cv[j] = ((const GLubyte *) ptr)[j] / 255.0f;
				break;
			case GL_FIXED:
				for (j = 0; j < 4; j++)
					cv[j] = X_TO_F(((const GLfixed *) ptr)[j]);
				break;
			case GL_FLOAT:
				for (j = 0; j < 4; j++)
					cv[j] = ((const GLfloat *) ptr)[j];
				break;
			}

			dgl.glColor4fv(cv);
		}

		/* Normal */

		if ((state.array_flags & NORMAL_ARRAY) && state.nptr.pointer)
		{
			float nv[3];
			ptr = ARRAY_PTR(&state.nptr, index);

			switch (state.nptr.type)
			{
			case GL_BYTE:
				for (j = 0; j < 3; j++)
					nv[j] = (((const GLbyte *) ptr)[j] + 128) / 255.0f * 2 - 1;
				break;
			case GL_SHORT:
				for (j = 0; j < 3; j++)
					nv[j] = (((const GLbyte *) ptr)[j] + 32768) / 65535.0f * 2 - 1;
				break;
			case GL_FIXED:
				for (j = 0; j < 3; j++)
					nv[j] = X_TO_F(((const GLfixed *) ptr)[j]);
				break;
			case GL_FLOAT:
				for (j = 0; j < 3; j++)
					nv[j] = ((const GLfloat *) ptr)[j];
				break;
			}

			dgl.glNormal3fv(nv);
		}

		/* Texture coords */

		/* TODO: !! */
		assert(state.max_texture_units <= 2);
		assert((state.array_flags & TEXCOORD0_ARRAY) == 0);
		assert((state.array_flags & TEXCOORD1_ARRAY) == 0);

		/* Vertex */

		for (j = 0; j < state.vptr.size; j++)
		{
			switch (state.vptr.type)
			{
			case GL_BYTE:
				vv[j] = ((const GLbyte *) ARRAY_PTR(&state.vptr, index))[j];
				break;
			case GL_SHORT:
				vv[j] = ((const GLshort *) ARRAY_PTR(&state.vptr, index))[j];
				break;
			case GL_FIXED:
				vv[j] = X_TO_D(((const GLfixed *)
							ARRAY_PTR(&state.vptr, index))[j]);
				break;
			case GL_FLOAT:
				vv[j] = ((const GLfloat *) ARRAY_PTR(&state.vptr, index))[j];
				break;
			}
		}

		dgl.glBegin(GL_POINTS);
		dgl.glVertex4dv(vv);
		dgl.glEnd();
	}

	dgl.glPointSize(orig_ps);
}

static GLenum draw_elements(GLenum mode, GLsizei count, GLenum type,
                            const GLvoid *indices)
{
	void *vconv = NULL;
	void *cconv = NULL;
	void *nconv = NULL;
	void *tconv[MAX_TEXTURE_UNITS];
	int i;
	int max = 0;
	GLenum err = GL_NO_ERROR;

	ENTER();

	/* Check that mode is valid */

	if (!is_valid_primitive(mode))
		return GL_INVALID_ENUM;

	/* Check that count isn't negative */

	if (count < 0)
		return GL_INVALID_VALUE;

	/* Check that type is valid */

	if (type != GL_UNSIGNED_BYTE && type != GL_UNSIGNED_SHORT)
		return GL_INVALID_ENUM;

	/* Without vertex array or any elements, rendering is useless */

	if (!(state.array_flags & VERTEX_ARRAY) || count == 0)
		return GL_NO_ERROR;

	/* Draw points independently if point size array is used */

	if (mode == GL_POINTS)
	{
		if (state.array_flags & POINT_SIZE_ARRAY)
		{
			draw_points(count, type, indices);
			return GL_NO_ERROR;
		}
	}

	/* OpenGL 1.5 doesn't support GL_BYTE nor GL_FIXED, so those are converted
	 * to GL_SHORT and GL_FLOAT */

	{
#if 0
		int need_conv = 0;

		if (state.vptr.type || state.vptr.type == GL_FIXED)
			need_conv = 1;
		if ((state.array_flags & COLOR_ARRAY) && state.cptr.type == GL_FIXED)
			need_conv = 1;
		if ((state.array_flags & NORMAL_ARRAY) && state.nptr.type == GL_FIXED)
			need_conv = 1;
		for (i = 0; i < (int) state.max_texture_units; i++)
			if (state.array_flags & (TEXCOORD0_ARRAY << i))
				if (state.tptr[0].type == GL_BYTE ||
				    state.tptr[0].type == GL_FIXED)
					need_conv = 1;
#endif

		/* Get number of elements to convert */

		for (i = 0; i < count; i++)
		{
			int index = (type == GL_UNSIGNED_BYTE) ?
			                ((GLubyte *) indices)[i] :
			                ((GLushort *) indices)[i];
			if (index > max)
				max = index;
		}

		max++;
	}

	/* Vertex array */

	if (state.vptr.type == GL_BYTE)
	{
		convert_byte_to_short_array(&state.vptr, max, &vconv);
		if (vconv == NULL)
		{
			err = GL_OUT_OF_MEMORY;
			goto clean_up;
		}

		dgl.glVertexPointer(state.vptr.size, GL_SHORT, 0, vconv);
	}
	else if (state.vptr.type == GL_FIXED)
	{
		convert_fixed_to_float_array(&state.vptr, max, &vconv);
		if (vconv == NULL)
		{
			err = GL_OUT_OF_MEMORY;
			goto clean_up;
		}

		dgl.glVertexPointer(state.vptr.size, GL_FLOAT, 0, vconv);
	}
	else
		dgl.glVertexPointer(state.vptr.size, state.vptr.type,
				state.vptr.stride, ARRAY_BUFFER(&state.vptr));

	/* Color array */

	if (state.array_flags & COLOR_ARRAY)
	{
		if (state.cptr.type == GL_FIXED)
		{
			convert_fixed_to_float_array(&state.cptr, max, &cconv);
			if (cconv == NULL)
			{
				err = GL_OUT_OF_MEMORY;
				goto clean_up;
			}

			dgl.glColorPointer(state.cptr.size, GL_FLOAT, 0, cconv);
		}
		else
		{
			dgl.glColorPointer(state.cptr.size, state.cptr.type,
					state.cptr.stride, ARRAY_BUFFER(&state.cptr));
		}
	}

	/* Normal array */

	if (state.array_flags & NORMAL_ARRAY)
	{
		if (state.nptr.type == GL_FIXED)
		{
			convert_fixed_to_float_array(&state.nptr, max, &nconv);
			if (nconv == NULL)
			{
				err = GL_OUT_OF_MEMORY;
				goto clean_up;
			}

			dgl.glNormalPointer(GL_FLOAT, 0, nconv);
		}
		else
		{
			dgl.glNormalPointer(state.nptr.type,
					state.nptr.stride, ARRAY_BUFFER(&state.nptr));
		}
	}

	/* Texcoord arrays */

	memset(&tconv, 0, sizeof(tconv));

	for (i = 0; i < (int) state.max_texture_units; i++)
	{
		if (state.array_flags & (TEXCOORD0_ARRAY << i))
		{
			dgl.glClientActiveTexture(GL_TEXTURE0 + i);

			if (state.tptr[i].type == GL_BYTE)
			{
				convert_byte_to_short_array(&state.tptr[i], max, &tconv[i]);
				if (tconv[i] == NULL)
				{
					err = GL_OUT_OF_MEMORY;
					goto clean_up;
				}

				dgl.glTexCoordPointer(state.tptr[i].size, GL_SHORT, 0,
				                      tconv[i]);
			}
			else if (state.tptr[i].type == GL_FIXED)
			{
				convert_fixed_to_float_array(&state.tptr[i], max, &tconv[i]);
				if (tconv[i] == NULL)
				{
					err = GL_OUT_OF_MEMORY;
					goto clean_up;
				}

				dgl.glTexCoordPointer(state.tptr[i].size, GL_FLOAT, 0,
				                      tconv[i]);
			}
			else
			{
				dgl.glTexCoordPointer(state.tptr[i].size, state.tptr[i].type,
						state.tptr[i].stride, ARRAY_BUFFER(&state.tptr[i]));
			}
		}
	}

	/* Set original texture active */
	dgl.glClientActiveTexture(GL_TEXTURE0 + state.active_texture);

	/* Draw */

	dgl.glDrawElements(mode, count, type, indices);

	/* Clean up conversions */

clean_up:

	if (vconv)
		free(vconv);
	if (cconv)
		free(cconv);
	if (nconv)
		free(nconv);

	{
		unsigned int u;
		for (u = 0; u < state.max_texture_units; u++)
			if (tconv[u])
				free(tconv[u]);
	}

	return err;
}

void GL_APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
	GLushort *indices;
	int i;
	GLenum err;

	ENTER();
	assert(dgl.glGetError() == GL_NO_ERROR);

	if (count < 0)
		ERROR(GL_INVALID_VALUE);

	if ((first + count) > 1 << (sizeof(GLushort) * 8))
		ERROR(GL_INVALID_VALUE);

	/* Change DrawArrays to DrawElements */
	/* TODO: this isn't most efficient, don't do it */

	indices = malloc(sizeof(GLshort) * count);
	if (indices == NULL)
		ERROR(GL_OUT_OF_MEMORY);

	for (i = 0; i < count; i++)
		indices[i] = first + i;

	err = draw_elements(mode, count, GL_UNSIGNED_SHORT, indices);

	free(indices);

	if (err != GL_NO_ERROR)
		ERROR(err);

	LEAVE();
}

void GL_APIENTRY glDrawElements (GLenum mode, GLsizei count,
                                 GLenum type, const GLvoid *indices)
{
	GLenum err;

	ENTER();
	assert(dgl.glGetError() == GL_NO_ERROR);

	if (state.element_buffer)
		err = draw_elements(mode, count, type, state.element_buffer->data);
	else
		err = draw_elements(mode, count, type, indices);

	if (err != GL_NO_ERROR)
		ERROR(err);

	LEAVE();
}

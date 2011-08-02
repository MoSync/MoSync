#include "api.h"
#include <limits.h>
#include <stdio.h>

/*
 * Get functions
 */

union GetResult
{
	GLboolean b;
	GLint     i;
	GLfixed   fix;
	GLfloat   f;
	GLdouble  d;
};

enum GetType
{
	GET_BOOLEAN,
	GET_INT,
	GET_FIXED,
	GET_FLOAT,
	GET_DOUBLE
};

static int do_get(GLenum pname, union GetResult *res, enum GetType *type,
                  int *n)
{
	GLboolean b[16];
	GLint     i[16];
	//GLfloat   f[16];
	GLdouble  d[16];
	int j;

	assert(res && type && n);

	switch (pname)
	{
	default:
		*n = 0;
		return GL_INVALID_ENUM;
	case GL_ALIASED_POINT_SIZE_RANGE:
	case GL_ALIASED_LINE_WIDTH_RANGE:
	case GL_DEPTH_RANGE:
	case GL_SMOOTH_LINE_WIDTH_RANGE:
	case GL_SMOOTH_POINT_SIZE_RANGE:
		dgl.glGetDoublev(pname, d);
		*type = GET_DOUBLE;
		*n = 2;
		break;
	case GL_ALPHA_TEST_FUNC:
	case GL_ALPHA_BITS:
	case GL_RED_BITS:
	case GL_GREEN_BITS:
	case GL_BLUE_BITS:
	case GL_DEPTH_BITS:
	case GL_STENCIL_BITS:
	case GL_STENCIL_FAIL:
	case GL_STENCIL_FUNC:
	case GL_STENCIL_PASS_DEPTH_FAIL:
	case GL_STENCIL_PASS_DEPTH_PASS:
	case GL_STENCIL_VALUE_MASK:
	case GL_STENCIL_WRITEMASK:
	case GL_BLEND_DST:
	case GL_BLEND_SRC:
	case GL_CULL_FACE:
	case GL_DEPTH_FUNC:
	case GL_FOG_HINT:
	case GL_FOG_MODE:
	case GL_FRONT_FACE:
	case GL_LINE_SMOOTH_HINT:
	case GL_LOGIC_OP_MODE:
	case GL_MATRIX_MODE:
	case GL_MODELVIEW_STACK_DEPTH:
	case GL_PROJECTION_STACK_DEPTH:
	case GL_TEXTURE_STACK_DEPTH:
	case GL_PACK_ALIGNMENT:
	case GL_UNPACK_ALIGNMENT:
	case GL_PERSPECTIVE_CORRECTION_HINT:
	case GL_POINT_SMOOTH_HINT:
	case GL_SHADE_MODEL:
	case GL_TEXTURE_BINDING_2D:
	case GL_SUBPIXEL_BITS:
	case GL_SAMPLE_BUFFERS:
	case GL_SAMPLES:
	case GL_BUFFER_SIZE:
	case GL_CLIENT_ACTIVE_TEXTURE:
	case GL_GENERATE_MIPMAP_HINT:
	case GL_CULL_FACE_MODE:
	case GL_ACTIVE_TEXTURE:
	case GL_STENCIL_CLEAR_VALUE:
		dgl.glGetIntegerv(pname, i);
		*type = GET_INT;
		*n = 1;
		break;
	case GL_ALPHA_TEST_REF:
	case GL_STENCIL_REF:
	case GL_DEPTH_CLEAR_VALUE:
	case GL_FOG_DENSITY:
	case GL_FOG_START:
	case GL_FOG_END:
	case GL_LINE_WIDTH:
	case GL_POINT_SIZE:
	case GL_POINT_SIZE_MIN:
	case GL_POINT_SIZE_MAX:
	case GL_POLYGON_OFFSET_FACTOR:
	case GL_POLYGON_OFFSET_UNITS:
	case GL_POINT_FADE_THRESHOLD_SIZE:
		dgl.glGetDoublev(pname, d);
		*type = GET_DOUBLE;
		*n = 1;
		break;
	case GL_CURRENT_NORMAL:
	case GL_POINT_DISTANCE_ATTENUATION:
		dgl.glGetDoublev(pname, d);
		*type = GET_DOUBLE;
		*n = 3;
		break;
	case GL_COLOR_CLEAR_VALUE:
	case GL_CURRENT_TEXTURE_COORDS:
	case GL_CURRENT_COLOR:
	case GL_FOG_COLOR:
	case GL_LIGHT_MODEL_AMBIENT:
#if 0
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_EMISSION:
#endif
		dgl.glGetDoublev(pname, d);
		*type = GET_DOUBLE;
		*n = 4;
		break;
	case GL_COLOR_WRITEMASK:
		dgl.glGetBooleanv(pname, b);
		*type = GET_BOOLEAN;
		*n = 4;
		break;
	case GL_DEPTH_WRITEMASK:
	case GL_LIGHT_MODEL_TWO_SIDE:
		dgl.glGetBooleanv(pname, b);
		*type = GET_BOOLEAN;
		*n = 1;
		break;
	case GL_PROJECTION_MATRIX:
	case GL_MODELVIEW_MATRIX:
	case GL_TEXTURE_MATRIX:
		dgl.glGetDoublev(pname, d);
		*type = GET_DOUBLE;
		*n = 16;
		break;
	case GL_VIEWPORT:
	case GL_SCISSOR_BOX:
		dgl.glGetIntegerv(pname, i);
		*type = GET_INT;
		*n = 4;
		break;
	case GL_MAX_VIEWPORT_DIMS:
		dgl.glGetIntegerv(pname, i);
		*type = GET_INT;
		*n = 2;
		break;

	case GL_COLOR_ARRAY_SIZE:
		i[0] = state.cptr.size;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_COLOR_ARRAY_STRIDE:
		i[0] = state.cptr.stride;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_COLOR_ARRAY_TYPE:
		i[0] = state.cptr.type;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_NORMAL_ARRAY_STRIDE:
		i[0] = state.nptr.stride;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_NORMAL_ARRAY_TYPE:
		i[0] = state.nptr.type;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_TEXTURE_COORD_ARRAY_SIZE:
		i[0] = state.tptr[state.active_texture].size;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_TEXTURE_COORD_ARRAY_STRIDE:
		i[0] = state.tptr[state.active_texture].stride;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_TEXTURE_COORD_ARRAY_TYPE:
		i[0] = state.tptr[state.active_texture].type;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_VERTEX_ARRAY_SIZE:
		i[0] = state.vptr.size;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_VERTEX_ARRAY_STRIDE:
		i[0] = state.vptr.stride;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_VERTEX_ARRAY_TYPE:
		i[0] = state.vptr.type;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_POINT_SIZE_ARRAY_STRIDE_OES:
		i[0] = state.psptr.stride;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_POINT_SIZE_ARRAY_TYPE_OES:
		i[0] = state.psptr.type;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_COLOR_ARRAY_BUFFER_BINDING:
		i[0] = state.cptr.buffer ? state.cptr.buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_NORMAL_ARRAY_BUFFER_BINDING:
		i[0] = state.nptr.buffer ? state.nptr.buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
		i[0] = state.tptr[state.active_texture].buffer ?
			   state.tptr[state.active_texture].buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_VERTEX_ARRAY_BUFFER_BINDING:
		i[0] = state.vptr.buffer ? state.vptr.buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
		i[0] = state.psptr.buffer ? state.psptr.buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_ARRAY_BUFFER_BINDING:
		i[0] = state.array_buffer ? state.array_buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;
	case GL_ELEMENT_ARRAY_BUFFER_BINDING:
		i[0] = state.element_buffer ? state.element_buffer->name : 0;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_COMPRESSED_TEXTURE_FORMATS:
		i[0] = GL_PALETTE4_RGB8_OES;
		i[1] = GL_PALETTE4_RGBA8_OES;
		i[2] = GL_PALETTE4_R5_G6_B5_OES;
		i[3] = GL_PALETTE4_RGBA4_OES;
		i[4] = GL_PALETTE4_RGB5_A1_OES;
		i[5] = GL_PALETTE8_RGB8_OES;
		i[6] = GL_PALETTE8_RGBA8_OES;
		i[7] = GL_PALETTE8_R5_G6_B5_OES;
		i[8] = GL_PALETTE8_RGBA4_OES;
		i[9] = GL_PALETTE8_RGB5_A1_OES;
		*type = GET_INT;
		*n = 10;
		break;

	case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
		i[0] = 10;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
		i[0] = GL_RGBA;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
		i[0] = GL_UNSIGNED_BYTE;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_CLIP_PLANES:
		i[0] = 6;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_ELEMENTS_INDICES:
		i[0] = 65535;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_ELEMENTS_VERTICES:
		i[0] = 65535;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_LIGHTS:
		i[0] = 8;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_MODELVIEW_STACK_DEPTH:
		i[0] = 16;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_PROJECTION_STACK_DEPTH:
		i[0] = 2;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_TEXTURE_STACK_DEPTH:
		i[0] = 2;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_TEXTURE_SIZE:
		i[0] = 256;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MAX_TEXTURE_UNITS:
		i[0] = state.max_texture_units;
		*type = GET_INT;
		*n = 1;
		break;

	case GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES:
	case GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES:
	case GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES:
		/* TODO: look up what this really means */
		assert(!"todo");
		break;
	}

	/* Move values to result */

	for (j = 0; j < *n; j++)
		switch (*type)
		{
		case GET_BOOLEAN: res[j].b = b[j]; break;
		case GET_INT: res[j].i = i[j]; break;
		//case GET_FLOAT: res[j].f = f[j]; break;
		case GET_FLOAT: assert(!"float"); break;
		case GET_DOUBLE: res[j].d = d[j]; break;
		case GET_FIXED: break;
		}

	return GL_NO_ERROR;
}

void GL_APIENTRY glGetBooleanv (GLenum pname, GLboolean *params)
{
	GLenum err;
	int i, n;
	union GetResult res[16];
	enum GetType type = GET_BOOLEAN;

	ENTER();
	
	err = do_get(pname, res, &type, &n);
	if (err != GL_NO_ERROR)
		ERROR(err);

	for (i = 0; i < n; i++)
		switch (type)
		{
		case GET_BOOLEAN: params[i] = res[i].b; break;
		case GET_INT: params[i] = res[i].i != 0; break;
		case GET_FIXED: params[i] = res[i].fix != 0; break;
		case GET_FLOAT: params[i] = res[i].f != 0.0f; break;
		case GET_DOUBLE: params[i] = res[i].d != 0.0; break;
		}

	LEAVE();
}

void GL_APIENTRY glGetIntegerv (GLenum pname, GLint *params)
{
#if 0
	ENTER();

	switch (pname)
	{
	case GL_MAX_TEXTURE_UNITS:
		params[0] = 2;
		break;

	default:
		dgl.glGetIntegerv(pname, params);
		break;
	}

	LEAVE();
#else
	GLenum err;
	int i, n;
	union GetResult res[16];
	enum GetType type = GET_INT;

	ENTER();

	err = do_get(pname, res, &type, &n);
	if (err != GL_NO_ERROR)
		ERROR(err);

	/* Some special values are converted differently
	 * (color, depth value, normal) */

	switch (pname)
	{
	case GL_ALPHA_TEST_REF:
	case GL_DEPTH_CLEAR_VALUE:
		params[0] = CLAMPED_TO_I(res[0].d);
		LEAVE();
	case GL_CURRENT_COLOR:
		params[0] = CLAMPED_TO_I(res[0].d);
		params[1] = CLAMPED_TO_I(res[1].d);
		params[2] = CLAMPED_TO_I(res[2].d);
		params[3] = CLAMPED_TO_I(res[3].d);
		LEAVE();
	}

	for (i = 0; i < n; i++)
		switch (type)
		{
		case GET_BOOLEAN: params[i] = res[i].b; break;
		case GET_INT: params[i] = res[i].i; break;
		case GET_FIXED: params[i] = X_TO_I(res[i].fix); break;
		case GET_FLOAT: params[i] = (int) res[i].f; break;
		case GET_DOUBLE: params[i] = (int) res[i].d; break;
		}

	LEAVE();
#endif
}

void GL_APIENTRY glGetFixedv (GLenum pname, GLfixed *params)
{
	GLenum err;
	int i, n;
	union GetResult res[16];
	enum GetType type = GET_FLOAT;

	ENTER();

	err = do_get(pname, res, &type, &n);
	if (err != GL_NO_ERROR)
		ERROR(err);

	for (i = 0; i < n; i++)
		switch (type)
		{
		case GET_BOOLEAN: params[i] = I_TO_X(res[i].b); break;
		case GET_INT: params[i] = I_TO_X(res[i].i); break;
		case GET_FIXED: params[i] = res[i].fix; break;
		case GET_FLOAT: params[i] = F_TO_X(res[i].f); break;
		case GET_DOUBLE: params[i] = D_TO_X(res[i].d); break;
		}

	LEAVE();
}

void GL_APIENTRY glGetFloatv (GLenum pname, GLfloat *params)
{
	GLenum err;
	int i, n;
	union GetResult res[16];
	enum GetType type = GET_FLOAT;

	ENTER();
	
	err = do_get(pname, res, &type, &n);
	if (err != GL_NO_ERROR)
		ERROR(err);

	for (i = 0; i < n; i++)
		switch (type)
		{
		case GET_BOOLEAN: params[i] = (float) res[i].b; break;
		case GET_INT: params[i] = (float) res[i].i; break;
		case GET_FIXED: params[i] = X_TO_F(res[i].fix); break;
		case GET_FLOAT: params[i] = res[i].f; break;
		case GET_DOUBLE: params[i] = (float) res[i].d; break;
		}

	LEAVE();
}

void GL_APIENTRY glGetPointerv (GLenum pname, void **params)
{
	ENTER();

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_COLOR_ARRAY_POINTER:
		*params = (void *) state.cptr.pointer;
		break;
	case GL_NORMAL_ARRAY_POINTER:
		*params = (void *) state.nptr.pointer;
		break;
	case GL_TEXTURE_COORD_ARRAY_POINTER:
		*params = (void *) state.tptr[state.active_texture].pointer;
		break;
	case GL_VERTEX_ARRAY_POINTER:
		*params = (void *) state.vptr.pointer;
		break;
	case GL_POINT_SIZE_ARRAY_POINTER_OES:
		*params = (void *) state.psptr.pointer;
		break;
	}

	LEAVE();
}

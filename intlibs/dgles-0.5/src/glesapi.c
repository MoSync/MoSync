#include <GLES/gl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <limits.h>
#include <string.h>
#include "state.h"
#include "dgl.h"

struct DGLES_State _dgles_state;
struct DGL _dgles_dgl;

#include "api.h"

GL_API int GL_APIENTRY _dgles_load_library(void *handle, void *(*load_func)(void *, const char *))
{
	void *ptr;

#define LOAD(x) \
	ptr = (*load_func)(handle, #x); \
	if (ptr == NULL) \
	{ \
		fprintf(stderr, "Symbol %s not found.\n", #x); \
		goto error; \
	} \
	* (void **) &dgl.x = ptr;

/* Load optional */
#define LOADO(x) \
	ptr = (*load_func)(handle, #x); \
	* (void **) &dgl.x = ptr;

#include "load.h"

#undef LOAD
#undef LOADO

	state.library_loaded = 1;
	return 0;

error:
	return 1;
}

void _dgles_init()
{
	if (state.inited)
		return;

	if (state.library_loaded == 0)
	{
		void *h = _dgles_platform_open();
		if (h == NULL)
		{
			fprintf(stderr, "Couldn't open OpenGL library\n");
			exit(1);
		}
		if (_dgles_load_library(h, _dgles_platform_sym) != 0)
		{
			fprintf(stderr, "Couldn't find necessary symbols from library\n");
			exit(1);
		}

		_dgles_platform_close(h);
	}

	/* Set some default values */

	memset(&state, 0, sizeof(state));

	state.inited = 1;
	state.library_loaded = 1;

	{
		GLint a;
		dgl.glGetIntegerv(GL_MAX_TEXTURE_UNITS, &a);
		assert(a >= 2);
		state.max_texture_units = (unsigned int) a;
		if (state.max_texture_units > MAX_TEXTURE_UNITS)
			state.max_texture_units = MAX_TEXTURE_UNITS;
	}

	state.vptr.size = 4;
	state.vptr.type = GL_FLOAT;
	state.cptr.size = 4;
	state.cptr.type = GL_FLOAT;
	state.nptr.size = 3;
	state.nptr.type = GL_FLOAT;

	{
		unsigned int i;
		for (i = 0; i < state.max_texture_units; i++)
		{
			state.tptr[i].size = 4;
			state.tptr[i].type = GL_FLOAT;
		}
	}

	state.psptr.size = 1;
	state.psptr.type = GL_FLOAT;

	state.ambient[0] = 0.2f;
	state.ambient[1] = 0.2f;
	state.ambient[2] = 0.2f;
	state.ambient[3] = 1.0f;

	state.diffuse[0] = 0.8f;
	state.diffuse[1] = 0.8f;
	state.diffuse[2] = 0.8f;
	state.diffuse[3] = 1.0f;

	/* Check available extensions */

	{
		const GLubyte *p = dgl.glGetString(GL_EXTENSIONS);
		const GLubyte *q = p;

		while (*q != '\0')
		{
			while (*q != ' ' && *q != '\0')
				q++;

			if (strncmp((const char *) p, "GL_ARB_point_sprite", q - p) == 0)
				state.arb_point_sprite = 1;

			if (*q == ' ')
				q++;

			p = q;
		}
	}
}

static int is_valid_enable_enum(GLenum cap)
{
	switch (cap)
	{
	default:
		return 0;

	case GL_DEPTH_TEST:
	case GL_SCISSOR_TEST:
	case GL_STENCIL_TEST:
	case GL_ALPHA_TEST:

	case GL_MULTISAMPLE:
	case GL_SAMPLE_COVERAGE:
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	case GL_SAMPLE_ALPHA_TO_ONE:

	case GL_COLOR_LOGIC_OP:
	case GL_COLOR_MATERIAL:
	case GL_FOG:

	case GL_BLEND:
	case GL_DITHER:
	case GL_CULL_FACE:

	case GL_LINE_SMOOTH:
	case GL_POINT_SMOOTH:
	case GL_NORMALIZE:
	case GL_RESCALE_NORMAL:
	case GL_POLYGON_OFFSET_FILL:

	case GL_TEXTURE_2D:

	case GL_POINT_SPRITE_OES:

	case GL_CLIP_PLANE0:
	case GL_CLIP_PLANE1:
	case GL_CLIP_PLANE2:
	case GL_CLIP_PLANE3:
	case GL_CLIP_PLANE4:
	case GL_CLIP_PLANE5:

	case GL_LIGHTING:
	case GL_LIGHT0:
	case GL_LIGHT1:
	case GL_LIGHT2:
	case GL_LIGHT3:
	case GL_LIGHT4:
	case GL_LIGHT5:
	case GL_LIGHT6:
	case GL_LIGHT7:
		return 1;
	}
}

void GL_APIENTRY glClear (GLbitfield mask)
{
	ENTER();

	if (mask & ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))
		ERROR(GL_INVALID_VALUE);

	dgl.glClear(mask);

	LEAVE();
}

void GL_APIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	ENTER();
	dgl.glClearColor(red, green, blue, alpha);
	LEAVE();
}

void GL_APIENTRY glClearColorx (GLclampx red, GLclampx green,
                             GLclampx blue, GLclampx alpha)
{
	ENTER();
	dgl.glClearColor(X_TO_F(red), X_TO_F(green), X_TO_F(blue), X_TO_F(alpha));
	LEAVE();
}

void GL_APIENTRY glClearDepthf (GLclampf depth)
{
	ENTER();
	dgl.glClearDepth(depth);
	LEAVE();
}

void GL_APIENTRY glClearDepthx (GLclampx depth)
{
	ENTER();
	dgl.glClearDepth(X_TO_D(depth));
	LEAVE();
}

void GL_APIENTRY glClearStencil (GLint s) {
	ENTER();
	dgl.glClearStencil(s);
	LEAVE();
}

void GL_APIENTRY glColor4f (GLfloat red, GLfloat green,
                         GLfloat blue, GLfloat alpha)
{
	ENTER();
	dgl.glColor4f(red, green, blue, alpha);
	LEAVE();
}

void GL_APIENTRY glColor4x (GLfixed red, GLfixed green,
                         GLfixed blue, GLfixed alpha)
{
	ENTER();
	dgl.glColor4d(X_TO_D(red), X_TO_D(green), X_TO_D(blue), X_TO_D(alpha));
	LEAVE();
}

void GL_APIENTRY glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	ENTER();
	dgl.glColor4ub(red, green, blue, alpha);
	LEAVE();
}

void GL_APIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	ENTER();
	dgl.glColorMask(red, green, blue, alpha);
	LEAVE();
}

/*
 * Alpha test
 */

static int is_valid_comparison_func(GLenum func)
{
	switch (func)
	{
	default:
		return 0;
	case GL_ALWAYS:
	case GL_NEVER:
	case GL_LESS:
	case GL_LEQUAL:
	case GL_GREATER:
	case GL_GEQUAL:
	case GL_EQUAL:
	case GL_NOTEQUAL:
		return 1;
	}
}

void GL_APIENTRY glAlphaFunc (GLenum func, GLclampf ref)
{
	ENTER();

	if (!is_valid_comparison_func(func))
		ERROR(GL_INVALID_ENUM);

	dgl.glAlphaFunc(func, ref);

	LEAVE();
}

void GL_APIENTRY glAlphaFuncx (GLenum func, GLclampx ref)
{
	ENTER();

	if (!is_valid_comparison_func(func))
		ERROR(GL_INVALID_ENUM);

	dgl.glAlphaFunc(func, X_TO_F(ref));

	LEAVE();
}

/*
 * Blending
 */

void GL_APIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor)
{
	ENTER();

	switch (sfactor)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_ZERO:
	case GL_ONE:
	case GL_DST_COLOR:
	case GL_ONE_MINUS_DST_COLOR:
	case GL_SRC_ALPHA:
	case GL_ONE_MINUS_SRC_ALPHA:
	case GL_DST_ALPHA:
	case GL_ONE_MINUS_DST_ALPHA:
	case GL_SRC_ALPHA_SATURATE:
		break;
	}

	switch (dfactor)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_ZERO:
	case GL_ONE:
	case GL_SRC_COLOR:
	case GL_ONE_MINUS_SRC_COLOR:
	case GL_SRC_ALPHA:
	case GL_ONE_MINUS_SRC_ALPHA:
	case GL_DST_ALPHA:
	case GL_ONE_MINUS_DST_ALPHA:
		break;
	}

	dgl.glBlendFunc(sfactor, dfactor);

	LEAVE();
}

/*
 * Depth test
 */

void GL_APIENTRY glDepthFunc (GLenum func)
{
	ENTER();

	if (!is_valid_comparison_func(func))
		ERROR(GL_INVALID_ENUM);

	dgl.glDepthFunc(func);

	LEAVE();
}

void GL_APIENTRY glDepthMask (GLboolean flag)
{
	ENTER();
	dgl.glDepthMask(flag);
	LEAVE();
}

void GL_APIENTRY glDepthRangef (GLclampf zNear, GLclampf zFar)
{
	ENTER();
	dgl.glDepthRange(zNear, zFar);
	LEAVE();
}

void GL_APIENTRY glDepthRangex (GLclampx zNear, GLclampx zFar)
{
	ENTER();
	dgl.glDepthRange(X_TO_D(zNear), X_TO_D(zFar));
	LEAVE();
}

/*
 * Stencil test
 */

static int is_valid_stencil_op(GLenum op)
{
	switch (op)
	{
	default:
		return 0;
	case GL_KEEP:
	case GL_ZERO:
	case GL_REPLACE:
	case GL_INCR:
	case GL_DECR:
	case GL_INVERT:
		return 1;
	}
}

void GL_APIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask)
{
	ENTER();

	if (!is_valid_comparison_func(func))
		ERROR(GL_INVALID_ENUM);

	dgl.glStencilFunc(func, ref, mask);

	LEAVE();
}

void GL_APIENTRY glStencilMask (GLuint mask)
{
	ENTER();
	dgl.glStencilMask(mask);
	LEAVE();
}

void GL_APIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
{
	ENTER();

	if (!is_valid_stencil_op(fail) ||
	    !is_valid_stencil_op(zfail) ||
	    !is_valid_stencil_op(zpass))
		ERROR(GL_INVALID_ENUM);

	dgl.glStencilOp(fail, zfail, zpass);

	LEAVE();
}

/*
 * Enable / Disable
 */

void GL_APIENTRY glEnable (GLenum cap)
{
	ENTER();

	if (!is_valid_enable_enum(cap))
		ERROR(GL_INVALID_ENUM);

	if (cap == GL_POINT_SPRITE_OES)
		if (state.arb_point_sprite)
			dgl.glEnable(GL_POINT_SPRITE_ARB);
		else
			LEAVE();
	else
		dgl.glEnable(cap);

	LEAVE();
}

void GL_APIENTRY glDisable (GLenum cap)
{
	ENTER();

	if (!is_valid_enable_enum(cap))
		ERROR(GL_INVALID_ENUM);

	if (cap == GL_POINT_SPRITE_OES)
		if (state.arb_point_sprite)
			dgl.glDisable(GL_POINT_SPRITE_ARB);
		else
			LEAVE();
	else
		dgl.glDisable(cap);

	/* Set back old ambient and diffuse */

	if (cap == GL_COLOR_MATERIAL)
	{
		dgl.glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, state.ambient);
		dgl.glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, state.diffuse);
	}

	LEAVE();
}

GLboolean GL_APIENTRY glIsEnabled (GLenum cap)
{
	GLboolean ret;

	ENTER();

	switch (cap)
	{
	case GL_POINT_SPRITE_OES:
		ret = dgl.glIsEnabled(GL_POINT_SPRITE_ARB);
		break;
	case GL_VERTEX_ARRAY:
		ret = (state.array_flags & VERTEX_ARRAY) != 0;
		break;
	case GL_NORMAL_ARRAY:
		ret = (state.array_flags & NORMAL_ARRAY) != 0;
		break;
	case GL_COLOR_ARRAY:
		ret = (state.array_flags & COLOR_ARRAY) != 0;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		ret = (state.array_flags &
				(TEXCOORD0_ARRAY << state.active_texture)) != 0;
		break;
	case GL_POINT_SIZE_ARRAY_OES:
		ret = (state.array_flags & POINT_SIZE_ARRAY) != 0;
		break;
	default:
		if (!is_valid_enable_enum(cap))
			ERROR2(GL_INVALID_ENUM, GL_FALSE);
		ret = dgl.glIsEnabled(cap);
		break;
	}

	assert(ret == GL_TRUE || ret == GL_FALSE);

	LEAVE2(ret);
}

void GL_APIENTRY glEnableClientState (GLenum array)
{
	ENTER();

	/* Check that array is valid */

	switch (array)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_COLOR_ARRAY:
		state.array_flags |= COLOR_ARRAY;
		break;
	case GL_VERTEX_ARRAY:
		state.array_flags |= VERTEX_ARRAY;
		break;
	case GL_NORMAL_ARRAY:
		state.array_flags |= NORMAL_ARRAY;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		state.array_flags |= TEXCOORD0_ARRAY << state.active_texture;
		break;

	case GL_POINT_SIZE_ARRAY_OES:
		state.array_flags |= POINT_SIZE_ARRAY;
		LEAVE();
	}

	dgl.glEnableClientState(array);

	LEAVE();
}

void GL_APIENTRY glDisableClientState (GLenum array)
{
	ENTER();

	/* Check that array is valid */

	switch (array)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_COLOR_ARRAY:
		state.array_flags &= ~COLOR_ARRAY;
		break;
	case GL_VERTEX_ARRAY:
		state.array_flags &= ~VERTEX_ARRAY;
		break;
	case GL_NORMAL_ARRAY:
		state.array_flags &= ~NORMAL_ARRAY;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		state.array_flags &= ~(TEXCOORD0_ARRAY << state.active_texture);
		break;
	case GL_POINT_SIZE_ARRAY_OES:
		state.array_flags &= ~POINT_SIZE_ARRAY;
		LEAVE();
	}

	dgl.glDisableClientState(array);

	LEAVE();
}

void GL_APIENTRY glFinish (void)
{
	ENTER();
	dgl.glFinish();
	LEAVE();
}

void GL_APIENTRY glFlush (void)
{
	ENTER();
	dgl.glFlush();
	LEAVE();
}

/*
 * Fog
 */

static int is_valid_single_valued_fog_parm(GLenum pname)
{
	switch (pname)
	{
	default:
		return 0;
	case GL_FOG_MODE:
	case GL_FOG_DENSITY:
	case GL_FOG_START:
	case GL_FOG_END:
		return 1;
	}
}

static int is_valid_fog_parm(GLenum pname)
{
	switch (pname)
	{
	default:
		return 0;
	case GL_FOG_MODE:
	case GL_FOG_DENSITY:
	case GL_FOG_START:
	case GL_FOG_END:
	case GL_FOG_COLOR:
		return 1;
	}
}

static int is_valid_fogmode(GLenum mode)
{
	switch (mode)
	{
	default:
		return 0;
	case GL_LINEAR:
	case GL_EXP:
	case GL_EXP2:
		return 1;
	}
}

void GL_APIENTRY glFogf (GLenum pname, GLfloat param)
{
	ENTER();

	if (!is_valid_single_valued_fog_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (pname == GL_FOG_DENSITY && param < 0)
		ERROR(GL_INVALID_VALUE);
	else if (pname == GL_FOG_MODE && !is_valid_fogmode((GLenum) param))
		ERROR(GL_INVALID_ENUM);

	dgl.glFogf(pname, param);

	LEAVE();
}

void GL_APIENTRY glFogx (GLenum pname, GLfixed param)
{
	ENTER();

	if (!is_valid_single_valued_fog_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (pname == GL_FOG_DENSITY && param < 0)
		ERROR(GL_INVALID_VALUE);
	else if (pname == GL_FOG_MODE)
	{
		if (!is_valid_fogmode(param))
			ERROR(GL_INVALID_ENUM);
		
		dgl.glFogi(pname, param);
		LEAVE();
	}

	dgl.glFogf(pname, X_TO_F(param));

	LEAVE();
}

void GL_APIENTRY glFogfv (GLenum pname, const GLfloat *params)
{
	ENTER();

	if (!is_valid_fog_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (pname == GL_FOG_DENSITY && params[0] < 0)
		ERROR(GL_INVALID_VALUE);
	else if (pname == GL_FOG_MODE && !is_valid_fogmode((GLenum) params[0]))
		ERROR(GL_INVALID_ENUM);

	dgl.glFogfv(pname, params);

	LEAVE();
}

void GL_APIENTRY glFogxv (GLenum pname, const GLfixed *params)
{
	float f[4];

	ENTER();

	if (!is_valid_fog_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (pname == GL_FOG_DENSITY && params[0] < 0)
		ERROR(GL_INVALID_VALUE);
	else if (pname == GL_FOG_MODE)
	{
		if (!is_valid_fogmode(params[0]))
			ERROR(GL_INVALID_ENUM);

		dgl.glFogi(GL_FOG_MODE, params[0]);
		LEAVE();
	}

	f[0] = X_TO_F(params[0]);
	if (pname == GL_FOG_COLOR)
	{
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		f[3] = X_TO_F(params[3]);
	}

	dgl.glFogfv(pname, f);

	LEAVE();
}

/*
 * Polygon
 */

void GL_APIENTRY glFrontFace (GLenum mode)
{
	ENTER();

	if (mode != GL_CW && mode != GL_CCW)
		ERROR(GL_INVALID_ENUM);

	dgl.glFrontFace(mode);

	LEAVE();
}

void GL_APIENTRY glCullFace (GLenum mode)
{
	ENTER();

	if (mode != GL_FRONT && mode != GL_BACK && mode != GL_FRONT_AND_BACK)
		ERROR(GL_INVALID_ENUM);

	dgl.glCullFace(mode);

	LEAVE();
}

/*
 * Error and string
 */

GLenum GL_APIENTRY glGetError (void)
{
	GLenum error;
	ENTER();
	error = state.error;
	state.error = GL_NO_ERROR;
	LEAVE2(error);
}

const GLubyte * GL_APIENTRY glGetString (GLenum name)
{
	ENTER();

	switch (name)
	{
	case GL_VENDOR: LEAVE2((const GLubyte *) "hipo");
	case GL_RENDERER: LEAVE2((const GLubyte *) "dgles");
	case GL_VERSION: LEAVE2((const GLubyte *) "OpenGL ES-CM 1.1");
	case GL_EXTENSIONS: LEAVE2((const GLubyte *)
		(state.arb_point_sprite ?
		"GL_OES_byte_coordinate "
		"GL_OES_fixed_point "
		"GL_OES_single_precision "
		"GL_OES_matrix_get "
		"GL_OES_read_format "
		"GL_OES_compressed_paletted_texture "
		"GL_OES_point_size_array "
		"GL_OES_point_sprite"
		:
		"GL_OES_byte_coordinate "
		"GL_OES_fixed_point "
		"GL_OES_single_precision "
		"GL_OES_matrix_get "
		"GL_OES_read_format "
		"GL_OES_compressed_paletted_texture "
		"GL_OES_point_size_array"));
	default: ERROR2(GL_INVALID_ENUM, 0);
	}
}

void GL_APIENTRY glHint (GLenum target, GLenum mode)
{
	ENTER();

	switch (target)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_FOG_HINT:
	case GL_GENERATE_MIPMAP_HINT:
	case GL_LINE_SMOOTH_HINT:
	case GL_PERSPECTIVE_CORRECTION_HINT:
	case GL_POINT_SMOOTH_HINT:
		break;
	}

	switch (mode)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_FASTEST:
	case GL_NICEST:
	case GL_DONT_CARE:
		break;
	}

	dgl.glHint(target, mode);

	LEAVE();
}

void GL_APIENTRY glPixelStorei (GLenum pname, GLint param)
{
	ENTER();

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_PACK_ALIGNMENT:
	case GL_UNPACK_ALIGNMENT:
		break;
	}

	switch (param)
	{
	default:
		ERROR(GL_INVALID_VALUE);
	case 1:
	case 2:
	case 4:
	case 8:
		break;
	}

	dgl.glPixelStorei(pname, param);

	LEAVE();
}

/*
 * Logic op
 */

void GL_APIENTRY glLogicOp (GLenum opcode)
{
	ENTER();

	switch (opcode)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_CLEAR:
	case GL_SET:
	case GL_COPY:
	case GL_COPY_INVERTED:
	case GL_NOOP:
	case GL_INVERT:
	case GL_AND:
	case GL_NAND:
	case GL_OR:
	case GL_NOR:
	case GL_XOR:
	case GL_EQUIV:
	case GL_AND_REVERSE:
	case GL_AND_INVERTED:
	case GL_OR_REVERSE:
	case GL_OR_INVERTED:
		break;
	}

	dgl.glLogicOp(opcode);

	LEAVE();
}

void GL_APIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
	ENTER();
	dgl.glNormal3f(nx, ny, nz);
	LEAVE();
}

void GL_APIENTRY glNormal3x (GLfixed nx, GLfixed ny, GLfixed nz)
{
	ENTER();
	dgl.glNormal3d(X_TO_D(nx), X_TO_D(ny), X_TO_D(nz));
	LEAVE();
}

void GL_APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	ENTER();

	if (width < 0 || height < 0)
		ERROR(GL_INVALID_VALUE);

	dgl.glViewport(x, y, width, height);

	LEAVE();
}

void GL_APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	ENTER();

	if (width < 0 || height < 0)
		ERROR(GL_INVALID_VALUE);

	if (format == GL_RGBA && type == GL_UNSIGNED_BYTE)
	{
		dgl.glReadPixels(x, y, width, height, format, type, pixels);
		LEAVE();
	}

	if (format == GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES &&
	    type == GL_IMPLEMENTATION_COLOR_READ_TYPE_OES)
	{
		dgl.glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
		                 pixels);
		LEAVE();
	}

	ERROR(GL_INVALID_ENUM);
}

void GL_APIENTRY glSampleCoverage (GLclampf value, GLboolean invert)
{
	(void) value;
	(void) invert;

	ENTER();
#if 0
	fprintf(stderr, "%f %d\n", value, invert);
	dgl.glSampleCoverage(value, invert);
#endif
	LEAVE();
}

void GL_APIENTRY glSampleCoveragex (GLclampx value, GLboolean invert)
{
	ENTER();
	dgl.glSampleCoverage(X_TO_F(value), invert);
	LEAVE();
}

void GL_APIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
	ENTER();

	if (width < 0 || height < 0)
		ERROR(GL_INVALID_VALUE);

	dgl.glScissor(x, y, width, height);

	LEAVE();
}

void GL_APIENTRY glShadeModel (GLenum mode)
{
	ENTER();

	if (mode != GL_FLAT && mode != GL_SMOOTH)
		ERROR(GL_INVALID_ENUM);
	dgl.glShadeModel(mode);

	LEAVE();
}

/*
 * Clip plane
 */

void GL_APIENTRY glClipPlanef (GLenum plane, const GLfloat *equation)
{
	int i;
	GLdouble eq[4];

	ENTER();

	for (i = 0; i < 4; i++)
		eq[i] = (double) equation[i];
	dgl.glClipPlane(plane, eq);

	LEAVE();
}

void GL_APIENTRY glClipPlanex (GLenum plane, const GLfixed *equation)
{
	int i;
	GLdouble eq[4];

	ENTER();

	for (i = 0; i < 4; i++)
		eq[i] = X_TO_D(equation[i]);
	dgl.glClipPlane(plane, eq);

	LEAVE();
}

void GL_APIENTRY glGetClipPlanef (GLenum pname, GLfloat eqn[4])
{
	int i;
	GLdouble eq[4];

	ENTER();

	dgl.glGetClipPlane(pname, eq);
	for (i = 0; i < 4; i++)
		eqn[i] = (float) eq[i];

	LEAVE();
}

void GL_APIENTRY glGetClipPlanex (GLenum pname, GLfixed eqn[4])
{
	int i;
	GLdouble eq[4];

	ENTER();

	dgl.glGetClipPlane(pname, eq);
	for (i = 0; i < 4; i++)
		eqn[i] = D_TO_X(eq[i]);

	LEAVE();
}

/*
 * Point parameter
 */

static int is_valid_single_valued_point_parm(GLenum pname)
{
	switch (pname)
	{
	default:
		return 0;
	case GL_POINT_SIZE_MIN:
	case GL_POINT_SIZE_MAX:
	case GL_POINT_FADE_THRESHOLD_SIZE:
		return 1;
	}
}

static int is_valid_point_parm(GLenum pname)
{
	switch (pname)
	{
	default:
		return 0;
	case GL_POINT_SIZE_MIN:
	case GL_POINT_SIZE_MAX:
	case GL_POINT_FADE_THRESHOLD_SIZE:
	case GL_POINT_DISTANCE_ATTENUATION:
		return 1;
	}
}

void GL_APIENTRY glPointParameterf (GLenum pname, GLfloat param)
{
	ENTER();

	if (!is_valid_single_valued_point_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (param < 0)
		ERROR(GL_INVALID_VALUE);

	if (!dgl.glPointParameterf)
		ERROR(GL_INVALID_ENUM);

	dgl.glPointParameterf(pname, param);

	LEAVE();
}

void GL_APIENTRY glPointParameterx (GLenum pname, GLfixed param)
{
	ENTER();

	if (!is_valid_single_valued_point_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (param < 0)
		ERROR(GL_INVALID_VALUE);

	if (!dgl.glPointParameterf)
		ERROR(GL_INVALID_ENUM);

	dgl.glPointParameterf(pname, X_TO_F(param));

	LEAVE();
}

void GL_APIENTRY glPointParameterfv (GLenum pname, const GLfloat *params)
{
	ENTER();

	if (!is_valid_point_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (is_valid_single_valued_point_parm(pname) && params[0] < 0)
		ERROR(GL_INVALID_VALUE);

	if (!dgl.glPointParameterfv)
		ERROR(GL_INVALID_ENUM);

	dgl.glPointParameterfv(pname, params);

	LEAVE();
}

void GL_APIENTRY glPointParameterxv (GLenum pname, const GLfixed *params)
{
	float f[4];

	ENTER();

	if (!is_valid_point_parm(pname))
		ERROR(GL_INVALID_ENUM);

	if (is_valid_single_valued_point_parm(pname))
	{
		if (params[0] < 0)
			ERROR(GL_INVALID_VALUE);
	}
	else
	{
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		f[3] = X_TO_F(params[3]);
	}

	f[0] = X_TO_F(params[0]);

	if (!dgl.glPointParameterfv)
		ERROR(GL_INVALID_ENUM);

	dgl.glPointParameterfv(pname, f);

	LEAVE();
}

void GL_APIENTRY glLineWidth (GLfloat width)
{
	ENTER();
	if (width <= 0)
		ERROR(GL_INVALID_VALUE);
	dgl.glLineWidth(width);
	LEAVE();
}

void GL_APIENTRY glLineWidthx (GLfixed width)
{
	ENTER();
	if (width <= 0)
		ERROR(GL_INVALID_VALUE);
	dgl.glLineWidth(X_TO_F(width));
	LEAVE();
}

void GL_APIENTRY glPointSize (GLfloat size)
{
	ENTER();
	if (size <= 0)
		ERROR(GL_INVALID_VALUE);
	dgl.glPointSize(size);
	LEAVE();
}

void GL_APIENTRY glPointSizex (GLfixed size)
{
	ENTER();
	if (size <= 0)
		ERROR(GL_INVALID_VALUE);
	dgl.glPointSize(X_TO_F(size));
	LEAVE();
}

void GL_APIENTRY glPolygonOffset (GLfloat factor, GLfloat units)
{
	ENTER();
	dgl.glPolygonOffset(factor, units);
	LEAVE();
}

void GL_APIENTRY glPolygonOffsetx (GLfixed factor, GLfixed units)
{
	ENTER();
	dgl.glPolygonOffset(X_TO_F(factor), X_TO_F(units));
	LEAVE();
}

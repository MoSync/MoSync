#include "api.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Textures
 */

void GL_APIENTRY glGenTextures (GLsizei n, GLuint *textures)
{
	ENTER();

	if (n < 0)
		ERROR(GL_INVALID_VALUE);

	dgl.glGenTextures(n, textures);

	LEAVE();
}

void GL_APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures)
{
	ENTER();

	if (n < 0)
		ERROR(GL_INVALID_VALUE);

	dgl.glDeleteTextures(n, textures);

	LEAVE();
}

void GL_APIENTRY glActiveTexture (GLenum texture)
{
	ENTER();

	if (texture < GL_TEXTURE0 ||
	    texture >= GL_TEXTURE0 + state.max_texture_units)
		ERROR(GL_INVALID_ENUM);

	dgl.glActiveTexture(texture);

	LEAVE();
}

void GL_APIENTRY glBindTexture (GLenum target, GLuint texture)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	dgl.glBindTexture(target, texture);

	LEAVE();
}

GLboolean GL_APIENTRY glIsTexture (GLuint texture)
{
	GLboolean ret;

	ENTER();

	ret = dgl.glIsTexture(texture);

	LEAVE2(ret);
}

void GL_APIENTRY glClientActiveTexture (GLenum texture)
{
	ENTER();

	if (texture < GL_TEXTURE0 ||
	    texture >= GL_TEXTURE0 + state.max_texture_units)
		ERROR(GL_INVALID_ENUM);

	state.active_texture = texture - GL_TEXTURE0;

	dgl.glClientActiveTexture(texture);

	LEAVE();
}

static GLenum uncompress_image(int l, int tl, GLenum internalformat,
                               GLsizei width, GLsizei height,
                               GLsizei imageSize, const GLvoid *data,
                               GLenum *newformat, GLvoid **newdata_)
{
	GLubyte *newdata;
	const GLubyte *palette;
	const GLubyte *imagedata;
	int is_alpha;
	int i;
	int total;
	int skip;

	total = 0;
	for (i = 0; i <= tl; i++)
		total += (width >> i) * (height >> i);

	skip = 0;
	for (i = 0; i < l; i++)
	{
		skip += width * height;
		width /= 2;
		height /= 2;
	}

	/* TODO: different levels should be tested! */
#if 0
	printf("level %d/%d total %d skip %d\n", l, tl, total, skip);
#endif

	palette = (const GLubyte *) data;

	switch (internalformat)
	{
	default:
		return GL_INVALID_ENUM;
	case GL_PALETTE4_RGB8_OES:
		if (imageSize != 16*3 + total / 2)
			return GL_INVALID_VALUE;
		imagedata = palette + 16 * 3 + skip / 2;
		is_alpha = 0;
		break;
	case GL_PALETTE4_R5_G6_B5_OES:
		if (imageSize != 16*2 + total / 2)
			return GL_INVALID_VALUE;
		imagedata = palette + 16 * 2 + skip / 2;
		is_alpha = 0;
		break;
	case GL_PALETTE8_RGB8_OES:
		if (imageSize != 256*3 + total)
			return GL_INVALID_VALUE;
		imagedata = palette + 256 * 3 + skip;
		is_alpha = 0;
		break;
	case GL_PALETTE8_R5_G6_B5_OES:
		if (imageSize != 256*2 + total)
			return GL_INVALID_VALUE;
		imagedata = palette + 256 * 2 + skip;
		is_alpha = 0;
		break;
	case GL_PALETTE4_RGBA8_OES:
		if (imageSize != 16*4 + total / 2)
			return GL_INVALID_VALUE;
		imagedata = palette + 16 * 4 + skip / 2;
		is_alpha = 1;
		break;
	case GL_PALETTE4_RGBA4_OES:
		if (imageSize != 16*2 + total / 2)
			return GL_INVALID_VALUE;
		imagedata = palette + 16 * 2 + skip / 2;
		is_alpha = 1;
		break;
	case GL_PALETTE4_RGB5_A1_OES:
		if (imageSize != 16*2 + total / 2)
			return GL_INVALID_VALUE;
		imagedata = palette + 16 * 2 + skip / 2;
		is_alpha = 1;
		break;
	case GL_PALETTE8_RGBA8_OES:
		if (imageSize != 256*4 + total)
			return GL_INVALID_VALUE;
		imagedata = palette + 256 * 4 + skip;
		is_alpha = 1;
		break;
	case GL_PALETTE8_RGBA4_OES:
		if (imageSize != 256*2 + total)
			return GL_INVALID_VALUE;
		imagedata = palette + 256 * 2 + skip;
		is_alpha = 1;
		break;
	case GL_PALETTE8_RGB5_A1_OES:
		if (imageSize != 256*2 + total)
			return GL_INVALID_VALUE;
		imagedata = palette + 256 * 2 + skip;
		is_alpha = 1;
		break;
	}

	newdata = malloc(width * height * (3 + is_alpha));
	if (newdata == NULL)
		return GL_OUT_OF_MEMORY;

	for (i = 0; (int) i < width * height; i++)
	{
		int r, g, b, a = 0;
		int index;

		switch (internalformat)
		{
		default:
			free(newdata);
			return GL_INVALID_ENUM;
		case GL_PALETTE4_RGB8_OES:
		case GL_PALETTE4_RGBA8_OES:
		case GL_PALETTE4_R5_G6_B5_OES:
		case GL_PALETTE4_RGBA4_OES:
		case GL_PALETTE4_RGB5_A1_OES:
			if (i & 1)
				index = imagedata[i / 2] & 15;
			else
				index = imagedata[i / 2] >> 4;
			break;
		case GL_PALETTE8_RGB8_OES:
		case GL_PALETTE8_RGBA8_OES:
		case GL_PALETTE8_R5_G6_B5_OES:
		case GL_PALETTE8_RGBA4_OES:
		case GL_PALETTE8_RGB5_A1_OES:
			index = imagedata[i];
			break;
		}

		switch (internalformat)
		{
		default:
			free(newdata);
			return GL_INVALID_ENUM;
		case GL_PALETTE4_RGB8_OES:
		case GL_PALETTE8_RGB8_OES:
			r = palette[index*3];
			g = palette[index*3+1];
			b = palette[index*3+2];
			break;
		case GL_PALETTE4_RGBA8_OES:
		case GL_PALETTE8_RGBA8_OES:
			r = palette[index*4];
			g = palette[index*4+1];
			b = palette[index*4+2];
			a = palette[index*4+3];
			break;
		case GL_PALETTE4_R5_G6_B5_OES:
		case GL_PALETTE8_R5_G6_B5_OES:
			r = palette[index*2+1] >> 3;
			r = (r << 3) | (r >> 2);
			g = ((palette[index*2+1] & 7) << 3) | (palette[index*2] >> 5);
			g = (g << 2) | (g >> 4);
			b = palette[index*2] & 0x1F;
			b = (b << 3) | (b >> 2);
			break;
		case GL_PALETTE4_RGBA4_OES:
		case GL_PALETTE8_RGBA4_OES:
			r = palette[index*2+1] >> 4;
			r |= (r << 4) | r;
			g = palette[index*2+1] & 0xF;
			g |= (g << 4) | g;
			b = palette[index*2] >> 4;
			b |= (b << 4) | b;
			a = palette[index*2] & 0xF;
			a |= (a << 4) | a;
			break;
		case GL_PALETTE4_RGB5_A1_OES:
		case GL_PALETTE8_RGB5_A1_OES:
			r = palette[index*2+1] >> 3;
			r = (r << 3) | (r >> 2);
			g = ((palette[index*2+1] & 7) << 2) | (palette[index*2] >> 6);
			g = (g << 3) | (g >> 2);
			b = (palette[index*2] >> 1) & 0x1F;
			b = (b << 3) | (b >> 2);
			a = (palette[index*2] & 1) ? 255 : 0;
			break;
		}

		if (is_alpha)
		{
			newdata[i*4+0] = r;
			newdata[i*4+1] = g;
			newdata[i*4+2] = b;
			newdata[i*4+3] = a;
		}
		else
		{
			newdata[i*3+0] = r;
			newdata[i*3+1] = g;
			newdata[i*3+2] = b;
		}
	}

	*newformat = is_alpha ? GL_RGBA : GL_RGB;
	*newdata_ = newdata;

	return GL_NO_ERROR;
}

void GL_APIENTRY glCompressedTexImage2D (GLenum target, GLint level,
                                         GLenum internalformat, GLsizei width,
                                         GLsizei height, GLint border,
                                         GLsizei imageSize, const GLvoid *data)
{
	GLenum newformat;
	GLvoid *newdata;
	GLenum err;
	int i, w, h;

	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (level > 0)
		ERROR(GL_INVALID_VALUE);

	if (border != 0)
		ERROR(GL_INVALID_VALUE);

	w = width;
	h = height;
	for (i = 0; i >= level && w && h; i--)
	{
		err = uncompress_image(-i, -level, internalformat, width, height,
		                       imageSize, data,
		                       &newformat, &newdata);
		if (err != GL_NO_ERROR)
			ERROR(err);

		dgl.glTexImage2D(GL_TEXTURE_2D, -i, newformat, w, h, 0,
		                 newformat, GL_UNSIGNED_BYTE, newdata);

		free(newdata);

		w /= 2;
		h /= 2;
	}

	LEAVE();
}

void GL_APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
	ENTER();

	(void) target;
	(void) level;
	(void) xoffset;
	(void) yoffset;
	(void) width;
	(void) height;
	(void) format;
	(void) imageSize;
	(void) data;

	/* There are no supported formats so this always causes error. */
	ERROR(GL_INVALID_ENUM);

	//LEAVE();
}

void GL_APIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (level < 0)
		ERROR(GL_INVALID_VALUE);

	if (border != 0)
		ERROR(GL_INVALID_VALUE);

	switch (internalformat)
	{
	default:
		ERROR(GL_INVALID_VALUE);
	case GL_ALPHA:
	case GL_LUMINANCE:
	case GL_LUMINANCE_ALPHA:
	case GL_RGB:
	case GL_RGBA:
		break;
	}

	/* TODO: check x + width <= texwidth && y + height <= texheight */
	if (x < 0 || y < 0 || width < 0 || height < 0)
		ERROR(GL_INVALID_VALUE);

	dgl.glCopyTexImage2D(target, level, internalformat, x, y, width, height, 0);

	LEAVE();
}

void GL_APIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (level < 0)
		ERROR(GL_INVALID_VALUE);

	/* TODO: check x + width <= texwidth && y + height <= texheight */
	if (x < 0 || y < 0 || width < 0 || height < 0 ||
	    xoffset < 0 || yoffset < 0)
		ERROR(GL_INVALID_VALUE);

	dgl.glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y,
	                        width, height);

	{
		GLenum err = dgl.glGetError();
		if (err)
			ERROR(err);
	}

	LEAVE();
}

void GL_APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
	ENTER();

	if (target < GL_TEXTURE0 ||
	    target >= GL_TEXTURE0 + state.max_texture_units)
		ERROR(GL_INVALID_ENUM);

	dgl.glMultiTexCoord4f(target, s, t, r, q);

	LEAVE();
}

void GL_APIENTRY glMultiTexCoord4x (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	ENTER();

	if (target < GL_TEXTURE0 ||
	    target >= GL_TEXTURE0 + state.max_texture_units)
		ERROR(GL_INVALID_ENUM);

	dgl.glMultiTexCoord4d(target, X_TO_F(s), X_TO_F(t), X_TO_F(r), X_TO_F(q));

	LEAVE();
}

void GL_APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	GLenum real_format;

	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (level < 0)
		ERROR(GL_INVALID_VALUE);

	switch (internalformat)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_RGBA:
	case GL_RGB:
	case GL_LUMINANCE:
	case GL_ALPHA:
	case GL_LUMINANCE_ALPHA:
		break;
	}

	if (internalformat != (int) format)
		ERROR(GL_INVALID_OPERATION);

	if (width < 0 || height < 0 ||
	    (width & (width - 1)) != 0 || (height & (height - 1)) != 0)
		ERROR(GL_INVALID_VALUE);

	if (border != 0)
		ERROR(GL_INVALID_VALUE);

	switch (type)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_UNSIGNED_BYTE:
		real_format = format;
		break;
	case GL_UNSIGNED_SHORT_5_6_5:
		real_format = GL_RGB;
		break;
	case GL_UNSIGNED_SHORT_4_4_4_4:
	case GL_UNSIGNED_SHORT_5_5_5_1:
		real_format = GL_RGBA;
		break;
	}

	/* TODO: this is wrong! */
	if (width == 0 || height == 0)
		LEAVE();

	/* TODO: there was some problems in Mesa with this */

#if 1
	dgl.glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0,
	                 real_format, type, pixels);
#else
	dgl.glTexImage2D(GL_TEXTURE_2D, level, 4, width, height, 0,
	                 GL_RGBA, type, pixels);
#endif

#if 0
	{
	GLenum err = dgl.glGetError();
	fprintf(stderr, "ERR %x\n", err);
	}

	fprintf(stderr, "l:%d w:%d h:%d %x %x %x\n", level, width, height, internalformat, real_format, type);
#endif

	LEAVE();
}

void GL_APIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (level < 0)
		ERROR(GL_INVALID_VALUE);

	switch (format)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_RGBA:
	case GL_RGB:
	case GL_LUMINANCE:
	case GL_ALPHA:
	case GL_LUMINANCE_ALPHA:
		break;
	}

	/* TODO: check x + width <= texwidth && y + height <= texheight */
	if (xoffset < 0 || yoffset < 0 || width < 0 || height < 0)
		ERROR(GL_INVALID_VALUE);

	switch (type)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_UNSIGNED_BYTE:
	case GL_UNSIGNED_SHORT_5_6_5:
	case GL_UNSIGNED_SHORT_4_4_4_4:
	case GL_UNSIGNED_SHORT_5_5_5_1:
		break;
	}

	dgl.glTexSubImage2D(target, level, xoffset, yoffset, width, height,
	                 format, type, pixels);

	{
		GLenum err = dgl.glGetError();
		if (err)
			ERROR(err);
	}

	LEAVE();
}

static int is_valid_single_valued_texparam(GLenum pname)
{
	switch (pname)
	{
	default:
		return 0;
	case GL_TEXTURE_MIN_FILTER:
	case GL_TEXTURE_MAG_FILTER:
	case GL_TEXTURE_WRAP_S:
	case GL_TEXTURE_WRAP_T:
	case GL_GENERATE_MIPMAP:
		return 1;
	}
}

void GL_APIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (!is_valid_single_valued_texparam(pname))
		ERROR(GL_INVALID_ENUM);

	dgl.glTexParameterf(target, pname, param);

	LEAVE();
}

void GL_APIENTRY glTexParameterx (GLenum target, GLenum pname, GLfixed param)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (!is_valid_single_valued_texparam(pname))
		ERROR(GL_INVALID_ENUM);

	dgl.glTexParameteri(target, pname, param);

	LEAVE();
}

void GL_APIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (!is_valid_single_valued_texparam(pname))
		ERROR(GL_INVALID_ENUM);

	dgl.glTexParameteri(target, pname, param);

	LEAVE();
}

void GL_APIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint *params)
{
	glTexParameteri(target, pname, params[0]);
}

void GL_APIENTRY glTexParameterxv (GLenum target, GLenum pname, const GLfixed *params)
{
	glTexParameterx(target, pname, params[0]);
}

void GL_APIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
	glTexParameterf(target, pname, params[0]);
}

#if 0
static int is_valid_single_valued_env_mode(GLenum param)
{
	switch (param)
	{
	default:
		return 0;
	case GL_REPLACE:
	case GL_MODULATE:
	case GL_DECAL:
	case GL_BLEND:
	case GL_ADD:
		return 1;
	}
}
#endif

void GL_APIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
	ENTER();

	if (target == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glTexEnvf(target, pname, param);

	FORWARD_ERROR();
	LEAVE();

#if 0
	if (pname != GL_TEXTURE_ENV_MODE)
		ERROR(GL_INVALID_ENUM);

	if (target == GL_TEXTURE_ENV)
	{
		if (is_valid_single_valued_env_mode(param) == 0)
			ERROR(GL_INVALID_ENUM);

		dgl.glTexEnvf(GL_TEXTURE_ENV, pname, param);
	}
	else if (target == GL_POINT_SPRITE_OES)
	{
		if (pname == GL_COORD_REPLACE_OES)
			dgl.glTexEnvf(GL_POINT_SPRITE_ARB, pname, GL_COORD_REPLACE_ARB);
		else if (is_valid_single_valued_env_mode(param) == 0)
			ERROR(GL_INVALID_ENUM);
		else
			dgl.glTexEnvf(GL_POINT_SPRITE_ARB, pname, param);
	}
	else
		ERROR(GL_INVALID_ENUM);

	LEAVE();
#endif
}

void GL_APIENTRY glTexEnvx (GLenum target, GLenum pname, GLfixed param)
{
	ENTER();

	if (target == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
		break;
	case GL_RGB_SCALE:
	case GL_ALPHA_SCALE:
		dgl.glTexEnvf(target, pname, X_TO_F(param));
		break;
	case GL_COORD_REPLACE_OES:
	case GL_TEXTURE_ENV_MODE:
	case GL_COMBINE_RGB:
	case GL_COMBINE_ALPHA:
	case GL_SRC0_RGB:
	case GL_SRC0_ALPHA:
	case GL_SRC1_RGB:
	case GL_SRC1_ALPHA:
	case GL_SRC2_RGB:
	case GL_SRC2_ALPHA:
	case GL_OPERAND0_RGB:
	case GL_OPERAND0_ALPHA:
	case GL_OPERAND1_RGB:
	case GL_OPERAND1_ALPHA:
	case GL_OPERAND2_RGB:
	case GL_OPERAND2_ALPHA:
		dgl.glTexEnvi(target, pname, param);
		break;
	}

	FORWARD_ERROR();
	LEAVE();
}

void GL_APIENTRY glTexEnvi (GLenum target, GLenum pname, GLint param)
{
	ENTER();

	if (target == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glTexEnvi(target, pname, param);

	FORWARD_ERROR();
	LEAVE();

#if 0
	if (pname != GL_TEXTURE_ENV_MODE)
		ERROR(GL_INVALID_ENUM);

	if (is_valid_single_valued_env_mode(param) == 0)
		ERROR(GL_INVALID_ENUM);

	if (target == GL_TEXTURE_ENV)
		dgl.glTexEnvi(GL_TEXTURE_ENV, pname, param);
	else if (target == GL_POINT_SPRITE_OES)
		dgl.glTexEnvi(GL_POINT_SPRITE_ARB, pname, param);
	else
		ERROR(GL_INVALID_ENUM);

	LEAVE();
#endif
}

void GL_APIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
	ENTER();

	if (target == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glTexEnvfv(target, pname, params);

	FORWARD_ERROR();
	LEAVE();
}

void GL_APIENTRY glTexEnvxv (GLenum target, GLenum pname, const GLfixed *params)
{
	float f[4];
	ENTER();

	if (target == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_TEXTURE_ENV_COLOR:
		f[0] = X_TO_F(params[0]);
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		f[3] = X_TO_F(params[3]);
		dgl.glTexEnvfv(target, pname, f);
		break;
	case GL_RGB_SCALE:
	case GL_ALPHA_SCALE:
		dgl.glTexEnvf(target, pname, X_TO_F(params[0]));
		break;
	case GL_COORD_REPLACE_OES:
	case GL_TEXTURE_ENV_MODE:
	case GL_COMBINE_RGB:
	case GL_COMBINE_ALPHA:
	case GL_SRC0_RGB:
	case GL_SRC0_ALPHA:
	case GL_SRC1_RGB:
	case GL_SRC1_ALPHA:
	case GL_SRC2_RGB:
	case GL_SRC2_ALPHA:
	case GL_OPERAND0_RGB:
	case GL_OPERAND0_ALPHA:
	case GL_OPERAND1_RGB:
	case GL_OPERAND1_ALPHA:
	case GL_OPERAND2_RGB:
	case GL_OPERAND2_ALPHA:
		dgl.glTexEnvi(target, pname, params[0]);
		break;
	}

	LEAVE();
}

void GL_APIENTRY glTexEnviv (GLenum target, GLenum pname, const GLint *params)
{
	ENTER();

	if (target == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glTexEnviv(target, pname, params);

	FORWARD_ERROR();
	LEAVE();
}

void GL_APIENTRY glGetTexEnvfv (GLenum env, GLenum pname, GLfloat *params)
{
	ENTER();

	if (env == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glGetTexEnvfv(env, pname, params);

	FORWARD_ERROR();
	LEAVE();
}

void GL_APIENTRY glGetTexEnviv (GLenum env, GLenum pname, GLint *params)
{
	ENTER();

	if (env == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glGetTexEnviv(env, pname, params);

	FORWARD_ERROR();
	LEAVE();
}

void GL_APIENTRY glGetTexEnvxv (GLenum env, GLenum pname, GLfixed *params)
{
	float f[4];
	ENTER();

	if (env == GL_POINT_SPRITE_OES && state.arb_point_sprite == 0)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_TEXTURE_ENV_COLOR:
		dgl.glGetTexEnvfv(env, pname, f);
		params[0] = F_TO_X(f[0]);
		params[1] = F_TO_X(f[1]);
		params[2] = F_TO_X(f[2]);
		params[3] = F_TO_X(f[3]);
		break;
	case GL_RGB_SCALE:
	case GL_ALPHA_SCALE:
		dgl.glGetTexEnvfv(env, pname, f);
		params[0] = F_TO_X(f[0]);
		break;
	case GL_COMBINE_RGB:
	case GL_COMBINE_ALPHA:
	case GL_COORD_REPLACE_OES:
	case GL_TEXTURE_ENV_MODE:
	case GL_SRC0_RGB:
	case GL_SRC0_ALPHA:
	case GL_SRC1_RGB:
	case GL_SRC1_ALPHA:
	case GL_SRC2_RGB:
	case GL_SRC2_ALPHA:
	case GL_OPERAND0_RGB:
	case GL_OPERAND0_ALPHA:
	case GL_OPERAND1_RGB:
	case GL_OPERAND1_ALPHA:
	case GL_OPERAND2_RGB:
	case GL_OPERAND2_ALPHA:
		{
			GLint i;
			dgl.glGetTexEnviv(env, pname, &i);
			params[0] = (GLfixed) i;
		}
		break;
	}

	LEAVE();
}

void GL_APIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (is_valid_single_valued_texparam(pname) == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glGetTexParameterfv(target, pname, params);

	LEAVE();
}

void GL_APIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint *params)
{
	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (is_valid_single_valued_texparam(pname) == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glGetTexParameteriv(target, pname, params);

	LEAVE();
}

void GL_APIENTRY glGetTexParameterxv (GLenum target, GLenum pname, GLfixed *params)
{
	float f;

	ENTER();

	if (target != GL_TEXTURE_2D)
		ERROR(GL_INVALID_ENUM);

	if (is_valid_single_valued_texparam(pname) == 0)
		ERROR(GL_INVALID_ENUM);

	dgl.glGetTexParameterfv(target, pname, &f);

	params[0] = F_TO_X(f);

	LEAVE();
}

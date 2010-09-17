#include "api.h"

/*
 * Lights
 */

void GL_APIENTRY glLightModelf (GLenum pname, GLfloat param)
{
	ENTER();

	if (pname != GL_LIGHT_MODEL_TWO_SIDE)
		ERROR(GL_INVALID_ENUM);

	dgl.glLightModelf(pname, param);

	LEAVE();
}

void GL_APIENTRY glLightModelx (GLenum pname, GLfixed param)
{
	ENTER();

	if (pname != GL_LIGHT_MODEL_TWO_SIDE)
		ERROR(GL_INVALID_ENUM);

	dgl.glLightModelf(pname, X_TO_F(param));

	LEAVE();
}

void GL_APIENTRY glLightModelfv (GLenum pname, const GLfloat *params)
{
	ENTER();

	if (pname != GL_LIGHT_MODEL_TWO_SIDE && pname != GL_LIGHT_MODEL_AMBIENT)
		ERROR(GL_INVALID_ENUM);

	dgl.glLightModelfv(pname, params);

	LEAVE();
}

void GL_APIENTRY glLightModelxv (GLenum pname, const GLfixed *params)
{
	float f[4];
	ENTER();

	if (pname == GL_LIGHT_MODEL_TWO_SIDE)
	{
		f[0] = X_TO_F(params[0]);
	}
	else if (pname == GL_LIGHT_MODEL_AMBIENT)
	{
		f[0] = X_TO_F(params[0]);
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		f[3] = X_TO_F(params[3]);
	}
	else
		ERROR(GL_INVALID_ENUM);

	dgl.glLightModelfv(pname, f);

	LEAVE();
}

static GLenum check_single_valued_light_params_f(GLenum light, GLenum pname,
                                                 GLfloat param)
{
	if (light < GL_LIGHT0 || light > GL_LIGHT7)
		return GL_INVALID_ENUM;

	switch (pname)
	{
	default:
		return GL_INVALID_ENUM;
	case GL_SPOT_EXPONENT:
		if (param < 0.0f || param > 128.0f)
			return GL_INVALID_VALUE;
		break;
	case GL_SPOT_CUTOFF:
		if ((param < 0.0f || param > 90.0f) && param != 180.0f)
			return GL_INVALID_VALUE;
		break;
	case GL_CONSTANT_ATTENUATION:
	case GL_LINEAR_ATTENUATION:
	case GL_QUADRATIC_ATTENUATION:
		if (param < 0.0f)
			return GL_INVALID_VALUE;
		break;
	}

	return GL_NO_ERROR;
}

static GLenum check_single_valued_light_params_x(GLenum light, GLenum pname,
                                                 GLfixed param)
{
	if (light < GL_LIGHT0 || light > GL_LIGHT7)
		return GL_INVALID_ENUM;

	switch (pname)
	{
	default:
		return GL_INVALID_ENUM;
	case GL_SPOT_EXPONENT:
		if (param < I_TO_X(0) || param > I_TO_X(128))
			return GL_INVALID_VALUE;
		break;
	case GL_SPOT_CUTOFF:
		if ((param < I_TO_X(0) || param > I_TO_X(90)) && param != I_TO_X(180))
			return GL_INVALID_VALUE;
		break;
	case GL_CONSTANT_ATTENUATION:
	case GL_LINEAR_ATTENUATION:
	case GL_QUADRATIC_ATTENUATION:
		if (param < I_TO_X(0))
			return GL_INVALID_VALUE;
		break;
	}

	return GL_NO_ERROR;
}

void GL_APIENTRY glLightf (GLenum light, GLenum pname, GLfloat param)
{
	GLenum err;

	ENTER();

	err = check_single_valued_light_params_f(light, pname, param);
	if (err != GL_NO_ERROR)
		ERROR(err);

	dgl.glLightf(light, pname, param);

	LEAVE();
}

void GL_APIENTRY glLightx (GLenum light, GLenum pname, GLfixed param)
{
	GLenum err;

	ENTER();

	err = check_single_valued_light_params_x(light, pname, param);
	if (err != GL_NO_ERROR)
		ERROR(err);

	dgl.glLightf(light, pname, X_TO_F(param));

	LEAVE();
}

void GL_APIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params)
{
	ENTER();

	if (light < GL_LIGHT0 || light > GL_LIGHT7)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		{
			GLenum err;
			err = check_single_valued_light_params_f(light, pname, params[0]);
			if (err != GL_NO_ERROR)
				ERROR(err);
		}
		break;
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_POSITION:
	case GL_SPOT_DIRECTION:
		break;
	}

	dgl.glLightfv(light, pname, params);

	LEAVE();
}

void GL_APIENTRY glLightxv (GLenum light, GLenum pname, const GLfixed *params)
{
	GLfloat f[4];

	ENTER();

	if (light < GL_LIGHT0 || light > GL_LIGHT7)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		{
			GLenum err;
			err = check_single_valued_light_params_x(light, pname, params[0]);
			if (err != GL_NO_ERROR)
				ERROR(err);

			f[0] = X_TO_F(params[0]);
		}
		break;
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_POSITION:
		f[0] = X_TO_F(params[0]);
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		f[3] = X_TO_F(params[3]);
		break;
	case GL_SPOT_DIRECTION:
		f[0] = X_TO_F(params[0]);
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		break;
	}

	dgl.glLightfv(light, pname, f);

	LEAVE();
}

void GL_APIENTRY glGetLightfv (GLenum light, GLenum pname, GLfloat *params)
{
	ENTER();

	if (light < GL_LIGHT0 || light > GL_LIGHT7)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_SPOT_EXPONENT:
	case GL_SPOT_CUTOFF:
	case GL_CONSTANT_ATTENUATION:
	case GL_LINEAR_ATTENUATION:
	case GL_QUADRATIC_ATTENUATION:
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_POSITION:
	case GL_SPOT_DIRECTION:
		break;
	}

	dgl.glGetLightfv(light, pname, params);

	LEAVE();
}

void GL_APIENTRY glGetLightxv (GLenum light, GLenum pname, GLfixed *params)
{
	float f[4];

	ENTER();

	if (light < GL_LIGHT0 || light > GL_LIGHT7)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_SPOT_EXPONENT:
	case GL_SPOT_CUTOFF:
	case GL_CONSTANT_ATTENUATION:
	case GL_LINEAR_ATTENUATION:
	case GL_QUADRATIC_ATTENUATION:
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_POSITION:
	case GL_SPOT_DIRECTION:
		break;
	}

	dgl.glGetLightfv(light, pname, f);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_POSITION:
		params[3] = F_TO_X(f[3]);
	case GL_SPOT_DIRECTION:
		params[2] = F_TO_X(f[2]);
		params[1] = F_TO_X(f[1]);
	case GL_SPOT_EXPONENT:
	case GL_SPOT_CUTOFF:
	case GL_CONSTANT_ATTENUATION:
	case GL_LINEAR_ATTENUATION:
	case GL_QUADRATIC_ATTENUATION:
		params[0] = F_TO_X(f[0]);
		break;
	}

	LEAVE();
}

#include "api.h"
#include <stdio.h>
#include <string.h>

/*
 * Material
 */

void GL_APIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param)
{
	ENTER();

	if (face != GL_FRONT_AND_BACK)
		ERROR(GL_INVALID_ENUM);

	if (pname != GL_SHININESS)
		ERROR(GL_INVALID_ENUM);

	if (param < 0.0f || param > 128.0f)
		ERROR(GL_INVALID_VALUE);

	dgl.glMaterialf(face, pname, param);

	LEAVE();
}

void GL_APIENTRY glMaterialx (GLenum face, GLenum pname, GLfixed param)
{
	ENTER();

	if (face != GL_FRONT_AND_BACK)
		ERROR(GL_INVALID_ENUM);

	if (pname != GL_SHININESS)
		ERROR(GL_INVALID_ENUM);

	if (param < 0 * 65536 || param > 128 * 65536)
		ERROR(GL_INVALID_VALUE);

	dgl.glMaterialf(face, pname, X_TO_F(param));

	LEAVE();
}

void GL_APIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params)
{
	ENTER();

	if (face != GL_FRONT_AND_BACK)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_EMISSION:
	case GL_SHININESS:
	case GL_AMBIENT_AND_DIFFUSE:
		break;
	}

	if (pname == GL_SHININESS &&
	    (params[0] < 0.0f || params[0] > 128.0f))
			ERROR(GL_INVALID_VALUE);

	if (pname == GL_AMBIENT)
		memcpy(state.ambient, params, sizeof(float[4]));
	else if (pname == GL_DIFFUSE)
		memcpy(state.diffuse, params, sizeof(float[4]));

	dgl.glMaterialfv(face, pname, params);

	LEAVE();
}

void GL_APIENTRY glMaterialxv (GLenum face, GLenum pname, const GLfixed *params)
{
	float f[4];

	ENTER();

	if (face != GL_FRONT_AND_BACK)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_SHININESS:
		if (params[0] < 0 * 65536 || params[0] > 128 * 65536)
			ERROR(GL_INVALID_VALUE);
		f[0] = X_TO_F(params[0]);
		break;
	case GL_AMBIENT:
	case GL_DIFFUSE:
	case GL_SPECULAR:
	case GL_EMISSION:
	case GL_AMBIENT_AND_DIFFUSE:
		f[0] = X_TO_F(params[0]);
		f[1] = X_TO_F(params[1]);
		f[2] = X_TO_F(params[2]);
		f[3] = X_TO_F(params[3]);
		break;
	}

	if (pname == GL_AMBIENT)
		memcpy(state.ambient, f, sizeof(float[4]));
	else if (pname == GL_DIFFUSE)
		memcpy(state.diffuse, f, sizeof(float[4]));

	dgl.glMaterialfv(face, pname, f);

	LEAVE();
}

void GL_APIENTRY glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params)
{
	ENTER();

	if (face != GL_FRONT && face != GL_BACK)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_AMBIENT:
		memcpy(params, state.ambient, sizeof(float[4]));
		break;
	case GL_DIFFUSE:
		memcpy(params, state.diffuse, sizeof(float[4]));
		break;
	case GL_SPECULAR:
	case GL_EMISSION:
	case GL_SHININESS:
		dgl.glGetMaterialfv(face, pname, params);
		break;
	}

	LEAVE();
}

void GL_APIENTRY glGetMaterialxv (GLenum face, GLenum pname, GLfixed *params)
{
	float f[4];

	ENTER();

	if (face != GL_FRONT && face != GL_BACK)
		ERROR(GL_INVALID_ENUM);

	switch (pname)
	{
	default:
		ERROR(GL_INVALID_ENUM);
	case GL_AMBIENT:
		memcpy(f, state.ambient, sizeof(float[4]));
		break;
	case GL_DIFFUSE:
		memcpy(f, state.diffuse, sizeof(float[4]));
		break;
	case GL_SPECULAR:
	case GL_EMISSION:
	case GL_SHININESS:
		dgl.glGetMaterialfv(face, pname, f);
		break;
	}

	if (pname == GL_SHININESS)
		params[0] = F_TO_X(f[0]);
	else
	{
		params[0] = F_TO_X(f[0]);
		params[1] = F_TO_X(f[1]);
		params[2] = F_TO_X(f[2]);
		params[3] = F_TO_X(f[3]);
	}

	LEAVE();
}

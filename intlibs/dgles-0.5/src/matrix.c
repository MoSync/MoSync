#include "api.h"

/*
 * Matrices
 */

void GL_APIENTRY glLoadIdentity (void)
{
	ENTER();
	dgl.glLoadIdentity();
	LEAVE();
}

void GL_APIENTRY glLoadMatrixf (const GLfloat *m)
{
	ENTER();
	dgl.glLoadMatrixf(m);
	LEAVE();
}

void GL_APIENTRY glLoadMatrixx (const GLfixed *m)
{
	int i;
	double a[16];

	ENTER();

	for (i = 0; i < 16; i++)
		a[i] = X_TO_D(m[i]);

	dgl.glLoadMatrixd(a);

	LEAVE();
}

void GL_APIENTRY glMatrixMode (GLenum mode)
{
	ENTER();

	switch (mode)
	{
	case GL_MODELVIEW:
	case GL_PROJECTION:
	case GL_TEXTURE:
		dgl.glMatrixMode(mode);
		break;

	default:
		ERROR(GL_INVALID_ENUM);
	}

	LEAVE();
}

void GL_APIENTRY glMultMatrixf (const GLfloat *m)
{
	ENTER();
	dgl.glMultMatrixf(m);
	LEAVE();
}

void GL_APIENTRY glMultMatrixx (const GLfixed *m)
{
	double a[16];
	int i;

	ENTER();

	for (i = 0; i < 16; i++)
		a[i] = X_TO_D(m[i]);

	dgl.glMultMatrixd(a);

	LEAVE();
}

void GL_APIENTRY glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	ENTER();

	if (zNear <= 0 || zFar <= 0 || left == right || bottom == top)
		ERROR(GL_INVALID_VALUE);

	dgl.glFrustum(left, right, bottom, top, zNear, zFar);

	LEAVE();
}

void GL_APIENTRY glFrustumx (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	ENTER();

	if (zNear <= 0 || zFar <= 0 || left == right || bottom == top)
		ERROR(GL_INVALID_VALUE);

	dgl.glFrustum(X_TO_D(left), X_TO_D(right), X_TO_D(bottom), X_TO_D(top),
			X_TO_D(zNear), X_TO_D(zFar));

	LEAVE();
}

void GL_APIENTRY glOrthof (GLfloat left, GLfloat right, GLfloat bottom,
                           GLfloat top, GLfloat zNear, GLfloat zFar)
{
	ENTER();
	dgl.glOrtho(F_TO_D(left), F_TO_D(right), F_TO_D(bottom),
	            F_TO_D(top), F_TO_D(zNear), F_TO_D(zFar));
	LEAVE();
}

void GL_APIENTRY glOrthox (GLfixed left, GLfixed right, GLfixed bottom,
                           GLfixed top, GLfixed zNear, GLfixed zFar)
{
	ENTER();
	dgl.glOrtho(X_TO_D(left), X_TO_D(right), X_TO_D(bottom),
	            X_TO_D(top), X_TO_D(zNear), X_TO_D(zFar));
	LEAVE();
}

void GL_APIENTRY glPopMatrix (void)
{
	ENTER();

	dgl.glPopMatrix();

	/* Check for stack underflow */
	{
		GLenum err = dgl.glGetError();
		assert(err == GL_NO_ERROR || err == GL_STACK_UNDERFLOW);

		if (err == GL_STACK_UNDERFLOW)
			state.error = GL_STACK_UNDERFLOW;
	}

	LEAVE();
}

void GL_APIENTRY glPushMatrix (void)
{
	ENTER();

	dgl.glPushMatrix();

	{
		GLenum err = dgl.glGetError();
		assert(err == GL_NO_ERROR || err == GL_STACK_OVERFLOW);

		if (err == GL_STACK_OVERFLOW)
			state.error = GL_STACK_OVERFLOW;
	}

	LEAVE();
}

void GL_APIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	ENTER();
	dgl.glRotatef(angle, x, y, z);
	LEAVE();
}

void GL_APIENTRY glRotatex (GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	ENTER();
	dgl.glRotated(X_TO_D(angle), X_TO_D(x), X_TO_D(y), X_TO_D(z));
	LEAVE();
}

void GL_APIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z)
{
	ENTER();
	dgl.glScalef(x, y, z);
	LEAVE();
}

void GL_APIENTRY glScalex (GLfixed x, GLfixed y, GLfixed z)
{
	ENTER();
	dgl.glScaled(X_TO_D(x), X_TO_D(y), X_TO_D(z));
	LEAVE();
}

void GL_APIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
	ENTER();
	dgl.glTranslatef(x, y, z);
	LEAVE();
}

void GL_APIENTRY glTranslatex (GLfixed x, GLfixed y, GLfixed z)
{
	ENTER();
	dgl.glTranslated(X_TO_D(x), X_TO_D(y), X_TO_D(z));
	LEAVE();
}

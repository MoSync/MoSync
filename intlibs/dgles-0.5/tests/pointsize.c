#include <GLES/gl.h>
#include <assert.h>

void frame_done();

#define FNUM(a, b) ((a) * 65536 | (b))

static GLfixed vertices[2*8] =
{
	FNUM(-4, 0), FNUM(0, 0),
	FNUM(-3, 0), FNUM(0, 0),
	FNUM(-2, 0), FNUM(0, 0),
	FNUM(-1, 0), FNUM(0, 0),
	FNUM(0, 0), FNUM(0, 0),
	FNUM(1, 0), FNUM(0, 0),
	FNUM(2, 0), FNUM(0, 0),
	FNUM(3, 0), FNUM(0, 0)
};

#if 0
static GLfixed texcoords[2*8] =
{
	FNUM(0, 0), FNUM(0, 0),
	FNUM(0, 8192), FNUM(0, 0),
	FNUM(0, 16384), FNUM(0, 0),
	FNUM(0, 24576), FNUM(0, 0),
	FNUM(0, 32768), FNUM(0, 0),
	FNUM(0, 40960), FNUM(0, 0),
	FNUM(0, 49152), FNUM(0, 0),
	FNUM(0, 57344), FNUM(0, 0)
};
#endif

static GLfixed point_sizes[8] =
{
	FNUM(2, 0),
	FNUM(3, 0),
	FNUM(4, 0),
	FNUM(5, 0),
	FNUM(6, 0),
	FNUM(7, 0),
	FNUM(8, 0),
	FNUM(9, 0)
};

#if 0
static unsigned char texture[3*8] =
{
	0,0,0,
	255,0,0,
	0,255,0,
	0,0,255,
	255,255,0,
	0,255,255,
	255,0,255,
	255,255,255,
};
#endif

int test()
{
#if 0
	GLuint tex;
#endif
	int f = 0;

	glVertexPointer(2, GL_FIXED, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

#if 0
	glTexCoordPointer(2, GL_FIXED, 0, texcoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

#if 1
	glPointSizePointerOES(GL_FIXED, 0, point_sizes);
	glEnableClientState(GL_POINT_SIZE_ARRAY_OES);
#endif

#if 0
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 1, 0, GL_RGB,
	             GL_UNSIGNED_BYTE, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
#endif

	assert(glGetError() == GL_NO_ERROR);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthox(FNUM(-6,0), FNUM(6,0), FNUM(-6,0), FNUM(6,0),
	         FNUM(-1,0), FNUM(1,0));

	glEnable(GL_COLOR_MATERIAL);

	glPointSize(5);

	assert(glGetError() == GL_NO_ERROR);

	while (1)
	{
		glClearColorx(22313, 8212, 4423, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glScalef(1 / 8.0, 1 / 8.0, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatex(f * 65536 / 32, 0, 0, 1 * 65536);

		assert(glGetError() == GL_NO_ERROR);

		glColor4x(0, 0, 65536, 65536);

		assert(glGetError() == GL_NO_ERROR);

		glDrawArrays(GL_POINTS, 0, 8);

		assert(glGetError() == GL_NO_ERROR);

		frame_done();
		f++;
	}

	return 0;
}

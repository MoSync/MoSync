#include <GLES/gl.h>
#include <assert.h>
#include <stdio.h>

void frame_done();

static const signed char vertices[] =
{
	-1, -1,  1,
	-1,  1,  1,
	 1,  1,  1,
	 1, -1,  1,
	-1, -1, -1,
	-1,  1, -1,
	 1,  1, -1,
	 1, -1, -1
};

static const float texcoords[] =
{
	0, 0,
	0, 1,
	1, 1,
	1, 0,
	0, 0,
	0, 1,
	1, 1,
	1, 0,
#if 0
	0, 0,
	0, 127,
	0, 0,
	0, 127,
	127, 0,
	127, 127,
	127, 0,
	127, 127
#endif
};

static const unsigned char triangles[] =
{
	0, 2, 1,
	0, 7, 3,
	1, 4, 0,
	2, 5, 1,
	3, 2, 0,
	3, 6, 2,
	4, 6, 7,
	4, 7, 0,
	5, 4, 1,
	5, 6, 4,
	6, 5, 2,
	7, 6, 3
};

static const unsigned char colors[] =
{
	255, 255, 128, 255,
	255, 128, 255, 255,
	128, 255, 255, 255,
	255, 128,   0, 255,
	255, 255, 128, 255,
	255, 128, 255, 255,
	128, 255, 255, 255,
	255,   0, 255, 255,
};

static const signed char normals[] =
{
	-128, -128,  127,
	-128,  127,  127,
	 127,  127,  127,
	 127, -128,  127,
	-128, -128, -128,
	-128,  127, -128,
	 127,  127, -128,
	 127, -128, -128
};

static const unsigned char texture[] =
{
	/* Palette (4 bit, rgba8) */
	  0,  0,  0,  0,   32, 16, 16, 16,
	 32, 64, 32, 32,   48, 48, 48, 48,
	 64, 64,128, 64,   80, 96, 80, 80,
	 96,128, 96, 96,  176,112,112,112,
	128,128,128,128,  144,144,144,144,
	160,176,160,160,  176,176,192,176,
	192,192,192,192,  208,208,208,208,
	240,224,224,224,  240,255,240,240,
#if 0
	/* Palette (4 bit, rgba4) */
#define C(r, g, b, a) ((r) << 4) | (g), ((b) << 4) | (a)
	C(0, 0, 0, 15), C(15, 0, 0, 15),
	C(0, 15, 0, 15), C(0, 0, 15, 15),
	C(0, 15, 15, 15), C(0, 15, 0, 15),
	C(0, 0, 0, 15), C(8, 8, 8, 15),
	C(0, 12, 0, 15), C(0, 0, 0, 15),
	C(0, 0, 0, 15), C(7, 0, 0, 15),
	C(0, 0, 9, 15), C(0, 0, 0, 15),
	C(0, 0, 0, 15), C(15, 15, 15, 15),
#undef C
#endif
	0x01, 0x23, 0x45, 0x67,
	0x12, 0x34, 0x56, 0x78,
	0x23, 0x45, 0x67, 0x89,
	0x34, 0x56, 0x78, 0x9A,
	0x45, 0x67, 0x89, 0xAB,
	0x56, 0x78, 0x9A, 0xBC,
	0x67, 0x89, 0xAB, 0xCD,
	0x78, 0x9A, 0xBC, 0xDE,
};

#if 0
static unsigned char texture2[] =
{
	64,64,64,  0,0,0,
	0,0,0,  64,64,64,
};
#else
static unsigned char texture2a[] =
{
	64,64,64,255,  0,0,0,255,
	0,0,0,255,  64,64,64,255,
};
#endif

int test()
{
	static const GLfixed light_position[] = { 0, 4 * 65536, 0, 1 * 65536 };
	int f = 0;
	GLuint tex[2];

	/* Generate texture */

	glGenTextures(2, tex);

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_PALETTE4_RGBA8_OES, 8, 8, 0,
	                       sizeof(texture), texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
#if 0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
	             texture2);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	             texture2a);
#endif

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* Arrays */

	glVertexPointer(3, GL_BYTE, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

#if 0
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
#endif

	glNormalPointer(GL_BYTE, 0, normals);
	glEnableClientState(GL_NORMAL_ARRAY);

	/* Set up multitexturing */

	glClientActiveTexture(GL_TEXTURE0);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[0]);

	glClientActiveTexture(GL_TEXTURE1);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	/* Something else... */

	assert(glGetError() == GL_NO_ERROR);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumf(-1, 1, -1, 1, 1, 10);

	glLightxv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

#if 0
	{
		float a = -1;
		glPointSize(-44);
		glGetFloatv(GL_POINT_SIZE_MIN, &a);
		fprintf(stderr, "min %f\n", a);
		glGetFloatv(GL_POINT_SIZE_MAX, &a);
		fprintf(stderr, "max %f\n", a);
		glGetFloatv(GL_POINT_SIZE, &a);
		fprintf(stderr, "sisi %f\n", a);
		fprintf(stderr, "%d\n", glIsEnabled(GL_POINT_SMOOTH));
	}
#endif

#if 0
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
#endif

	assert(glGetError() == GL_NO_ERROR);

	
	while (1)
	{
		glClearColor(0.2f, 0.3f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_TEXTURE);

		glActiveTexture(GL_TEXTURE0);
		glLoadIdentity();
		glScalef(5, 5, 1);

		glActiveTexture(GL_TEXTURE1);
		glLoadIdentity();
		glScalef(3, 3, 1);
		glRotatef(f / 15.0f, 0, 0, 1);
		glTranslatef(-0.5f, -0.5f, 0);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();

		glTranslatef(0, 0, -5);
		glRotatef(f / 8.0f, 0.7f, 1, 0);
		glRotatef(f / 9.0f, 0.8f, 0, 0.2f);

		glColor4f(1, 1, 1, 1);
		glDrawElements(GL_TRIANGLES, sizeof(triangles) / sizeof(triangles[0]),
				GL_UNSIGNED_BYTE, triangles);

		assert(glGetError() == GL_NO_ERROR);

		frame_done();
		f++;
	}

	return 0;
}

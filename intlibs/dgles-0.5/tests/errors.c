#include <GLES/gl.h>
#include <stdio.h>

static const signed char vertices[3] = { 1, 2, 3 };

int test()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_BYTE, 3 * sizeof(char), vertices);
	fprintf(stderr, "ERR: %x\n", glGetError());
	return 0;
}

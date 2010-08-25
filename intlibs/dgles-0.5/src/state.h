#ifndef _STATE_H_
#define _STATE_H_

#include <GLES/gl.h>

#define VERTEX_ARRAY        1
#define NORMAL_ARRAY        2
#define COLOR_ARRAY         4
#define POINT_SIZE_ARRAY    8
#define TEXCOORD0_ARRAY    16
#define TEXCOORD1_ARRAY    32
#define TEXCOORD2_ARRAY    64
#define TEXCOORD3_ARRAY   128

#define MAX_TEXTURE_UNITS   2

/* OpenGL ES array / pointer */

struct DGLES_Array
{
	GLint         size;
	GLenum        type;
	GLsizei       stride;
	const GLvoid *pointer;

	/* real stride (different from stride if stride==0) */
	GLsizei       rstride;

	/* bound buffer object */
	struct DGLES_Buffer *buffer;
};

/* Ensure that buffer == NULL before using this macro */
#define ARRAY_PTR(ary, i) ((const char *) (ary)->pointer + (i)*(ary)->rstride)

#define ARRAY_BUFFER(ary) ((ary)->buffer ? (ary)->buffer->data : (ary)->pointer)

/* Buffer object */

struct DGLES_Buffer
{
	GLuint        name;

	GLsizeiptr    size;
	GLvoid       *data;
	GLenum        usage;

	struct DGLES_Buffer *next;
};

/* OpenGL ES state */

struct DGLES_State
{
	int inited;
	int library_loaded;
	int arb_point_sprite;

	/* Current error */
	GLenum error;

	/* Bitmask for enabled arrays */
	unsigned int array_flags;

	/* Client active texture */
	unsigned int active_texture;
	unsigned int max_texture_units;

	/* Vertex pointer */
	struct DGLES_Array vptr;

	/* Normal pointer */
	struct DGLES_Array nptr;

	/* Color */
	struct DGLES_Array cptr;

	/* Texcoord pointers */
	struct DGLES_Array tptr[MAX_TEXTURE_UNITS];

	/* Point size array */
	struct DGLES_Array psptr;

	/* Buffer objects */
	struct DGLES_Buffer *first_buffer;
	struct DGLES_Buffer *array_buffer;
	struct DGLES_Buffer *element_buffer;

	/* Ambient and diffuse */
	float ambient[4];
	float diffuse[4];
};

#endif

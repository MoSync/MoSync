#ifndef _API_H_
#define _API_H_

#include "dgl.h"
#include "state.h"
#include <assert.h>

extern struct DGLES_State _dgles_state;
extern struct DGL _dgles_dgl;

extern void _dgles_init();

#define state _dgles_state
#define dgl _dgles_dgl

#define X_TO_D(x) ((x) / 65536.0)
#define X_TO_F(x) ((x) / 65536.0f)
#define D_TO_X(d) ((d) > 32767.65535 ? 32767 * 65536 + 65535 : \
                   (d) < -32768.65535 ? -32768 * 65536 + 65535 : \
                   ((GLfixed) ((d) * 65536)))
#define F_TO_X(f) D_TO_X(f)
#define F_TO_D(f) ((double) (f))
#define I_TO_X(i) ((GLfixed) ((i) * 65536))
#define X_TO_I(x) ((x) / 65536)

/* TODO: this isn't really the right way, -1.0 should become INT_MIN */
#define CLAMPED_TO_I(c) ((int) ((c) * INT_MAX))

/* This macro is called in the beginning of every API function */
#define ENTER() do { _dgles_init(); } while(0)
#if 0
#include <stdio.h>
#define ENTER() do { _dgles_init(); fprintf(stderr, "entering %s\n", __func__); } while(0)
#endif

/* These macros are used to leave from functions */
#define ERROR(e) do { state.error = (e); return; } while(0)
#define ERROR2(e, v) do { state.error = (e); return (v); } while(0)

#if 0
#include <stdio.h>
#undef ERROR
#define ERROR(e) do { fprintf(stderr, "ERROR %s %d %x !!\n", __FILE__, __LINE__, e); state.error = (e); return; } while(0)
#endif

#define LEAVE() \
	do { \
		GLenum err = dgl.glGetError(); \
		if (err == GL_OUT_OF_MEMORY) \
			state.error = GL_OUT_OF_MEMORY; \
		else \
			assert(err == GL_NO_ERROR); \
		return; \
	} while(0)

#define LEAVE2(v) \
	do { \
		{ \
			GLenum err = dgl.glGetError(); \
			if (err == GL_OUT_OF_MEMORY) \
				state.error = GL_OUT_OF_MEMORY; \
			else \
				assert(err == GL_NO_ERROR); \
		} \
		return (v); \
	} while(0)

/* If there is an error in desktop OpenGL, forward it */

#define FORWARD_ERROR() \
	do { \
		GLenum err = dgl.glGetError(); \
		if (err != GL_NO_ERROR) \
			ERROR(err); \
	} while(0)

#define TODO() do { assert(!"TODO!"); } while(0)

#endif

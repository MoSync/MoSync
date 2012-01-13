// this file contains fixes for some of the opengl es bindings that couldn't
// be wrapped programatically.

// the wrapper generator can't yet handle a few set of functions
// in the opengles 2.0 api (so we manually override them).

// override implementations for broken bindings..
#undef maIOCtl_glGetPointerv_case
#define maIOCtl_glGetPointerv_case(func) \
case maIOCtl_glGetPointerv: \
{\
GLenum _pname = (GLuint)a; \
void* _pointer = GVMR(b, MAAddress);\
wrap_glGetPointerv(_pname, _pointer); \
return 0; \
}

#undef maIOCtl_glGetVertexAttribPointerv_case
#define maIOCtl_glGetVertexAttribPointerv_case(func) \
case maIOCtl_glGetVertexAttribPointerv: \
{\
GLuint _index = (GLuint)a; \
GLenum _pname = (GLuint)b; \
void* _pointer = GVMR(c, MAAddress);\
wrap_glGetVertexAttribPointerv(_index, _pname, _pointer); \
return 0; \
}

#undef maIOCtl_glShaderSource_case
#define maIOCtl_glShaderSource_case(func) \
case maIOCtl_glShaderSource: \
{ \
GLuint _shader = (GLuint)a; \
GLsizei _count = (GLsizei)b; \
void* _string = GVMR(c, MAAddress); \
const GLint* _length = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), GLint); \
wrap_glShaderSource(_shader, _count, _string, _length); \
return 0; \
} \

// these are a bit tricky. They get different semantics when a vbo is bound (the pointer is actually an offset)
#undef maIOCtl_glVertexAttribPointer_case
#define maIOCtl_glVertexAttribPointer_case(func) \
case maIOCtl_glVertexAttribPointer: \
{ \
GLuint _indx = (GLuint)a; \
GLint _size = (GLint)b; \
GLenum _type = (GLenum)c; \
GLboolean _normalized = (GLboolean)SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE); \
GLsizei _stride = (GLsizei)SYSCALL_THIS->GetValidatedStackValue(4 VSV_ARGPTR_USE); \
const void* _ptr = GVMR(SYSCALL_THIS->GetValidatedStackValue(8 VSV_ARGPTR_USE), MAAddress); \
wrap_glVertexAttribPointer(_indx, _size, _type, _normalized, _stride, _ptr); \
return 0; \
} \

#undef maIOCtl_glDrawElements_case
#define maIOCtl_glDrawElements_case(func) \
case maIOCtl_glDrawElements: \
{ \
GLenum _mode = (GLenum)a; \
GLsizei _count = (GLsizei)b; \
GLenum _type = (GLenum)c; \
const void* _indices = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), MAAddress); \
wrap_glDrawElements(_mode, _count, _type, _indices); \
return 0; \
} \

#undef maIOCtl_glVertexPointer_case
#define maIOCtl_glVertexPointer_case(func) \
case maIOCtl_glVertexPointer: \
{ \
GLint _size = (GLint)a; \
GLenum _type = (GLenum)b; \
GLsizei _stride = (GLsizei)c; \
const void* _pointer = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), MAAddress); \
wrap_glVertexPointer(_size, _type, _stride, _pointer); \
return 0; \
} \

#undef maIOCtl_glTexCoordPointer_case
#define maIOCtl_glTexCoordPointer_case(func) \
case maIOCtl_glTexCoordPointer: \
{ \
GLint _size = (GLint)a; \
GLenum _type = (GLenum)b; \
GLsizei _stride = (GLsizei)c; \
const void* _pointer = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), MAAddress); \
wrap_glTexCoordPointer(_size, _type, _stride, _pointer); \
return 0; \
} \

#undef maIOCtl_glColorPointer_case
#define maIOCtl_glColorPointer_case(func) \
case maIOCtl_glColorPointer: \
{ \
GLint _size = (GLint)a; \
GLenum _type = (GLenum)b; \
GLsizei _stride = (GLsizei)c; \
const void* _pointer = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), MAAddress); \
wrap_glColorPointer(_size, _type, _stride, _pointer); \
return 0; \
} \

#undef maIOCtl_glNormalPointer_case
#define maIOCtl_glNormalPointer_case(func) \
case maIOCtl_glNormalPointer: \
{ \
GLenum _type = (GLenum)a; \
GLsizei _stride = (GLsizei)b; \
const void* _pointer = GVMR(c, MAAddress); \
wrap_glNormalPointer(_type, _stride, _pointer); \
return 0; \
} \

	const GLvoid* getArrayBufferBindingOffset(const GLvoid *pointer)
	{
		GLint ret;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &ret);
		if(ret != 0)
		{
			if(pointer != NULL)
				pointer = (const void*)SYSCALL_THIS->TranslateNativePointerToMoSyncPointer((void*)pointer);
		}
		return pointer;
	}

	void wrap_glNormalPointer(GLenum type, GLsizei stride, const GLvoid * pointer)
	{

		glNormalPointer(type, stride, getArrayBufferBindingOffset(pointer));
	}

	void wrap_glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
	{
		glColorPointer(size, type, stride, getArrayBufferBindingOffset(pointer));
	}

	void wrap_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
	{
		glTexCoordPointer(size, type, stride, getArrayBufferBindingOffset(pointer));
	}

	void wrap_glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
	{
		glVertexPointer(size, type, stride, getArrayBufferBindingOffset(pointer));
	}

	void wrap_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indecies) {
		GLint ret;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ret);
		if(ret != 0)
		{
			if(indecies != NULL)
				indecies = (const void*)SYSCALL_THIS->TranslateNativePointerToMoSyncPointer((void*)indecies);
		}

		glDrawElements(mode, count, type, indecies);
	}

	void wrap_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
	{
#ifndef _android_1
		glVertexAttribPointer(index, size, type, normalized, stride, getArrayBufferBindingOffset(ptr));
#endif
	}

    void wrap_glShaderSource(GLuint shader, GLsizei count, void* strings, const GLint* length) {

        int* stringsArray = (int*)strings;
        const GLchar** strCopies = new const GLchar*[count];

        for(int i = 0; i < count; i++) {
            void* src = GVMR(stringsArray[i], MAAddress);
            strCopies[i] = (GLchar*)src;
        }
#ifndef _android_1
        glShaderSource(shader, count, strCopies, length);
#endif
        delete strCopies;
    }

    void wrap_glGetVertexAttribPointerv(GLuint index, GLenum pname, void* pointer) {
        GLvoid* outPointer;
#ifndef _android_1
        glGetVertexAttribPointerv(index, pname, &outPointer);

        if(pname != GL_VERTEX_ATTRIB_ARRAY_POINTER)
            return;
#endif
        *(int*)pointer = gSyscall->TranslateNativePointerToMoSyncPointer(outPointer);
    }

    void wrap_glGetPointerv(GLenum pname, void* pointer) {
        GLvoid* outPointer;
        glGetPointerv(pname, &outPointer);

        if(pname != GL_COLOR_ARRAY_POINTER &&
           pname != GL_NORMAL_ARRAY_POINTER &&
           pname != GL_POINT_SIZE_ARRAY_POINTER_OES &&
           pname != GL_TEXTURE_COORD_ARRAY_POINTER &&
           pname != GL_VERTEX_ARRAY_POINTER)
            return;

        *(int*)pointer = gSyscall->TranslateNativePointerToMoSyncPointer(outPointer);
    }

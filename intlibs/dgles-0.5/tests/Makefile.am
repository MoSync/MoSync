noinst_PROGRAMS = simple errors pointsize

simple_SOURCES = sdlstub.c simple.c
simple_LDADD = ../src/libGLES_CM.a

errors_SOURCES = sdlstub.c errors.c
errors_LDADD = ../src/libGLES_CM.a

pointsize_SOURCES = sdlstub.c pointsize.c
pointsize_LDADD = ../src/libGLES_CM.a

AM_CPPFLAGS = @SDL_CXXFLAGS@ -I..
AM_LDFLAGS = @SDL_LIBS@

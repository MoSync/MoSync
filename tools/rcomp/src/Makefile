#
# Makefile
#

GCC_VERSION=-2.95
GCC_VERSION=
CFLAGS+=-Wall -gstabs+ -Wno-deprecated
CFLAGS+=-I../inc -Iderived
LFLAGS=
DEPS=\
	../inc/array.h \
	../inc/astring.h \
	../inc/ctable.h \
	../inc/datatype.h \
	../inc/errorhan.h \
	../inc/fileacc.h \
	../inc/fileline.h \
	../inc/indextab.h \
	../inc/linkarra.h \
	../inc/linklist.h \
	../inc/main.h \
	../inc/mem.h \
	../inc/nameidma.h \
	../inc/numval.h \
	../inc/rcbinstr.h \
	../inc/rcscan.h \
	../inc/rcstack.h \
	../inc/resource.h \
	../inc/stack.h \
	../inc/stringar.h \
	../inc/structst.h \
	../inc/tokens.h \
	../inc/unicode_compressor.h \
	../inc/version.h \

#	fileacc.o \

OBJS=\
	array.o \
	astring.o \
	ccodes.o \
	ctable.o \
	datatype.o \
	derived/rcomp.o \
	errorhan.o \
	fileline.o \
	indextab.o \
	linkarra.o \
	linklist.o \
	main.o \
	mem.o \
	nameidma.o \
	numval.o \
	rcbinstr.o \
	rcompl_wrap.o \
	rcostrm.o \
	rcstack.o \
	resource.o \
	stack.o \
	stringar.o \
	structst.o \
	unicode_compressor.o \

#	rcscan.o \

BIN=rcomp

all: $(BIN)

$(BIN): $(OBJS) Makefile
	g++$(GCC_VERSION) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.cpp $(DEPS)
	g++$(GCC_VERSION) $(CFLAGS) -c $< -o $@

%.o: %.c
	gcc$(GCC_VERSION) $(CFLAGS) -c $< -o $@

install: $(BIN)
	install -m 755 $(BIN) $(DESTDIR)/usr/bin

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(BIN)

clean:
	rm -f $(OBJS) $(BIN) core

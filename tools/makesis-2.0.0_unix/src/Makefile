#
# Makefile
#

#GCC_VERSION=-2.95
CFLAGS+=-Wall -gstabs+
CFLAGS+=-I../include
#CFLAGS+=-D_UNICODE -DUNICODE
LFLAGS=
LIBS=-lz
OBJS=\
	crtsis.o \
	makesis.o \
	parsecmd.o \
	parsepkg.o \
	utf8.o \
	utils.o \
	stub.o \
	wcstub.o

BIN=makesis

all: $(BIN)

$(BIN): $(OBJS) Makefile
	g++$(GCC_VERSION) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.cpp crtsis.h makesis.h parsecmd.h parsepkg.h utf8.h utils.h \
	../include/windows.h ../include/tchar.h
	g++$(GCC_VERSION) $(CFLAGS) -c $< -o $@

%.o: %.c
	gcc$(GCC_VERSION) $(CFLAGS) -c $< -o $@

install: $(BIN)
	install -m 755 $(BIN) $(DESTDIR)/usr/bin

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(BIN)

clean:
	rm -f *.o $(BIN) core

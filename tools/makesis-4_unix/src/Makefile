#
# Makefile
#

#GCC_VERSION=-2.95
CFLAGS+=-Wall -gstabs+ -m32
CFLAGS+=-I../include
#CFLAGS+=-D_UNICODE -DUNICODE
CFLAGS+=-DTEST -ggdb
LDFLAGS=-m32
LIBS=-lz -lcrypto
OBJS=\
	crtsis.o \
	makesis.o \
	parsecmd.o \
	parsepkg.o \
	utf8.o \
	utils.o \
	stub.o \
	wcstub.o \
	signutils.o \
	sisfilegen.o \
	crc.o \
	sisfield.o

BIN=makesis signsis makekeys
DEVTOOLS=finddatetime dumpcontroller dumptree

all: $(BIN)

devtools: finddatetime dumpcontroller dumptree

makesis: $(OBJS) Makefile
	g++$(GCC_VERSION) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

signsis: signsis.o crc.o signutils.o sisfield.o
	g++$(GCC_VERSION) $(LDFLAGS) $+ -o $@ $(LIBS)

finddatetime: finddatetime.o sisfield.o
	g++$(GCC_VERSION) -o $@ $+ $(LDFLAGS) $(LIBS)

dumpcontroller: dumpcontroller.o sisfield.o
	g++$(GCC_VERSION) -o $@ $+ $(LDFLAGS) $(LIBS)

dumptree: dumptree.o sisfield.o
	g++$(GCC_VERSION) -o $@ $+ $(LDFLAGS) $(LIBS)

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
	rm -f *.o $(BIN) $(DEVTOOLS) core

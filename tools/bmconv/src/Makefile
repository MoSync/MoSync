#
# Makefile
#

#
# Modify this to suit your platform specific needs.
#
#GCC_VERSION=-2.95
#GCC_VERSION=-3.0
#GCC_VERSION=2	# For Darwin use gcc 2.95.2 - gcc 3.1 is b0rken !
#CC=/usr/local/encap/gcc-3.1/bin/g++  # Solaris
CC=g++$(GCC_VERSION)
CFLAGS=-O2 -Wall -Wno-deprecated -gstabs+ -I../include
LFLAGS=
DEPS=bmconv.h rgb.h
OBJS=\
	bmtopbm.o \
	pbmcomp.o \
	rgb.o \
	utils.o \
	mainfunc.o \
	pbmtobm.o

BIN=bmconv

all: $(BIN)

$(BIN): $(OBJS) Makefile
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

%.o:	%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(BIN)
	install -m 755 $(BIN) $(DESTDIR)/usr/bin

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(BIN)

clean:
	rm -f *.o $(BIN) core

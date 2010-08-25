#
# Set the path to the top of the source tree
PROJECT_DIR = ../../..

#
# Do not change this
include $(PROJECT_DIR)/build/$(CUSTOM_OS)Makefile.incl

#
# List all subdirectories to do recursive make in
SUBDIRS = 

#
# Add include paths etc here
TOMCRYPT_HEADERS = -I$(PROJECT_DIR)/libtomcrypt/src/headers
CFLAGS += $(TOMCRYPT_HEADERS) $(CFLAG_SHADOW) $(CFLAG_NO_UNUSED_PARAMETER) \
          -DLTM_DESC -DLTC_SOURCE $(CFLAG_UNROLL_LOOPS)

#
# Add extra link paths here
LDFLAGS += 

#
# Add libs that are compiled as part of the project here
APPLIBS += 

#
# Add external libs here
LIBS += 

#
# Set the name of the archive target of this makefile
LIBTARGET = $(OUT_DIR)/libprngs.a

#
# Set the name of the executable target of this makefile
TARGET = 

#
# Set all source files here
SRCS = fortuna.c rng_get_bytes.c rng_make_prng.c

#
# Do not change this
include $(PROJECT_DIR)/build/$(CUSTOM_OS)Makefile.post


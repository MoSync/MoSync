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
TOMMATH_HEADERS  = -I$(PROJECT_DIR)/libtommath
CFLAGS += $(TOMCRYPT_HEADERS) $(TOMMATH_HEADERS) $(CFLAG_SHADOW) \
          $(CFLAG_NO_UNUSED_PARAMETER) $(CFLAG_NO_UNUSED_FUNCTION) \
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
LIBTARGET = $(OUT_DIR)/libmath.a

#
# Set the name of the executable target of this makefile
TARGET = 

#
# Set all source files here
SRCS = multi.c \
	   ltm_desc_null.c

#
# Do not change this
include $(PROJECT_DIR)/build/$(CUSTOM_OS)Makefile.post


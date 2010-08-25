#
# Set the path to the top of the source tree
PROJECT_DIR = ..

#
# Do not change this
include $(PROJECT_DIR)/build/$(CUSTOM_OS)Makefile.incl

#
# List all subdirectories to do recursive make in
SUBDIRS = src

#
# Add include paths etc here
CFLAGS += 

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
LIBTARGET = 

#
# Set the name of the executable target of this makefile
TARGET = 

#
# Set all source files here
SRCS = 

MERGELIBS = src/ciphers/$(OUT_DIR)/libciphers.a \
            src/ciphers/aes/$(OUT_DIR)/libaes.a \
            src/hashes/$(OUT_DIR)/libhashes.a \
            src/hashes/helper/$(OUT_DIR)/libhelper.a \
            src/hashes/sha2/$(OUT_DIR)/libsha2.a \
            src/mac/hmac/$(OUT_DIR)/libhmac.a \
            src/misc/$(OUT_DIR)/libmisc.a \
            src/misc/crypt/$(OUT_DIR)/libcrypt.a \
            src/modes/cbc/$(OUT_DIR)/libcbc.a \
            src/modes/ecb/$(OUT_DIR)/libecb.a \
            src/prngs/$(OUT_DIR)/libprngs.a \
			src/pk/rsa/$(OUT_DIR)/librsa.a \
			src/math/$(OUT_DIR)/libmath.a
MERGETARGET = $(LIB_DIR)/libtomcrypt.a

#
# Do not change this
include $(PROJECT_DIR)/build/$(CUSTOM_OS)Makefile.post


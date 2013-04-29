LOCAL_PATH := $(PROJECT_DIR)

# Some defaults
MOSYNC_OUTPUT_REL_PATH := Output
MOSYNC_PLATFORM := Android
MOSYNC_VARIANT := Android
MOSYNC_OUTPUT_PATH := $(PROJECT_DIR)/$(MOSYNC_OUTPUT_REL_PATH)/$(MOSYNC_CONFIG)/$(MOSYNC_PLATFORM)/$(MOSYNC_VARIANT)

# This may cause issues with gdb, beware!
NDK_APP_OUT := $(MOSYNC_OUTPUT_PATH)/$(TARGET_ARCH_ABI)

APP_BUILD_SCRIPT	:= $(MOSYNC_OUTPUT_PATH)/Android.mk
APP_PROJECT_PATH	:= $(LOCAL_PATH)

APP_MODULES := $(MOSYNC_MODULE)
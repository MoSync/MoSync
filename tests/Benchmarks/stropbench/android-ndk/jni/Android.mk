LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
 
# Here we give our module name and source file(s)
LOCAL_MODULE    := stropbench
LOCAL_SRC_FILES := stropbench.cpp
 
include $(BUILD_SHARED_LIBRARY)
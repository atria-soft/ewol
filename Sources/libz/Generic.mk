LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zlib

LOCAL_LDLIBS := -lz

include $(EXTERN_LIBRARY)


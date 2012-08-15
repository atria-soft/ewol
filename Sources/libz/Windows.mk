LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zlib

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/zlib

LOCAL_CFLAGS := -D_LARGEFILE64_SOURCE=1 -DHAVE_HIDDEN

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

LOCAL_LDLIBS :=

include $(BUILD_STATIC_LIBRARY)


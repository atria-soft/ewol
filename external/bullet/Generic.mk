LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := bullet

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/bullet/src/

LOCAL_C_INCLUDES :=  \
                    $(LOCAL_PATH)/bullet/Extras/ConvexDecomposition

LOCAL_CFLAGS := -Wno-write-strings \
                -DHAVE_CONFIG_H


# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)

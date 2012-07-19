LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := etk
LOCAL_STATIC_LIBRARIES := libzip

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -Wall
else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=1 \
                -DMODE_RELEASE
endif


# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)



include $(BUILD_STATIC_LIBRARY)

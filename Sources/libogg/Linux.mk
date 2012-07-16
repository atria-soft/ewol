LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ogg

LOCAL_STATIC_LIBRARIES := etk

LOCAL_C_INCLUDES := $(LOCAL_PATH)/tremor/ \
                    $(LOCAL_PATH)/ogg/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DOGG_DEBUG_LEVEL=3 \
                -DOGG_VERSION_TAG_NAME="\"1.0.2-debug\""
                -DTREMOR_DEBUG_LEVEL=3 \
                -DTREMOR_VERSION_TAG_NAME="\"1.0.2-debug\""
else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DOGG_DEBUG_LEVEL=1 \
                -DOGG_VERSION_TAG_NAME="\"1.0.2-release\""
                -DTREMOR_DEBUG_LEVEL=1 \
                -DTREMOR_VERSION_TAG_NAME="\"1.0.2-release\""
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)

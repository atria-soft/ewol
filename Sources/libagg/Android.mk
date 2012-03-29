LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := agg

# name of the dependency
LOCAL_STATIC_LIBRARIES := libfreetype

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/agg-2.4/ \
                    $(LOCAL_PATH)/agg-2.4/util/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -DAGG_DEBUG_LEVEL=3 \
                -DAGG_VERSION_TAG_NAME="\"2.4-debug\""
else
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -DAGG_DEBUG_LEVEL=3 \
                -DAGG_VERSION_TAG_NAME="\"2.4-release\""
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)

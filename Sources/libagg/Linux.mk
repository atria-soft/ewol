LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := agg

# name of the dependency
LOCAL_STATIC_LIBRARIES := libfreetype

LOCAL_C_INCLUDES := -I$(LOCAL_PATH) \
                    -I$(LOCAL_PATH)/agg-2.4/ \
                    -I$(LOCAL_PATH)/agg-2.4/util/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := 

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DAGG_VERSION_TAG_NAME="\"2.4-debug\""
else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DAGG_VERSION_TAG_NAME="\"2.4-release\""
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := 


include $(BUILD_STATIC_LIBRARY)

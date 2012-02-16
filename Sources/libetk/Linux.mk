LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := etk
LOCAL_STATIC_LIBRARIES := libzip

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS :=

LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -std=c++0x

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := 


include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := tinyxml
LOCAL_STATIC_LIBRARIES :=

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/

LOCAL_CFLAGS := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := 


include $(BUILD_STATIC_LIBRARY)

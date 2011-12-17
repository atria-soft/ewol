LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := tinyxml
LOCAL_STATIC_LIBRARIES := 

LOCAL_C_INCLUDES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := 


# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)



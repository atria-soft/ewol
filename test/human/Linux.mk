LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_CONFIG_FILES := Config.in

# name of the librairy
LOCAL_MODULE := human

# name of the dependency
LOCAL_STATIC_LIBRARIES := ewol

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_SRC_FILES := $(FILE_LIST)


LOCAL_LDLIBS    := 

LOCAL_CFLAGS    :=  -DPROJECT_NAME="\"$(LOCAL_MODULE)\""

include $(BUILD_EXECUTABLE)


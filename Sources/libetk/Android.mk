LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := etk
LOCAL_STATIC_LIBRARIES := libzip

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \
                -DDATA_IN_APK \
                -DETK_DEBUG_LEVEL=3 \
                -std=c++0x





LOCAL_SRC_FILES := $(FILE_LIST)


# Ewol Test Software :
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_STATIC_LIBRARY)



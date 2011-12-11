LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_BUILD_PATH := $(LOCAL_PATH)/Object_android

LOCAL_MODULE := ewoltestExemple
#LOCAL_STATIC_LIBRARIES := ewolabstraction

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Sources

LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \

LOCAL_SRC_FILES := \
    ../../Sources/Main.cpp \

# Ewol Test Software :
CXXFILES +=		Main.cpp
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

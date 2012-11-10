LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := glew

#http://glew.sourceforge.net/index.html

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/glew/include/
LOCAL_EXPORT_LDLIBS := $(LOCAL_PATH)/glew/lib/glew32s.lib -lopengl32 -lgdi32

include $(BUILD_PREBUILT)


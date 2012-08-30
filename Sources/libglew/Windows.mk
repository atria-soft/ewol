LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := glew

#http://glew.sourceforge.net/index.html

ifeq ("$(SHADER)","1")
	LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/glew/include/
	LOCAL_EXPORT_LDLIBS := $(LOCAL_PATH)/glew/lib/glew32s.lib -lopengl32 -lgdi32
	# to enable open gl 2 mode
	LOCAL_CFLAGS += -D__VIDEO__OPENGL_ES_2
	LOCAL_EXPORT_CFLAGS := -D__VIDEO__OPENGL_ES_2
else
	LOCAL_EXPORT_LDLIBS := -lopengl32 -lgdi32
endif

include $(BUILD_PREBUILT)


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libzip libpng parsersvg lua

LOCAL_C_INCLUDES :=

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-$(BUILD_DIRECTORY_MODE)\"" \
                -DDATA_IN_APK

ifeq ("$(SHADER)","1")
	LOCAL_CFLAGS += -D__VIDEO__OPENGL_ES_2
	LOCAL_EXPORT_CFLAGS := -D__VIDEO__OPENGL_ES_2
	LOCAL_EXPORT_LDLIBS := -lGLESv2
else
	LOCAL_EXPORT_LDLIBS := -lGLESv1_CM
endif

LOCAL_EXPORT_LDLIBS += -ldl -llog

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := ewol/os/gui.Android.cpp $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)



LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

LOCAL_CONFIG_FILES := Config.in

# get the tag of the current project : 
LOCAL_VERSION=$(shell cat $(LOCAL_PATH)/tag)
$(info [TAG:$(LOCAL_MODULE)] $(LOCAL_VERSION))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libzip libpng parsersvg lua portaudio ode

LOCAL_C_INCLUDES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGL -lX11

#http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
# needed package on linux : libgl1-mesa-dev libglew1.5-dev


LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG)-$(BUILD_DIRECTORY_MODE)\"" \
                -Wall

LOCAL_EXPORT_CFLAGS := 

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	$(FILE_LIST) \
	ewol/renderer/os/gui.X11.cpp \
	ewol/renderer/audio/interfacePortAudio.cpp



include $(BUILD_STATIC_LIBRARY)

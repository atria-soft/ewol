LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

LOCAL_CONFIG_FILES := Config.in

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libzip libpng parsersvg lua portaudio

LOCAL_C_INCLUDES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGL -lX11

#http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
# needed package on linux : libgl1-mesa-dev libglew1.5-dev


LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-$(BUILD_DIRECTORY_MODE)\"" \
                -Wall

ifdef CONFIG_EWOL_OPENGL_ES_V1
	LOCAL_CFLAGS += -D__VIDEO__OPENGL_ES_2
	LOCAL_EXPORT_CFLAGS := -D__VIDEO__OPENGL_ES_2
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	ewol/os/gui.X11.cpp \
	ewol/audio/interfacePortAudio.cpp \
	$(FILE_LIST)


include $(BUILD_STATIC_LIBRARY)

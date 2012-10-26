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

#ifneq ("$(CONFIG__VIDEO__OPENGL_ES_2)","")
LOCAL_CFLAGS += -D__VIDEO__OPENGL_ES_2
LOCAL_EXPORT_CFLAGS := -D__VIDEO__OPENGL_ES_2
$(info Compilation For openGL-ES-v2 compatibility)
#endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	ewol/os/gui.X11.cpp \
	ewol/audio/interfacePortAudio.cpp \
	$(FILE_LIST)

LOCAL_END_PROJECT_NAME := $(call convert-special-char,$(CONFIG___EWOL_APPL_NAME__))

LOCAL_COPY_FILES := ../../share/textured3D.prog:usr/share/$(LOCAL_END_PROJECT_NAME)/textured3D.prog \
                    ../../share/textured3D.frag:usr/share/$(LOCAL_END_PROJECT_NAME)/textured3D.frag \
                    ../../share/textured3D.vert:usr/share/$(LOCAL_END_PROJECT_NAME)/textured3D.vert \
                    \
                    ../../share/color.prog:usr/share/$(LOCAL_END_PROJECT_NAME)/color.prog \
                    ../../share/color.frag:usr/share/$(LOCAL_END_PROJECT_NAME)/color.frag \
                    ../../share/color.vert:usr/share/$(LOCAL_END_PROJECT_NAME)/color.vert \
                    \
                    ../../share/textured.prog:usr/share/$(LOCAL_END_PROJECT_NAME)/textured.prog \
                    ../../share/textured.frag:usr/share/$(LOCAL_END_PROJECT_NAME)/textured.frag \
                    ../../share/textured.vert:usr/share/$(LOCAL_END_PROJECT_NAME)/textured.vert \
                    \
                    ../../share/widgetEntry.prog:usr/share/$(LOCAL_END_PROJECT_NAME)/widgetEntry.prog \
                    ../../share/widgetEntry.frag:usr/share/$(LOCAL_END_PROJECT_NAME)/widgetEntry.frag \
                    ../../share/widgetEntry.vert:usr/share/$(LOCAL_END_PROJECT_NAME)/widgetEntry.vert


include $(BUILD_STATIC_LIBRARY)

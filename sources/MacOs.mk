LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

LOCAL_CONFIG_FILES := Config.in ConfigLinux.in

# get the tag of the current project : 
LOCAL_VERSION=$(shell cat $(LOCAL_PATH)/tag)
$(info [TAG:$(LOCAL_MODULE)] $(LOCAL_VERSION))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libpng parsersvg
ifeq ("$(CONFIG_BUILD_BULLET)","y")
LOCAL_LIBRARIES += bullet
endif
ifeq ("$(CONFIG_BUILD_LUA)","y")
LOCAL_LIBRARIES += lua
endif
ifeq ("$(CONFIG_BUILD_PORTAUDIO)","y")
LOCAL_LIBRARIES += portaudio
endif
ifeq ("$(CONFIG_BUILD_LIBZIP)","y")
LOCAL_LIBRARIES += libzip
endif

LOCAL_C_INCLUDES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGL

#`pkg-config --cflags directfb` `pkg-config --libs directfb`

ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_X11__)","y")
LOCAL_EXPORT_LDLIBS += -lX11
endif
ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_DIRECT_FB__)","y")
LOCAL_EXPORT_LDLIBS +=  -L/usr/local/lib  -ldirectfb -lfusion -ldirect
endif

#http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
# needed package on linux : libgl1-mesa-dev libglew1.5-dev


LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG)-$(BUILD_DIRECTORY_MODE)\"" \
                -Wall

LOCAL_EXPORT_CFLAGS := 

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_X11__)","y")
LOCAL_SRC_FILES += ewol/renderer/os/gui.X11.cpp
endif
ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_DIRECT_FB__)","y")
LOCAL_CFLAGS += -I/usr/local/include/directfb
LOCAL_SRC_FILES += ewol/renderer/os/gui.directFB.cpp
endif


include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_STATIC_LIBRARIES :=    etk libfreetype    tinyxml libzip libpng    agg    parsersvg    lua    portaudio

LOCAL_C_INCLUDES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGL -lGLU -lz -lX11

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DEWOL_USE_FREE_TYPE \
                -DLUA_COMPAT_ALL \
                -Wall

else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-release\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DLUA_COMPAT_ALL \
                -DEWOL_USE_FREE_TYPE

endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	ewol/base/guiX11.cpp \
	ewol/Audio/interfacePortAudio.cpp \
	$(FILE_LIST)

# Ewol Test Software :
LOCAL_LDLIBS := -lGL -lGLU -lz -lX11


include $(BUILD_STATIC_LIBRARY)

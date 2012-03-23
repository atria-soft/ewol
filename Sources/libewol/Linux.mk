LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ewol
LOCAL_STATIC_LIBRARIES := etk libfreetype tinyxml libzip libpng agg parsersvg

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGL -lGLU -lz -lX11

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DEWOL_USE_FREE_TYPE \
                -Wall
else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=1 \
                -DEWOL_DEBUG_LEVEL=1 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DEWOL_USE_FREE_TYPE
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	ewol/base/guiX11.cpp \
	$(FILE_LIST)

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := -lGL -lGLU -lz -lX11


include $(BUILD_STATIC_LIBRARY)

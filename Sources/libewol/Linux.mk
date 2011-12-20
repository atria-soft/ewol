LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ewol
LOCAL_STATIC_LIBRARIES := etk tinyxml libzip libpng 

LOCAL_C_INCLUDES := -I$(LOCAL_PATH) -I$(LOCAL_PATH)/../libzip/ -I$(LOCAL_PATH)/../libpng/ -I$(LOCAL_PATH)/../libtinyxml/ -I$(LOCAL_PATH)/../libetk/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGL -lGLU -lz -lX11 -lXxf86vm

LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DEWOL_X11_MODE__XF86V

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	ewol/base/guiX11.cpp \
	$(FILE_LIST)

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := -lGL -lGLU -lz -lX11 -lXxf86vm


include $(BUILD_STATIC_LIBRARY)

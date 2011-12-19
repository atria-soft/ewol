LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ewol
LOCAL_STATIC_LIBRARIES := libzip libpng

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Sources
LOCAL_EXPORT_LDLIBS := -lGLESv1_CM -ldl -llog -lz

LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DDATA_IN_APK \

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := \
	base/guiX11.cpp \
	$(FILE_LIST)

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz


include $(BUILD_STATIC_LIBRARY)



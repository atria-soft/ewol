LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ewol
LOCAL_STATIC_LIBRARIES := libetk libtinyxml libzip libpng libfreetype libagg libparsersvg

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGLESv1_CM -ldl -llog -lz

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -D__MODE__Touch \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DDATA_IN_APK \
                -frtti
else
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -D__MODE__Touch \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=1 \
                -DEWOL_DEBUG_LEVEL=1 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DDATA_IN_APK \
                -frtti
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := ewol/base/guiAndroid.cpp $(FILE_LIST)

# Ewol Test Software :
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_STATIC_LIBRARY)



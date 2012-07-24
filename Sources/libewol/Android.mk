LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_STATIC_LIBRARIES := libetk libfreetype libtinyxml libzip libpng libagg libparsersvg liblua

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lGLESv1_CM -ldl -llog -lz

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-debug\"" \
                -DBUILD_TIME="\"$(BUILD_TIME)\"" \
                -DDATA_IN_APK \
                -DLUA_COMPAT_ALL \
                -frtti
else
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=1 \
                -DEWOL_DEBUG_LEVEL=1 \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-debug\"" \
                -DBUILD_TIME="\"$(BUILD_TIME)\"" \
                -DDATA_IN_APK \
                -DLUA_COMPAT_ALL \
                -frtti
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := ewol/base/guiAndroid.cpp $(FILE_LIST)

# Ewol Test Software :
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_STATIC_LIBRARY)



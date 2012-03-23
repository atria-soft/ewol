LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := parsersvg

# name of the dependency
LOCAL_STATIC_LIBRARIES := libetk libagg libtinyxml

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := 

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DPARSER_SVG_VERSION_TAG_NAME="\"???-debug\"" \
                -Wall -Wextra
else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DPARSER_SVG_VERSION_TAG_NAME="\"???-release\""
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

# Ewol Test Software :
LOCAL_LDLIBS := 

include $(BUILD_STATIC_LIBRARY)



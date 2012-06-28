LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := portaudio

# name of the dependency
LOCAL_STATIC_LIBRARIES := 

LOCAL_C_INCLUDES := -I$(LOCAL_PATH) \
                    -I$(LOCAL_PATH)/portaudio/ \
                    -I$(LOCAL_PATH)/portaudio/common \
                    -I$(LOCAL_PATH)/portaudio/os/unix

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -lasound

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DPORTAUDIO_DEBUG_LEVEL=3 \
                -DPA_USE_ALSA \
                -DPORTAUDIO_VERSION_TAG_NAME="\"v19-debug\""
else
LOCAL_CFLAGS := -D__PLATFORM__Linux \
                -DPORTAUDIO_DEBUG_LEVEL=3 \
                -DPA_USE_ALSA \
                -DPORTAUDIO_VERSION_TAG_NAME="\"v19-release\""
endif

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)
LOCAL_LDLIBS := -lasound

include $(BUILD_STATIC_LIBRARY)

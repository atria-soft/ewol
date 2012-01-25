LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := freetype
LOCAL_STATIC_LIBRARIES := 

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(addprefix $(LOCAL_PATH)/, $(sort $(dir $(FILE_LIST))))

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS += -W -Wall \
                -fPIC -DPIC \
                -DDARWIN_NO_CARBON \
                -DFT2_BUILD_LIBRARY \
                -DANDROID_FONT_HACK=1




LOCAL_SRC_FILES:= $(FILE_LIST)



ifeq ($(TARGET_BUILD_TYPE),release)
	LOCAL_CFLAGS += -O2
endif

include $(BUILD_STATIC_LIBRARY)

#for freetype : https://github.com/cdave1/freetype2-android


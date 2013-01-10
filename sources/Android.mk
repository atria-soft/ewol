LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

LOCAL_CONFIG_FILES := Config.in ConfigAndroid.in

# get the tag of the current project : 
LOCAL_VERSION=$(shell cat $(LOCAL_PATH)/tag)
$(info [TAG:$(LOCAL_MODULE)] $(LOCAL_VERSION))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libpng parsersvg lua libzip bullet

LOCAL_C_INCLUDES :=

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG)-$(BUILD_DIRECTORY_MODE)\"" \
                -DDATA_IN_APK \
                -O2

#LOCAL_EXPORT_CFLAGS := -DBT_USE_NEON

LOCAL_EXPORT_LDLIBS := -lGLESv2

LOCAL_EXPORT_LDLIBS += -ldl -llog


FILE_ABSTRACTION:=$(LOCAL_PATH)/ewol/renderer/os/gui.Android.base.cpp
FILE_ABSTRACTION_DEST:=ewol/renderer/os/gui.Android.tmp.cpp

EWOL_TMP_PATH:=$(LOCAL_PATH)

$(shell rm -f $(LOCAL_PATH)/$(FILE_ABSTRACTION_DEST))

$(LOCAL_PATH)/$(FILE_ABSTRACTION_DEST): $(FILE_ABSTRACTION)
	$(Q)mkdir -p $(dir $@)
	$(Q)cp -f $(FILE_ABSTRACTION) $@
	$(Q)sed -i "s|__PROJECT_ORG_TYPE__|$(PROJECT_COMPAGNY_TYPE)|" $@
	$(Q)sed -i "s|__PROJECT_VENDOR__|$(PROJECT_COMPAGNY_NAME2)|" $@
	$(Q)sed -i "s|__PROJECT_NAME__|$(PROJECT_NAME2)|" $@
	$(Q)sed -i "s|__PROJECT_PACKAGE__|$(PROJECT_NAME2)|" $@
$(FILE_ABSTRACTION_DEST): $(LOCAL_PATH)/$(FILE_ABSTRACTION_DEST)
	


# this is the abstraction file for Android
LOCAL_PREREQUISITES := $(FILE_ABSTRACTION_DEST)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := ewol/renderer/os/gui.Android.cpp $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)



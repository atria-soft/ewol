LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# name of the librairy
LOCAL_MODULE := ewol

LOCAL_CONFIG_FILES := Config.in

# get the tag of the current project : 
LOCAL_VERSION_TAG=$(shell cd $(LOCAL_PATH) ; git describe --tags)
LOCAL_VERSION_TAG_SHORT=$(shell cd $(LOCAL_PATH) ; git describe --tags --abbrev=0)
$(info $(LOCAL_MODULE) version TAG : $(LOCAL_VERSION_TAG))

# name of the dependency
LOCAL_LIBRARIES := etk freetype tinyxml libpng parsersvg lua libzip

LOCAL_C_INCLUDES :=

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := -Wno-write-strings \
                -DEWOL_VERSION_TAG_NAME="\"$(LOCAL_VERSION_TAG_SHORT)-$(BUILD_DIRECTORY_MODE)\"" \
                -DDATA_IN_APK

#ifeq ("$(CONFIG__VIDEO__OPENGL_ES_2)","y")
LOCAL_CFLAGS += -D__VIDEO__OPENGL_ES_2
LOCAL_EXPORT_CFLAGS := -D__VIDEO__OPENGL_ES_2
LOCAL_EXPORT_LDLIBS := -lGLESv2
#else
#	LOCAL_EXPORT_LDLIBS := -lGLESv1_CM
#endif

LOCAL_EXPORT_LDLIBS += -ldl -llog


FILE_ABSTRACTION:=$(LOCAL_PATH)/ewol/os/AndroidAbstractionBase.cpp
FILE_ABSTRACTION_DEST:=ewol/os/AndroidAbstraction.cpp

EWOL_TMP_PATH:=$(LOCAL_PATH)

$(FILE_ABSTRACTION_DEST): $(FILE_ABSTRACTION)
	@mkdir -p $(dir $(EWOL_TMP_PATH)/$@)
	cp -f $(FILE_ABSTRACTION) $(EWOL_TMP_PATH)/$@
	sed -i "s|__PROJECT_VENDOR__|$(PROJECT_VENDOR)|" $(EWOL_TMP_PATH)/$@
	sed -i "s|__PROJECT_NAME__|$(PROJECT_NAME)|" $(EWOL_TMP_PATH)/$@
	sed -i "s|__PROJECT_PACKAGE__|$(PROJECT_PACKAGE)|" $(EWOL_TMP_PATH)/$@


# this is the abstraction file for Android
LOCAL_PREREQUISITES := $(FILE_ABSTRACTION_DEST)

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := ewol/os/gui.Android.cpp $(FILE_ABSTRACTION_DEST) $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)



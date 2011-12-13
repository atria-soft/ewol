LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ewol
LOCAL_STATIC_LIBRARIES := libzip libpng

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Sources $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Sources
LOCAL_EXPORT_LDLIBS := -lGLESv1_CM -ldl -llog -lz

LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -DEWOL_DEBUG_LEVEL=3 \
                -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" \
                -DVERSION_BUILD_TIME="\"pasd_heure\"" \
                -DDATA_IN_APK \

LOCAL_SRC_FILES := \
    ../../Sources/base/guiAndroid.cpp \
    ../../Sources/tinyXML/tinyxml.cpp \
    ../../Sources/tinyXML/tinyxmlparser.cpp \
    ../../Sources/tinyXML/tinyxmlerror.cpp \
    ../../Sources/tinyXML/tinystr.cpp \
    ../../Sources/etk/Debug.cpp \
    ../../Sources/etk/DebugInternal.cpp \
    ../../Sources/etk/Memory.cpp \
    ../../Sources/etk/String.cpp \
    ../../Sources/etk/Stream.cpp \
    ../../Sources/etk/File.cpp \
    ../../Sources/etk/RegExp.cpp \
    ../../Sources/ewol/ewol.cpp \
    ../../Sources/ewol/Debug.cpp \
    ../../Sources/ewol/OObject.cpp \
    ../../Sources/ewol/OObject/2DText.cpp \
    ../../Sources/ewol/OObject/2DColored.cpp \
    ../../Sources/ewol/OObject/2DTextured.cpp \
    ../../Sources/ewol/Texture.cpp \
    ../../Sources/ewol/FontBitmap.cpp \
    ../../Sources/ewol/Widget.cpp \
    ../../Sources/ewol/WidgetManager.cpp \
    ../../Sources/ewol/Windows.cpp \
    ../../Sources/ewol/widget/Button.cpp \
    ../../Sources/ewol/widget/CheckBox.cpp \
    ../../Sources/ewol/widget/Entry.cpp \
    ../../Sources/ewol/widget/List.cpp \
    ../../Sources/ewol/widget/SizerHori.cpp \
    ../../Sources/ewol/widget/SizerVert.cpp \
    ../../Sources/ewol/widget/Test.cpp \
    ../../Sources/ewol/themeManager.cpp \
    ../../Sources/ewol/theme/Theme.cpp \
    ../../Sources/ewol/theme/EolElement.cpp \
    ../../Sources/ewol/theme/EolElementFrame.cpp \
    ../../Sources/ewol/theme/EolColor.cpp \
    ../../Sources/ewol/theme/EolBase.cpp \
    ../../Sources/ewol/theme/EolBaseCircle.cpp \
    ../../Sources/ewol/theme/EolBaseCirclePart.cpp \
    ../../Sources/ewol/theme/EolBaseLine.cpp \
    ../../Sources/ewol/theme/EolBasePolygone.cpp \
    ../../Sources/ewol/theme/EolBaseRect.cpp \
    ../../Sources/ewol/theme/EolBaseTriangle.cpp

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

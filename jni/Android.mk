LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_BUILD_PATH := $(LOCAL_PATH)/Object_android

LOCAL_MODULE := ewolabstraction

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Sources

LOCAL_CFLAGS := -D__PLATFORM__Android -Wno-write-strings -DETK_DEBUG_LEVEL=3 -DEWOL_DEBUG_LEVEL=3 -DEWOL_VERSION_TAG_NAME="\"UNKNOW-debug\"" -DVERSION_BUILD_TIME="\"pasd_heure\""

#enable io stream in the STL ...
#APP_STL := gnustl_static
#APP_STL := stlport_shared

LOCAL_SRC_FILES := \
    ../Sources/demo.cpp \
    ../Sources/Main.cpp \
    ../Sources/base/guiAndroid.cpp \
    ../Sources/tinyXML/tinyxml.cpp \
    ../Sources/tinyXML/tinyxmlparser.cpp \
    ../Sources/tinyXML/tinyxmlerror.cpp \
    ../Sources/tinyXML/tinystr.cpp \
    ../Sources/etk/Debug.cpp \
    ../Sources/etk/DebugInternal.cpp \
    ../Sources/etk/Memory.cpp \
    ../Sources/etk/String.cpp \
    ../Sources/etk/Stream.cpp \
    ../Sources/etk/File.cpp \
    ../Sources/etk/RegExp.cpp \
    ../Sources/ewol/ewol.cpp \
    ../Sources/ewol/Debug.cpp \
    ../Sources/ewol/OObject.cpp \
    ../Sources/ewol/OObject/2DText.cpp \
    ../Sources/ewol/OObject/2DColored.cpp \
    ../Sources/ewol/OObject/2DTextured.cpp \
    ../Sources/ewol/Texture.cpp \
    ../Sources/ewol/FontBitmap.cpp \
    ../Sources/ewol/Widget.cpp \
    ../Sources/ewol/WidgetManager.cpp \
    ../Sources/ewol/Windows.cpp \
    ../Sources/ewol/widget/Button.cpp \
    ../Sources/ewol/widget/CheckBox.cpp \
    ../Sources/ewol/widget/Entry.cpp \
    ../Sources/ewol/widget/List.cpp \
    ../Sources/ewol/widget/SizerHori.cpp \
    ../Sources/ewol/widget/SizerVert.cpp \
    ../Sources/ewol/widget/Test.cpp \
    ../Sources/ewol/themeManager.cpp \
    ../Sources/ewol/theme/Theme.cpp \
    ../Sources/ewol/theme/EolElement.cpp \
    ../Sources/ewol/theme/EolElementFrame.cpp \
    ../Sources/ewol/theme/EolColor.cpp \
    ../Sources/ewol/theme/EolBase.cpp \
    ../Sources/ewol/theme/EolBaseCircle.cpp \
    ../Sources/ewol/theme/EolBaseCirclePart.cpp \
    ../Sources/ewol/theme/EolBaseLine.cpp \
    ../Sources/ewol/theme/EolBasePolygone.cpp \
    ../Sources/ewol/theme/EolBaseRect.cpp \
    ../Sources/ewol/theme/EolBaseTriangle.cpp

#for freetype : https://github.com/cdave1/freetype2-android

# Ewol Test Software :
CXXFILES +=		Main.cpp
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)

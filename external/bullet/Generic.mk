
# --------------------------------------------------------
# -- Linear Math librairy
# --------------------------------------------------------
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := linearmath

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/bullet/src/

LOCAL_CFLAGS := -Wno-write-strings \
                -DHAVE_CONFIG_H\
                -O2

LOCAL_SRC_FILES := \
			bullet/src/LinearMath/btQuickprof.cpp \
			bullet/src/LinearMath/btGeometryUtil.cpp \
			bullet/src/LinearMath/btAlignedAllocator.cpp \
			bullet/src/LinearMath/btSerializer.cpp \
			bullet/src/LinearMath/btConvexHull.cpp \
			bullet/src/LinearMath/btPolarDecomposition.cpp \
			bullet/src/LinearMath/btVector3.cpp \
			bullet/src/LinearMath/btConvexHullComputer.cpp

include $(BUILD_STATIC_LIBRARY)


# --------------------------------------------------------
# -- Bullet librairy
# --------------------------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := bullet

LOCAL_LIBRARIES := linearmath

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/bullet/src/

LOCAL_C_INCLUDES :=  \
                    $(LOCAL_PATH)/bullet/Extras/ConvexDecomposition

LOCAL_CFLAGS := -Wno-write-strings \
                -DHAVE_CONFIG_H\
                -O2


# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_SRC_FILES := $(FILE_LIST)

include $(BUILD_STATIC_LIBRARY)

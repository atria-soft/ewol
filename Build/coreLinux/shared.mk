###############################################################################
## @file shared.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## Build a shared library.
###############################################################################

LOCAL_MODULE_SUFFIX := $(TARGET_SHARED_LIB_SUFFIX)
LOCAL_BUILDING_SHARED_LIBRARY := 1

include $(RULES)

# Generate .dll.a file under mingw32
ifeq ("$(OS_MINGW32)","1")
$(LOCAL_BUILT_MODULE)$(TARGET_STATIC_LIB_SUFFIX): $(LOCAL_BUILT_MODULE)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LDFLAGS += \
	-Wl,--out-implib -Wl,$(LOCAL_BUILT_MODULE)$(TARGET_STATIC_LIB_SUFFIX)
endif

$(LOCAL_BUILT_MODULE): $(all_objects) $(all_libraries)
	$(transform-o-to-shared-lib)

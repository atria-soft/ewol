###############################################################################
## @file static.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## Build a static library.
###############################################################################

LOCAL_MODULE_SUFFIX := $(TARGET_STATIC_LIB_SUFFIX)
LOCAL_BUILDING_STATIC_LIBRARY := 1

include $(RULES)

#$(info plopppppp=$(LOCAL_BUILT_MODULE)= $(all_libraries))
$(LOCAL_BUILT_MODULE): $(all_libraries) $(all_objects) 
	$(transform-o-to-static-lib)

LIB_EXTERN_C_INCLUDE := $(LIB_EXTERN_C_INCLUDE) $(LOCAL_EXPORT_C_INCLUDES)
LIB_EXTERN_LDLIBS := $(LIB_EXTERN_LDLIBS) $(LOCAL_EXPORT_LDLIBS)

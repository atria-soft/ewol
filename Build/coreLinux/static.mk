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

$(LOCAL_BUILT_MODULE): $(all_objects)
	$(transform-o-to-static-lib)


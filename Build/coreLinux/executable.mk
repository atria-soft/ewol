###############################################################################
## @file executable.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## Build an executable.
###############################################################################

LOCAL_MODULE_SUFFIX := $(TARGET_EXECUTABLE_SUFFIX)
LOCAL_BUILDING_EXECUTABLE := 1

include $(RULES)

$(LOCAL_BUILT_MODULE): $(all_libraries) $(all_objects)
	$(transform-o-to-executable)


###############################################################################
## @file clearvars.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## Clear out values of all variables used by rule templates.
###############################################################################

# Do NOT clear LOCAL_PATH, it is set BEFORE including this makefile

# Name of what's supposed to be generated
LOCAL_MODULE :=

# Source files to compile
LOCAL_SRC_FILES :=

# Files to copy verbatim
LOCAL_COPY_FILES :=

# Static libraries that you want to include in your module
LOCAL_STATIC_LIBRARIES :=

# Libraries you directly link against
# Specify the name without the suffix
LOCAL_SHARED_LIBRARIES :=

# Additional directories to instruct the C/C++ compilers to look for header files in.
# Format : -I<fullpath>
LOCAL_C_INCLUDES :=

# Additional flags to pass into the C or C++ compiler
LOCAL_CFLAGS :=

# Additional flags to pass into only the C++ compiler
LOCAL_CPPFLAGS :=

# Additional flags to pass into the static library generator
LOCAL_ARFLAGS :=

# Additional flags to pass into the resource compiler (Windows only)
LOCAL_RCFLAGS :=

# Additional flags to pass into the linker
LOCAL_LDFLAGS :=

# Additional libraries to pass into the linker
# Format : -l<name>
LOCAL_LDLIBS :=

# Resource list file
LOCAL_RESLIST :=

# Xrc file
LOCAL_XRC :=

# Additionnal dependecies for rc files
LOCAL_RC_DEPS :=

# Precompiled file
LOCAL_PRECOMPILED_FILE :=

# Other variables used internally
LOCAL_BUILT_MODULE :=
LOCAL_INSTALLED_MODULE :=
LOCAL_INTERMEDIATE_TARGETS :=
LOCAL_MODULE_SUFFIX :=
LOCAL_BUILDING_STATIC_LIBRARY :=
LOCAL_BUILDING_SHARED_LIBRARY :=
LOCAL_BUILDING_EXECUTABLE :=

# Trim MAKEFILE_LIST so that $(call my-dir) doesn't need to
# iterate over thousands of entries every time.
# Leave the current makefile to make sure we don't break anything
# that expects to be able to find the name of the current makefile.
MAKEFILE_LIST := $(lastword $(MAKEFILE_LIST))


###############################################################################
## @file main.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## Main Makefile.
###############################################################################

# Make sure SHELL is correctly set
SHELL := /bin/bash

# Turns off suffix rules built into make
.SUFFIXES:

# Installation variables
PACKAGE := fsanalyser
DESTDIR :=
PREFIX := /usr/local

# Tools
GCC := gcc
GXX := g++
AR := ar
LD := ld
WINDRES := windres

# Overridable settings
V := 0
W := 0
DEBUG := 0
STATIC := 0

# Quiet command if V is not 1
ifneq ("$(V)","1")
  Q := @
endif

# Directories
TOP_DIR := .
#BUILD_SYSTEM := $(TOP_DIR)/Build/coreLinux
BUILD_SYSTEM := $(PROJECT_NDK)/Build/coreLinux/

# This is the default target.  It must be the first declared target.
all:

# Global variables
TARGET_PROJECT_INCLUDES :=
TARGET_C_INCLUDES :=
TARGET_GLOBAL_CFLAGS := -fno-exceptions -fstrict-aliasing
TARGET_GLOBAL_CPPFLAGS :=
TARGET_GLOBAL_RCFLAGS :=
TARGET_GLOBAL_ARFLAGS := -rcs
TARGET_GLOBAL_LDFLAGS := -Wl,--gc-sections -Wl,--warn-common -Wl,--warn-constructors
ALL_MODULES :=

# To be able to load automatically .so libraries located in the same
# folder than the application, we need to add $ORIGIN to DT_RPATH
# '\' is to escape the '$' in the shell command
# '$$' is to escape the '$' in the makefile
TARGET_GLOBAL_LDFLAGS += -Wl,-rpath,\$$ORIGIN

###############################################################################
# Version
###############################################################################
VERSION_MAJOR := 111
VERSION_MINOR := 2222
VERSION_REVISION := 3453
VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_REVISION)

###############################################################################
###############################################################################

# Setup macros definitions
include $(BUILD_SYSTEM)/defs.mk

# Setup configuration
include $(BUILD_SYSTEM)/config.mk

# Names of makefiles that can be included by "local.mk" Makefiles
CLEAR_VARS := $(BUILD_SYSTEM)/clearvars.mk
BUILD_STATIC_LIBRARY := $(BUILD_SYSTEM)/static.mk
BUILD_SHARED_LIBRARY := $(BUILD_SYSTEM)/shared.mk
BUILD_EXECUTABLE := $(BUILD_SYSTEM)/executable.mk
RULES := $(BUILD_SYSTEM)/rules.mk

###############################################################################
## Flags to activate warnings.
###############################################################################

COMMON_FLAGS_WARNINGS :=
GCC_FLAGS_WARNINGS :=
GXX_FLAGS_WARNINGS :=

COMMON_FLAGS_WARNINGS += -Wall

# Add common flags to specific flags
GCC_FLAGS_WARNINGS += $(COMMON_FLAGS_WARNINGS)
GXX_FLAGS_WARNINGS += $(COMMON_FLAGS_WARNINGS)

###############################################################################
###############################################################################

# Get the list of all makefiles available and include them
makefiles += $(shell find $(TOP_DIR) -name Linux.mk)
include $(makefiles)

TARGET_GLOBAL_CPPFLAGS := $(addprefix -I, $(LIB_EXTERN_C_INCLUDE))
TARGET_GLOBAL_LDFLAGS := $(TARGET_GLOBAL_LDFLAGS) $(LIB_EXTERN_LDLIBS)
###############################################################################
# Main rules.
###############################################################################

.PHONY: all
all: $(foreach m,$(ALL_MODULES),$(m))

.PHONY: clean
clean: $(foreach m,$(ALL_MODULES),clean-$(m))

.PHONY: distclean
distclean: clean

.PHONY: test
test:

.PHONY: install
install: $(foreach m,$(ALL_MODULES),install-$(m))

###############################################################################
# debian package generation.
###############################################################################

.PHONY: deb
deb:
	@chmod +x debian/rules
	$(Q)fakeroot debian/rules binary

.PHONY: deb-clean
deb-clean:
	@chmod +x debian/rules
	$(Q)fakeroot debian/rules clean

###############################################################################
# source/binary pakage generation.
###############################################################################

# Generate archive with source files from git
SRCZIP_NAME := $(PACKAGE)-src-$(VERSION)
.PHONY: src-zip
src-zip:
	@rm -f $(SRCZIP_NAME).tar
	@rm -f $(SRCZIP_NAME).tar.bz2
	$(Q)git archive --format=tar --prefix=$(SRCZIP_NAME)/ \
		-o $(SRCZIP_NAME).tar HEAD
	$(Q)bzip2 $(SRCZIP_NAME).tar

# Generate archive with binary files
BINZIP_NAME := $(PACKAGE)-bin-$(VERSION)
.PHONY: bin-zip
bin-zip:
	@rm -f $(BINZIP_NAME).tar
	@rm -f $(BINZIP_NAME).tar.bz2
	$(Q)tar -cf $(BINZIP_NAME).tar -C $(TARGET_OUT) \
		--transform "s/\.\(\/.*\)/$(BINZIP_NAME)\\1/" .
	$(Q)bzip2 $(BINZIP_NAME).tar

###############################################################################
# Display configuration.
###############################################################################

$(info ----------------------------------------------------------------------)
$(info + VERSION = $(VERSION))
$(info + DEBUG = $(DEBUG))
$(info + STATIC = $(STATIC))
$(info + TARGET_OUT_INTERMEDIATES = $(TARGET_OUT_INTERMEDIATES))
$(info + TARGET_OUT = $(TARGET_OUT))
$(info + GCC_PATH = $(GCC_PATH))
$(info + GCC_VERSION = $(GCC_VERSION))
$(info ----------------------------------------------------------------------)


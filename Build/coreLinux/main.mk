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
BUILD_SYSTEM := /home/edupin/progperso/ewol/Build/coreLinux/

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
#VERSION_FILE := $(TOP_DIR)/App/Res/Version.h
#VERSION_MAJOR := $(shell grep "VERSINFO_PRODUCT_VERSION_MAJOR" $(VERSION_FILE) | sed -e "s/.*\([0-9]\+\).*/\\1/")
#VERSION_MINOR := $(shell grep "VERSINFO_PRODUCT_VERSION_MINOR" $(VERSION_FILE) | sed -e "s/.*\([0-9]\+\).*/\\1/")
#VERSION_REVISION := $(shell grep "VERSINFO_PRODUCT_VERSION_REVISION" $(VERSION_FILE) | sed -e "s/.*\([0-9]\+\).*/\\1/")
VERSION_MAJOR := 1
VERSION_MINOR := 2
VERSION_REVISION := 3
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
COMMON_FLAGS_WARNINGS += -Wno-unused -Wunused-value -Wunused-variable -Wunused-label
COMMON_FLAGS_WARNINGS += -Wextra
COMMON_FLAGS_WARNINGS += -Wshadow
COMMON_FLAGS_WARNINGS += -Wswitch-default
COMMON_FLAGS_WARNINGS += -Wwrite-strings
COMMON_FLAGS_WARNINGS += -Wundef
COMMON_FLAGS_WARNINGS += -Wpointer-arith

# C specific
GCC_FLAGS_WARNINGS += -Wmissing-declarations
GCC_FLAGS_WARNINGS += -Wmissing-prototypes

# Extra warnings
ifeq ("$(W)","1")

COMMON_FLAGS_WARNINGS += -Wconversion
COMMON_FLAGS_WARNINGS += -Wswitch-enum
COMMON_FLAGS_WARNINGS += -Wcast-qual

# gcc >= 4.4.0
ifneq (0,$(shell expr $(GCC_VERSION) \>= 4.4.0))
COMMON_FLAGS_WARNINGS += -Wframe-larger-than=1024
endif

# c++ specific
GXX_FLAGS_WARNINGS += -Wctor-dtor-privacy
GXX_FLAGS_WARNINGS += -Wnon-virtual-dtor
GXX_FLAGS_WARNINGS += -Wreorder
GXX_FLAGS_WARNINGS += -Woverloaded-virtual

endif

# Add common flags to specific flags
GCC_FLAGS_WARNINGS += $(COMMON_FLAGS_WARNINGS)
GXX_FLAGS_WARNINGS += $(COMMON_FLAGS_WARNINGS)

###############################################################################
###############################################################################

# Get the list of all makefiles available and include them
makefiles += $(shell find $(TOP_DIR) -name Linux.mk)
include $(makefiles)

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


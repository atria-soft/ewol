###############################################################################
## @file config.mk
## @author Y.M. Morgan
## @date 2011/05/14
###############################################################################

###############################################################################
## Make sure that there are no spaces in the absolute path; the build system
## can't deal with them.
###############################################################################

ifneq ("$(words $(shell pwd))","1")
$(error Top directory contains space characters)
endif

###############################################################################
## OS type.
###############################################################################

# set MingW32 flags
ifneq ("$(shell echo $$OSTYPE | grep msys)","")
  OS_MINGW32 := 1
  DIR_SUFFIX := _mingw
else
  OS_MINGW32 := 0
  DIR_SUFFIX :=
endif

# Exe/dll suffix under mingw
TARGET_STATIC_LIB_SUFFIX := .a
ifeq ("$(OS_MINGW32)","1")
  TARGET_EXE_SUFFIX := .exe
  TARGET_SHARED_LIB_SUFFIX := .dll
else
  TARGET_EXE_SUFFIX :=
  TARGET_SHARED_LIB_SUFFIX := .so
endif

ifeq ("$(OS_MINGW32)","1")
  TARGET_GLOBAL_CFLAGS += -D__MINGW_FEATURES__=0
endif

###############################################################################
## Variables based on DEBUG/STATIC.
###############################################################################

ifeq ("$(DEBUG)","0")
  TARGET_GLOBAL_CFLAGS += -O2 -g0 -DNDEBUG -DUNICODE -D_UNICODE
  TARGET_GLOBAL_LDFLAGS += -Wl,--strip-all
  TARGET_OUT_INTERMEDIATES := $(TOP_DIR)/linux/obj$(DIR_SUFFIX)/release
  TARGET_OUT := $(TOP_DIR)/linux/bin$(DIR_SUFFIX)/release
else
  TARGET_GLOBAL_CFLAGS += -O0 -g2 -DDEBUG -D_DEBUG -DUNICODE -D_UNICODE
  TARGET_GLOBAL_LDFLAGS +=
  TARGET_OUT_INTERMEDIATES := $(TOP_DIR)/linux/obj$(DIR_SUFFIX)/debug
  TARGET_OUT := $(TOP_DIR)/linux/bin$(DIR_SUFFIX)/debug
endif


###############################################################################
## Determine gcc path and version.
###############################################################################

GCC_PATH := $(shell which $(GCC))
GCC_VERSION := $(shell $(GCC) --version | head -1 \
	| sed "s/.*\([0-9]\.[0-9]\.[0-9]\).*/\1/")


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
  TARGET_OUT_INTERMEDIATES := $(TOP_DIR)/build_gcc$(DIR_SUFFIX)/release
  TARGET_OUT := $(TOP_DIR)/out_gcc$(DIR_SUFFIX)/release
else
  TARGET_GLOBAL_CFLAGS += -O0 -g2 -DDEBUG -D_DEBUG -DUNICODE -D_UNICODE
  TARGET_GLOBAL_LDFLAGS +=
  TARGET_OUT_INTERMEDIATES := $(TOP_DIR)/build_gcc$(DIR_SUFFIX)/debug
  TARGET_OUT := $(TOP_DIR)/out_gcc$(DIR_SUFFIX)/debug
endif

###############################################################################
## wxWidgets configuration.
###############################################################################

# wx-config script
WX_CONFIG := wx-config
ifeq ("$(shell which $(WX_CONFIG))","")
$(warning Unable to find $(WX_CONFIG))
$(error Please make sure wxWidgets development package is installed)
endif

# Argument for debug
ifeq ("$(DEBUG)","0")
  WX_CONFIG_ARGS += --debug=no
else
  WX_CONFIG_ARGS += --debug=yes
endif

# Argument for unicode
WX_CONFIG_ARGS += --unicode=yes

# Argument for static
ifeq ("$(STATIC)","0")
  WX_CONFIG_ARGS += --static=no
else
  WX_CONFIG_ARGS += --static=yes
endif

# Get wxWidgets configuration
WX_LIBS := $(shell $(WX_CONFIG) --libs $(WX_CONFIG_ARGS))
WX_FLAGS := $(shell $(WX_CONFIG) --cppflags $(WX_CONFIG_ARGS))
WX_VERSION := $(shell $(WX_CONFIG) --version $(WX_CONFIG_ARGS))

# Check that the configuration exists
ifeq ("$(WX_LIBS)","")
$(error wxWidgets configuration not supported: $(WX_CONFIG_ARGS))
endif

# wxWidgets >= 2.8.10 required
ifeq (0,$(shell expr $(WX_VERSION) \>= 2.8.10))
$(error Unsupported version of wxWidgets: $(WX_VERSION))
endif

###############################################################################
## Determine gcc path and version.
###############################################################################

GCC_PATH := $(shell which $(GCC))
GCC_VERSION := $(shell $(GCC) --version | head -1 \
	| sed "s/.*\([0-9]\.[0-9]\.[0-9]\).*/\1/")


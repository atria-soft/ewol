###############################################################################
## @file rules.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## Generate rules for building an executable or library.
###############################################################################

# Make sure LOCAL_MODULE is defined and not empty
LOCAL_MODULE := $(strip $(LOCAL_MODULE))
ifeq ("$(LOCAL_MODULE)","")
  $(error $(LOCAL_PATH): LOCAL_MODULE is not defined)
endif

# Intermediates directory
intermediates := $(TARGET_OUT_INTERMEDIATES)/$(LOCAL_MODULE)

# Full path to build module
LOCAL_BUILT_MODULE := $(intermediates)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)

# Full path to final module
LOCAL_FINAL_MODULE := $(TARGET_OUT)/$(LOCAL_MODULE)$(LOCAL_MODULE_SUFFIX)

# Assemble the list of targets to create PRIVATE_ variables for.
LOCAL_INTERMEDIATE_TARGETS += $(LOCAL_BUILT_MODULE)

# Prepend some directories in include list
LOCAL_C_INCLUDES := -I$(intermediates) -I$(TOP_DIR)/$(LOCAL_PATH) $(LOCAL_C_INCLUDES)

# Register the module
ALL_MODULES += $(LOCAL_MODULE)

###############################################################################
## List of sources, objects and libraries.
###############################################################################

cpp_sources := $(filter %.cpp,$(LOCAL_SRC_FILES))
cpp_objects := $(addprefix $(intermediates)/,$(cpp_sources:.cpp=.o))

cxx_sources := $(filter %.cxx,$(LOCAL_SRC_FILES))
cxx_objects := $(addprefix $(intermediates)/,$(cxx_sources:.cxx=.o))

c_sources := $(filter %.c,$(LOCAL_SRC_FILES))
c_objects := $(addprefix $(intermediates)/,$(c_sources:.c=.o))

# rc files are used only under Mingw32
ifeq ("$(OS_MINGW32)","1")
rc_sources := $(filter %.rc,$(LOCAL_SRC_FILES))
rc_objects := $(addprefix $(intermediates)/,$(rc_sources:.rc=.rc.o))
endif

all_objects := \
	$(cpp_objects) \
	$(cxx_objects) \
	$(c_objects) \
	$(rc_objects) \

# Under mingw32, use .dll.a to link against shared libraries instead of .so
ifeq ("$(OS_MINGW32)","1")
  shared_suffix := $(TARGET_SHARED_LIB_SUFFIX)$(TARGET_STATIC_LIB_SUFFIX)
else
  shared_suffix := $(TARGET_SHARED_LIB_SUFFIX)
endif

built_shared_libraries := \
	$(foreach lib,$(LOCAL_SHARED_LIBRARIES), \
		$(TARGET_OUT_INTERMEDIATES)/$(lib)/$(lib)$(shared_suffix))

built_static_libraries := \
	$(foreach lib,$(LOCAL_STATIC_LIBRARIES), \
		$(TARGET_OUT_INTERMEDIATES)/$(lib)/$(lib)$(TARGET_STATIC_LIB_SUFFIX))

# all_libraries is used for the dependencies on LOCAL_BUILT_MODULE.
all_libraries := \
	$(built_shared_libraries) \
	$(built_static_libraries) \

###############################################################################
## Resource list management.
###############################################################################

LOCAL_RESLIST := $(strip $(LOCAL_RESLIST))

ifneq ("$(LOCAL_RESLIST)","")

# This makes sure resource list is created before compilation (order prerequisite)
$(all_objects): | $(intermediates)/ResList

# Additional dependencies
$(intermediates)/ResList.h: $(intermediates)/ResList
$(intermediates)/ResList.c: $(intermediates)/ResList
$(intermediates)/ResList: $(TOP_DIR)/$(LOCAL_PATH)/$(LOCAL_XRC)

reslist := $(TOP_DIR)/$(LOCAL_PATH)/$(LOCAL_RESLIST)
reslistsrc := $(shell dirname $(reslist))
reslistdst := $(intermediates)
reslistmaker := $(reslistdst)/ResListMaker/ResListMaker$(TARGET_EXE_SUFFIX)

# Generate resource list
$(intermediates)/ResList: $(reslist) $(reslistmaker)
	$(call transform-reslist, $(reslistmaker), $(reslistsrc), $(reslistdst))

# Generate resource list maker
$(reslistmaker): $(reslistsrc)/ResListMaker.c
	$(call generate-reslistmaker, $(reslistsrc), $(dir $(reslistmaker)))

endif

###############################################################################
## Actual rules.
###############################################################################

# Final copy (not for static libraries)
$(LOCAL_FINAL_MODULE): $(LOCAL_BUILT_MODULE)
ifeq ("$(LOCAL_BUILDING_STATIC_LIBRARY)","")
	@echo "Copy: $@"
	$(copy-file-to-target)
endif

# cpp files
ifneq ("$(strip $(cpp_objects))","")
$(cpp_objects): $(intermediates)/%.o: $(TOP_DIR)/$(LOCAL_PATH)/%.cpp
	$(transform-cpp-to-o)
-include $(cpp_objects:%.o=%.d)
endif

# cxx files
ifneq ("$(strip $(cxx_objects))","")
$(cxx_objects): $(intermediates)/%.o: $(TOP_DIR)/$(LOCAL_PATH)/%.cxx
	$(transform-cpp-to-o)
-include $(cxx_objects:%.o=%.d)
endif

# c files
ifneq ("$(strip $(c_objects))","")
$(c_objects): $(intermediates)/%.o: $(TOP_DIR)/$(LOCAL_PATH)/%.c
	$(transform-c-to-o)
-include $(c_objects:%.o=%.d)
endif

# rc files
ifneq ("$(strip $(rc_objects))","")
$(rc_objects): $(addprefix $(TOP_DIR)/$(LOCAL_PATH)/,$(LOCAL_RC_DEPS))
$(rc_objects): $(intermediates)/%.rc.o: $(TOP_DIR)/$(LOCAL_PATH)/%.rc
	$(transform-rc-to-o)
endif

# copy files verbatim to target (flat copy in output directory)
copy_files :=
$(foreach f,$(LOCAL_COPY_FILES), \
  $(eval _src := $(TOP_DIR)/$(LOCAL_PATH)/$(f)) \
  $(eval _dst := $(TARGET_OUT)/$(notdir $(f))) \
  $(eval copy_files += $(_dst)) \
  $(eval $(call copy-one-file,$(_src),$(_dst))) \
)
$(LOCAL_BUILT_MODULE): $(copy_files)

# clean- targets
cleantarget := clean-$(LOCAL_MODULE)
$(cleantarget) : PRIVATE_MODULE := $(LOCAL_MODULE)
$(cleantarget) : PRIVATE_CLEAN_FILES := \
	$(PRIVATE_CLEAN_FILES) \
	$(LOCAL_BUILT_MODULE) \
	$(LOCAL_FINAL_MODULE) \
	$(copy_files) \
	$(intermediates)
$(cleantarget)::
	@echo "Clean: $(PRIVATE_MODULE)"
	$(Q)rm -rf $(PRIVATE_CLEAN_FILES)

# install- targets
installtarget := install-$(LOCAL_MODULE)
$(installtarget) : PRIVATE_MODULE := $(LOCAL_MODULE)
$(installtarget) : PRIVATE_FINAL_MODULE := $(LOCAL_FINAL_MODULE)
$(installtarget) : PRIVATE_COPY_FILES := $(copy_files)
$(installtarget) ::
	@echo "Install: $(PRIVATE_MODULE)"
# create directories
	$(Q)install -d $(DESTDIR)$(PREFIX)/bin
	$(Q)install -d $(DESTDIR)$(PREFIX)/lib/$(PACKAGE)
# install executables and shared libraries in lib/<package> directory
ifeq ("$(LOCAL_BUILDING_STATIC_LIBRARY)","")
	$(Q)install $(PRIVATE_FINAL_MODULE) $(DESTDIR)$(PREFIX)/lib/$(PACKAGE)
endif
# install files copied verbatim
ifneq ("$(strip $(copy_files))","")
	$(Q)install $(PRIVATE_COPY_FILES) $(DESTDIR)$(PREFIX)/lib/$(PACKAGE)
endif
# create a link for executables in bin
ifeq ("$(LOCAL_BUILDING_EXECUTABLE)","1")
	$(Q)ln -s ../lib/$(PACKAGE)/$(PRIVATE_MODULE) $(DESTDIR)$(PREFIX)/bin/$(PRIVATE_MODULE)
endif

## Provide a short-hand for building this module.
.PHONY: $(LOCAL_MODULE)
$(LOCAL_MODULE): $(LOCAL_BUILT_MODULE) $(LOCAL_FINAL_MODULE)

###############################################################################
## Precompiled headers.
###############################################################################

LOCAL_PRECOMPILED_FILE := $(strip $(LOCAL_PRECOMPILED_FILE))
ifneq ("$(LOCAL_PRECOMPILED_FILE)","")

gch_file := $(intermediates)/$(LOCAL_PRECOMPILED_FILE).gch

# All objects will depends on the precompiled file
$(all_objects): $(gch_file)

# Generate the precompiled file
$(gch_file): $(TOP_DIR)/$(LOCAL_PATH)/$(LOCAL_PRECOMPILED_FILE)
	$(transform-h-to-gch)
-include $(gch_file:%.gch=%.d)
endif

###############################################################################
# Rule-specific variable definitions.
###############################################################################

$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_PATH := $(LOCAL_PATH)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_MODULE := $(LOCAL_MODULE)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_CFLAGS := $(LOCAL_CFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_C_INCLUDES := $(LOCAL_C_INCLUDES)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_CPPFLAGS := $(LOCAL_CPPFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_RCFLAGS := $(LOCAL_RCFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ARFLAGS := $(LOCAL_ARFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LDFLAGS := $(LOCAL_LDFLAGS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_LDLIBS := $(LOCAL_LDLIBS)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_SHARED_LIBRARIES := $(built_shared_libraries)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_STATIC_LIBRARIES := $(built_static_libraries)
$(LOCAL_INTERMEDIATE_TARGETS): PRIVATE_ALL_OBJECTS := $(all_objects)


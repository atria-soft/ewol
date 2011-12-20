###############################################################################
## @file defs.mk
## @author Y.M. Morgan
## @date 2011/05/14
##
## This file contains macros used by other makefiles.
###############################################################################

###############################################################################
## Figure out where we are.
###############################################################################

define my-dir
$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST),$(MAKEFILE_LIST))))
endef

###############################################################################
## Commands for running gcc to generate a precompiled file.
###############################################################################

define transform-h-to-gch
@mkdir -p $(dir $@)
@echo "Precompile: $(PRIVATE_MODULE) <= $<"
$(Q)$(GXX) \
	$(TARGET_PROJECT_INCLUDES) $(TARGET_C_INCLUDES) $(PRIVATE_C_INCLUDES) \
	$(TARGET_GLOBAL_CFLAGS) $(TARGET_GLOBAL_CPPFLAGS) $(GXX_FLAGS_WARNINGS) \
	$(PRIVATE_CFLAGS) $(PRIVATE_CPPFLAGS) \
	-c -MMD -MP -o $@ $<
endef

###############################################################################
## Commands for running gcc to compile a C++ file.
###############################################################################

define transform-cpp-to-o
@mkdir -p $(dir $@)
@echo "C++: $(PRIVATE_MODULE) <= $<"
$(Q)$(GXX) \
	$(TARGET_PROJECT_INCLUDES) $(TARGET_C_INCLUDES) $(PRIVATE_C_INCLUDES) \
	$(TARGET_GLOBAL_CFLAGS) $(TARGET_GLOBAL_CPPFLAGS) $(GXX_FLAGS_WARNINGS) \
	$(PRIVATE_CFLAGS) $(PRIVATE_CPPFLAGS) \
	-c -MMD -MP -o $@ $<
endef

###############################################################################
## Commands for running gcc to compile a C file.
###############################################################################

define transform-c-to-o
@echo "C: $(PRIVATE_MODULE) <= $<"
@mkdir -p $(dir $@)
$(Q)$(GCC) \
	$(TARGET_PROJECT_INCLUDES) $(TARGET_C_INCLUDES) $(PRIVATE_C_INCLUDES) \
	$(TARGET_GLOBAL_CFLAGS) $(GCC_FLAGS_WARNINGS) \
	$(PRIVATE_CFLAGS) \
	-c -MMD -MP -o $@ $<
endef

###############################################################################
## Commands for compiling rc files.
###############################################################################
define transform-rc-to-o
@echo "RC: $(PRIVATE_MODULE) <= $<"
@mkdir -p $(dir $@)
@rm -f $@
$(Q)$(WINDRES) \
	--use-temp-file -i$< -o$@ -I$(dir $<) \
	$(filter -D%, $(TARGET_GLOBAL_CFLAGS)) \
	$(filter -D%, $(PRIVATE_CFLAGS)) \
	$(TARGET_GLOBAL_RCFLAGS) $(PRIVATE_RCFLAGS)
endef

###############################################################################
## Commands for running ar.
###############################################################################

# Explicitly delete the archive first so that ar doesn't
# try to add to an existing archive.
define transform-o-to-static-lib
@mkdir -p $(dir $@)
@echo "StaticLib: $(PRIVATE_MODULE) ($@)"
@rm -f $@
$(Q)$(AR) $(TARGET_GLOBAL_ARFLAGS) $(PRIVATE_ARFLAGS) $@ $^
endef

###############################################################################
## Commands for running gcc to link a shared library.
###############################################################################

define transform-o-to-shared-lib
@mkdir -p $(dir $@)
@echo "SharedLib: $(PRIVATE_MODULE) ($@)"
$(Q)$(GXX) \
	$(TARGET_GLOBAL_LDFLAGS) \
	-Wl,-Map -Wl,$(basename $@).map \
	-shared \
	-Wl,-soname -Wl,$(notdir $@) \
	-Wl,--unresolved-symbols=ignore-in-shared-libs \
	$(PRIVATE_LDFLAGS) \
	$(PRIVATE_ALL_OBJECTS) \
	-Wl,--no-whole-archive \
	$(PRIVATE_ALL_STATIC_LIBRARIES) \
	$(PRIVATE_ALL_SHARED_LIBRARIES) \
	-o $@ \
	$(PRIVATE_LDLIBS)
endef

###############################################################################
## Commands for running gcc to link an executable.
###############################################################################

define transform-o-to-executable
@mkdir -p $(dir $@)
@echo "Executable: $(PRIVATE_MODULE) ($@)"
$(Q)$(GXX) \
	-Wl,-Map -Wl,$(basename $@).map \
	$(PRIVATE_ALL_OBJECTS) \
	-Wl,--no-whole-archive \
	$(PRIVATE_ALL_STATIC_LIBRARIES) \
	$(PRIVATE_ALL_SHARED_LIBRARIES) \
	$(TARGET_GLOBAL_LDFLAGS) \
	$(PRIVATE_LDFLAGS) \
	-o $@ \
	$(PRIVATE_LDLIBS)
endef

###############################################################################
## Commands to generate resource list
###############################################################################

# $(1) : ResListMaker full path
# $(2) : source directory
# $(3) : destination directory
define transform-reslist
@mkdir -p $(dir $@)
@echo "ResList: $(PRIVATE_MODULE) ($@)"
$(Q)$(1) $(2)/ResList.txt $(3)/ResList.h $(3)/ResList.c $(2)
$(Q)date | cat > $@
endef

# $(1) : source directory
# $(2) : destination directory
define generate-reslistmaker
@mkdir -p $(dir $@)
@echo "ResListMaker: $(PRIVATE_MODULE) ($@)"
$(Q)$(GCC) -O2 -g0 -c -o $(2)/ResListMaker.o $(1)/ResListMaker.c
$(Q)$(GCC) -o $@ $(2)/ResListMaker.o
endef

###############################################################################
## Commands for copying files.
###############################################################################

# Copy a single file from one place to another, preserving permissions and
# overwriting any existing file.
define copy-file-to-target
@mkdir -p $(dir $@)
$(Q)cp -fp $< $@
endef

# Define a rule to copy a file.  For use via $(eval).
# $(1): source file
# $(2): destination file
define copy-one-file
$(2): $(1)
	@echo "Copy: $$@"
	$$(copy-file-to-target)
endef



###############################################################################
## Commands for importing module files.
###############################################################################
import-module = \
	$(eval include $(shell find $(PROJECT_MODULE)$1 -name Linux.mk))

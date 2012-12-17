
# get the local dir in a good form :
TMP_DIR := $(shell readlink -m -n $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST)))))


# Add ewol packages
EWOL_FOLDER:=$(TMP_DIR)
USER_PACKAGES+= $(TMP_DIR)/sources/
# Add external librairy (direct folder to reduce the search time (really special case for windows ...))
USER_PACKAGES+= $(TMP_DIR)/external/etk
USER_PACKAGES+= $(TMP_DIR)/external/agg
USER_PACKAGES+= $(TMP_DIR)/external/freetype
USER_PACKAGES+= $(TMP_DIR)/external/glew
USER_PACKAGES+= $(TMP_DIR)/external/lua
USER_PACKAGES+= $(TMP_DIR)/external/ogg
USER_PACKAGES+= $(TMP_DIR)/external/parsersvg
USER_PACKAGES+= $(TMP_DIR)/external/png
USER_PACKAGES+= $(TMP_DIR)/external/portaudio
USER_PACKAGES+= $(TMP_DIR)/external/tinyxml
USER_PACKAGES+= $(TMP_DIR)/external/z
USER_PACKAGES+= $(TMP_DIR)/external/zip

#include te generic toolchain :
include $(TMP_DIR)/build/Makefile.mk

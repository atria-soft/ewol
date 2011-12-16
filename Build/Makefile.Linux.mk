



include $(EWOL_FOLDER)/Build/Makefile.common.mk

#include $(EWOL_FOLDER)/Build/ewol.mk

###############################################################################
### Compilateur base system                                                 ###
###############################################################################
CXX=g++
CC=gcc
AR=ar



X11FLAGS= -lGL -lGLU
# some X11 mode availlable : 
# install package : libxxf86vm-dev
X11FLAGS+= -lX11 -DEWOL_X11_MODE__XF86V -lXxf86vm
# install package : libxrender-dev 
#X11FLAGS+= -lX11 -DEWOL_X11_MODE__XRENDER -lXrandr


ifeq ($(shell if `pkg-config --exists freetype2` ; then echo "yes"; else echo "no"; fi), yes)
    FREETYPE_CFLAGS=  `pkg-config --cflags freetype2` -DEWOL_USE_FREE_TYPE
    FREETYPE_LDFLAGS= `pkg-config --libs freetype2` -DEWOL_USE_FREE_TYPE
else
    FREETYPE_CFLAGS=
    FREETYPE_LDFLAGS=
    $(Info  libFreeType-dev is not installed)
endif



OBJECT_DIR=Object_$(PLATFORM)

ifeq ("$(DEBUG)", "0")
    OBJECT_DIRECTORY=$(OBJECT_DIR)/release
    OUTPUT_NAME = $(OUTPUT_NAME_RELEASE)
else
    OBJECT_DIRECTORY=$(OBJECT_DIR)/debug
    OUTPUT_NAME = $(OUTPUT_NAME_DEBUG)
endif



###############################################################################
### Basic C flags                                                           ###
###############################################################################

# basic X11 librairy ==> show if we can une under lib ...
CXXFLAGS=  $(X11FLAGS) $(FREETYPE_CFLAGS) -D__PLATFORM__$(PLATFORM) $(PROJECT_CXXFLAGS)

ifeq ("$(DEBUG)", "0")
	CXXFLAGS+= -O2
else
	# Enable debug (cgdb ***)
	CXXFLAGS+= -g -O0
endif
# display all flags
CXXFLAGS+= -Wall
# ...
CXXFLAGS+= -D_REENTRANT
# internal defines
CXXFLAGS+= $(DEFINE)
# remove warning from the convertion char*
CXXFLAGS+= -Wno-write-strings

CXXFLAGS+= -DPROJECT_NAME="$(PROJECT_NAME)"

CFLAGS=    $(CXXFLAGS) -std=c99

# basic extern librairy
LDFLAGS=  $(X11FLAGS) $(FREETYPE_LDFLAGS) $(PROJECT_LDFLAGS)

# Dynamic connection of the CALLBACK of the GUI
LDFLAGS+= -Wl,--export-dynamic

# TODO : add the prefix ...
CXXFILES = $(PROJECT_SOURCES)
# create the list of module : 
LISTE_MODULES = $(sort $(dir $(CXXFILES)))

###############################################################################
### Build Object Files List                                                 ###
###############################################################################
OBJ = $(addprefix $(OBJECT_DIRECTORY)/, $(CXXFILES:.cpp=.o))


###############################################################################
### Main Part of Makefile                                                   ###
###############################################################################
all: $(PROJECT_NAME)

-include $(OBJ:.o=.d)

.ewol:
	@cd $(EWOL_FOLDER) ; make -s PLATFORM=$(PLATFORM) DEBUG=$(DEBUG)
	
.ewol-clean:
	@cd $(EWOL_FOLDER) ; make -s PLATFORM=$(PLATFORM) clean

.encadrer:
	@echo $(CADRE_HAUT_BAS)
	@echo $(CADRE_COTERS)
	@echo '           DEBUT DE COMPILATION DU PROGRAMME :'$(CADRE_COTERS)
	@echo '             Repertoire Sources : $(PROJECT_FILE_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Repertoire object  : $(OBJECT_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Binaire de sortie  : $(F_VIOLET)$(PROJECT_NAME) $(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@mkdir -p $(addprefix $(OBJECT_DIRECTORY)/, $(LISTE_MODULES))

$(info PROJECT_FILE_DIRECTORY=$(PROJECT_FILE_DIRECTORY) ; OBJECT_DIRECTORY=$(OBJECT_DIRECTORY))

# build C++
$(OBJECT_DIRECTORY)/%.o: $(PROJECT_FILE_DIRECTORY)/%.cpp $(MAKE_DEPENDENCE)
	@echo $(F_VERT)"          (.o)  $<"$(F_NORMALE)
	@$(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD

# build binary Release Mode
$(PROJECT_NAME): .ewol .encadrer $(PROJECT_LIB_DEPENDENCY) $(OBJ) $(MAKE_DEPENDENCE)
	@echo $(F_ROUGE)"          (bin) $(PROJECT_NAME) "$(F_NORMALE)
	@$(CXX) $(OBJ) $(EWOL_FOLDER)/libewol.a $(LDFLAGS) -o $@



clean: .ewol-clean 
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : $(F_VIOLET)$(OUTPUT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@echo $(F_VERT)"          (rm)  $(OBJECT_DIR)"$(F_NORMALE)
	@rm -rf $(OBJECT_DIR) 
	@echo $(F_VERT)"          (rm)  $(PROJECT_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)"$(F_NORMALE)
	@rm -f $(PROJECT_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)
	@echo $(F_VERT)"          (rm)  doxygen/*"$(F_NORMALE)
	@rm -rf doxygen
	@rm -f doxygen.log
	@echo $(F_VERT)"          (rm)  *.bck"$(F_NORMALE)
	@rm -f `find . -name "*.bck"`


count:
	wc -l Makefile `find $(FILE_DIRECTORY)/ -name "*.cpp"` `find $(PROJECT_FILE_DIRECTORY)/ -name "*.h"` 


install: .encadrer .versionFile $(PROJECT_NAME)
	@echo '	INSTALL : $(OUTPUT_NAME_RELEASE)=>$(PROJECT_NAME)'
	@echo "	(stripped) $(OUTPUT_NAME_RELEASE) => $(PROJECT_NAME) "
	@cp $(OUTPUT_NAME_RELEASE) $(PROJECT_NAME)
	@strip -s $(PROJECT_NAME)
	@echo "	(copy) $(PROJECT_NAME) /usr/bin/ "
	@cp -vf $(PROJECT_NAME) /usr/bin/
	@echo "	(data) data/* ==> /usr/share/$(PROJECT_NAME)/ "
	@mkdir -p /usr/share/$(PROJECT_NAME)/
	@rm -rf /usr/share/$(PROJECT_NAME)/*
	@cp -vf assets/* /usr/share/$(PROJECT_NAME)/


# http://alp.developpez.com/tutoriels/debian/creer-paquet/
package: .encadrer
	@echo 'Create Folders ...'
	@mkdir -p package/$(PROJECT_NAME)/DEBIAN/
	@mkdir -p package/$(PROJECT_NAME)/usr/bin/
	@mkdir -p package/$(PROJECT_NAME)/usr/share/doc/
	@mkdir -p package/$(PROJECT_NAME)/usr/share/edn/
	# Create the control file
	@echo "Package: "$(PROJECT_NAME) > package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Version: "$(VERSION_TAG_SHORT) >> package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Section: Development,Editors" >> package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Priority: optional" >>package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Architecture: all" >> package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Depends: bash" >> package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Maintainer: Mr DUPIN Edouard <yui.heero@gmail.com>" >> package/$(PROJECT_NAME)/DEBIAN/control
	@echo "Description: Text editor for sources code with ctags management" >> package/$(PROJECT_NAME)/DEBIAN/control
	@echo "" >> package/$(PROJECT_NAME)/DEBIAN/control
	# Create the PostRm
	@echo "#!/bin/bash" > package/$(PROJECT_NAME)/DEBIAN/postrm
	@echo "rm ~/."$(PROJECT_NAME) >> package/$(PROJECT_NAME)/DEBIAN/postrm
	@echo "" >> package/$(PROJECT_NAME)/DEBIAN/postrm
	# Enable Execution in script
	@chmod 755 package/$(PROJECT_NAME)/DEBIAN/post*
	@#chmod 755 package/$(PROJECT_NAME)/DEBIAN/pre*
	# copy licence and information : 
	@cp README package/$(PROJECT_NAME)/usr/share/doc/README
	@cp licence.txt package/$(PROJECT_NAME)/usr/share/doc/copyright
	@echo "First generation in progress" >> package/$(PROJECT_NAME)/usr/share/doc/changelog
	@cp -vf $(PROJECT_NAME) package/$(PROJECT_NAME)/usr/bin/
	@cp -vf data/*.xml package/$(PROJECT_NAME)/usr/share/edn/
	@cd package; dpkg-deb --build $(PROJECT_NAME)



##################################################################################################################
#                                                                                                                #
#   Fichier     :   Makefile                                                                                     #
#                                                                                                                #
#   Type        :   Makefile d'un programme complet                                                              #
#                                                                                                                #
#   Auteur      :   Heero Yui                                                                                    #
#                                                                                                                #
#   Evolutions  :   Date          Auteur        Raison                                                           #
#                2010-01-29      Heero Yui      Mise en place d'un makefile ultra simple                         #
#                2011-07-14      Heero Yui      Rework the current dorder includion (simplification)             #
#                                                                                                                #
#   Notes       :   This makefile might be edited with an editor compatible with escape char and carrer return   #
#                   char                                                                                         #
#                                                                                                                #
# Concu Pour le projet ewol                                                                                      #
#                                                                                                                #
##################################################################################################################
export F_GRAS=[1m
export F_INVERSER=[7m
export F_SOUSLIGNER=[4m
export F_NORMALE=[m
export F_NOIR=[31m
export F_ROUGE=[31m
export F_VERT=[32m
export F_MARRON=[33m
export F_BLUE=[34m
export F_VIOLET=[35m
export F_CYAN=[36m
export F_GRIS=[37m
export CADRE_HAUT_BAS='	$(F_INVERSER)                                                                    $(F_NORMALE)'
export CADRE_COTERS='	$(F_INVERSER)  $(F_NORMALE)								  $(F_INVERSER)  $(F_NORMALE)'

VERSION_TAG=$(shell git describe --tags)
#$(info $(VERSION_TAG))

VERSION_TAG_SHORT=$(shell git describe --tags --abbrev=0)
#$(info $(VERSION_TAG_SHORT))

VERSION_BUILD_TIME=$(shell date)
#$(info $(VERSION_BUILD_TIME))

###############################################################################
### Platform specificity :                                                  ###
###############################################################################
SUPPORTED_PLATFORM=X11 DoubleBuffer IPhone IPad Android AndroidTablet
DEFAULT_PLATFORM=X11

# default platform can be overridden
PLATFORM?=$(DEFAULT_PLATFORM)

ifeq ($(PLATFORM), X11)
    CXXFILES += base/guiX11.cpp
else ifeq ($(PLATFORM), DoubleBuffer)
    CXXFILES += base/guiDoubleBuffer.cpp
else ifeq ($(PLATFORM), IPhone)
    CXXFILES += base/guiIPhone.cpp
else ifeq ($(PLATFORM), IPad)
    CXXFILES += base/guiIPad.cpp
else ifeq ($(PLATFORM), Android)
    CXXFILES += base/guiAndroid.cpp
else ifeq ($(PLATFORM), AndroidTablet)
    CXXFILES += base/guiAndroidTablet.cpp
else
    $(error you must specify a corect platform : make PLATFORM=$(SUPPORTED_PLATFORM))
endif

$(info Build for $(PLATFORM))

###############################################################################
### Compilateur base system                                                 ###
###############################################################################
CXX=g++
CC=gcc
AR=ar

DEBUG:=1

###############################################################################
### Compilation Define                                                      ###
###############################################################################
ifeq ("$(DEBUG)", "0")
    DEFINE = -DETK_DEBUG_LEVEL=1
    DEFINE+= -DEWOL_DEBUG_LEVEL=1
    DEFINE+= -DNDEBUG
    DEFINE+= -DEWOL_VERSION_TAG_NAME="\"$(VERSION_TAG)-release\""
else
    DEFINE = -DETK_DEBUG_LEVEL=3
    DEFINE+= -DEWOL_DEBUG_LEVEL=3
    DEFINE+= -DEWOL_VERSION_TAG_NAME="\"$(VERSION_TAG)-debug\""
endif
DEFINE+= -DVERSION_BUILD_TIME="\"$(VERSION_BUILD_TIME)\""

X11FLAGS= -lX11 -lGL -lGLU
# some X11 mode availlable : 
X11FLAGS+= -DEWOL_X11_MODE__XF86V -lXxf86vm
#X11FLAGS+= -DEWOL_X11_MODE__XRENDER -lXrandr

###############################################################################
### Basic C flags                                                           ###
###############################################################################

# basic X11 librairy ==> show if we can une under lib ...
CXXFLAGS=  $(X11FLAGS) -D__PLATFORM__=$(PLATFORM)

ifeq ("$(DEBUG)", "0")
	# Enable debug (cgdb ***)
	CXXFLAGS+= -g -O0
else
	CXXFLAGS+= -O2
endif
# display all flags
CXXFLAGS+= -Wall
# ...
CXXFLAGS+= -D_REENTRANT
# internal defines
CXXFLAGS+= $(DEFINE)

CFLAGS=    $(CXXFLAGS) -std=c99

# basic X11 librairy
LDFLAGS=  $(X11FLAGS)

# Dynamic connection of the CALLBACK of the GUI
LDFLAGS+= -Wl,--export-dynamic

###############################################################################
### Project Name                                                            ###
###############################################################################
PROG_NAME=ewol

###############################################################################
### Basic Project description Files                                         ###
###############################################################################
FILE_DIRECTORY=Sources
OUTPUT_NAME_RELEASE=$(PROG_NAME)_release
OUTPUT_NAME_DEBUG=$(PROG_NAME)_debug
OBJECT_DIR=Object_$(PLATFORM)

ifeq ("$(DEBUG)", "0")
    OBJECT_DIRECTORY=$(OBJECT_DIR)/release
    OUTPUT_NAME = $(OUTPUT_NAME_RELEASE)
else
    OBJECT_DIRECTORY=$(OBJECT_DIR)/debug
    OUTPUT_NAME = $(OUTPUT_NAME_DEBUG)
endif



###############################################################################
### Generique dependency                                                    ###
###############################################################################
MAKE_DEPENDENCE=Makefile

###############################################################################
### Files Listes                                                            ###
###############################################################################

# Ewol Tool Kit :
CXXFILES +=		etk/etkDebug.cpp \
				etk/etkDebugInternal.cpp \
				etk/etkMemory.cpp \
				etk/etkString.cpp \
				etk/etkFile.cpp \
				etk/etkRegExp.cpp

# Ewol Sources :
CXXFILES +=		ewol.cpp \
				ewolDebug.cpp \
				ewolOObject.cpp \
				ewolTexture.cpp \
				ewolFont.cpp \
				ewolWidget.cpp \
				ewolWindows.cpp


# Ewol Test Software :
CXXFILES +=		Main.cpp



###############################################################################
### Liste of folder where .h can be                                         ###
###############################################################################
LISTE_MODULES = $(sort $(dir $(CXXFILES)))
#$(info listeModule=$(LISTE_MODULES))
INCLUDE_DIRECTORY = $(addprefix -I$(FILE_DIRECTORY)/, $(LISTE_MODULES)) 

###############################################################################
### Build Object Files List                                                 ###
###############################################################################
OBJ =	$(addprefix $(OBJECT_DIRECTORY)/, $(CXXFILES:.cpp=.o))



###############################################################################
### Main Part of Makefile                                                   ###
###############################################################################
all: build

-include $(OBJ:.o=.d)

build: .encadrer .versionFile $(OUTPUT_NAME)

.encadrer:
	@echo $(CADRE_HAUT_BAS)
	@echo $(CADRE_COTERS)
	@echo '           DEBUT DE COMPILATION DU PROGRAMME :'$(CADRE_COTERS)
	@echo '             Repertoire Sources : $(FILE_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Repertoire object  : $(OBJECT_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Binaire de sortie  : $(F_VIOLET)$(OUTPUT_NAME) $(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@mkdir -p $(addprefix $(OBJECT_DIRECTORY)/, $(LISTE_MODULES))

.versionFile:
	@rm -f $(OBJECT_DIRECTORY)/ewol.o

# build C++
$(OBJECT_DIRECTORY)/%.o: $(FILE_DIRECTORY)/%.cpp $(MAKE_DEPENDENCE)
	@echo $(F_VERT)"          (.o)  $<"$(F_NORMALE)
	@$(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD

# build binary Release Mode
$(OUTPUT_NAME_RELEASE): $(OBJ) $(MAKE_DEPENDENCE)
	@echo $(F_ROUGE)"          (bin) $@ "$(F_NORMALE)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@cp $@ $(PROG_NAME)

# build binary Debug Mode
$(OUTPUT_NAME_DEBUG): $(OBJ) $(MAKE_DEPENDENCE)
	@echo $(F_ROUGE)"          (bin) $@ "$(F_NORMALE)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@cp $@ $(PROG_NAME)

clean:
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : $(F_VIOLET)$(OUTPUT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@echo Remove Folder : $(OBJECT_DIR)
	@rm -rf $(OBJECT_DIR) 
	@echo Remove File : $(PROG_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)
	@rm -f $(PROG_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)
	@echo Remove File : pngToCpp
	@rm -f pngToCpp
	@echo Remove File : $(FILE_DIRECTORY)/GuiTools/myImage.*
	@rm -f $(FILE_DIRECTORY)/GuiTools/myImage.*
	@echo Remove doxygen  files : doxygen/*
	@rm -rf doxygen
	@rm -f doxygen.log
	@echo Remove temporary files *.bck
	@rm -f `find . -name "*.bck"`

count:
	wc -l Makefile `find $(FILE_DIRECTORY)/ -name "*.cpp"` `find $(FILE_DIRECTORY)/ -name "*.h"` 

install: .encadrer .versionFile $(OUTPUT_NAME_RELEASE)
	@echo $(CADRE_HAUT_BAS)
	@echo '           INSTALL : $(F_VIOLET)$(OUTPUT_NAME_RELEASE)=>$(PROG_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@echo $(F_ROUGE)"          (stripped) $(OUTPUT_NAME_RELEASE) => $(PROG_NAME) "$(F_NORMALE)
	@cp $(OUTPUT_NAME_RELEASE) $(PROG_NAME)
	@strip -s $(PROG_NAME)
	@echo $(F_VERT)"          (copy) $(PROG_NAME) /usr/bin/ "$(F_NORMALE)
	@cp -vf $(PROG_NAME) /usr/bin/
	@echo $(F_VERT)"          (data) data/* ==> /usr/share/edn/ "$(F_NORMALE)
	@mkdir -p /usr/share/edn/
	@rm -rf /usr/share/edn/*
	@cp -vf data/*.xml /usr/share/edn/
	@mkdir -p /usr/share/edn/images/
	@cp -vf data/imagesSources/icone.png /usr/share/edn/images/
	@cp -vf data/imagesSources/delete-24px.png /usr/share/edn/images/


# http://alp.developpez.com/tutoriels/debian/creer-paquet/
package: .encadrer
	@echo 'Create Folders ...'
	@mkdir -p package/$(PROG_NAME)/DEBIAN/
	@mkdir -p package/$(PROG_NAME)/usr/bin/
	@mkdir -p package/$(PROG_NAME)/usr/share/doc/
	@mkdir -p package/$(PROG_NAME)/usr/share/edn/
	# Create the control file
	@echo "Package: "$(PROG_NAME) > package/$(PROG_NAME)/DEBIAN/control
	@echo "Version: "$(VERSION_TAG_SHORT) >> package/$(PROG_NAME)/DEBIAN/control
	@echo "Section: Development,Editors" >> package/$(PROG_NAME)/DEBIAN/control
	@echo "Priority: optional" >>package/$(PROG_NAME)/DEBIAN/control
	@echo "Architecture: all" >> package/$(PROG_NAME)/DEBIAN/control
	@echo "Depends: bash" >> package/$(PROG_NAME)/DEBIAN/control
	@echo "Maintainer: Mr DUPIN Edouard <yui.heero@gmail.com>" >> package/$(PROG_NAME)/DEBIAN/control
	@echo "Description: Text editor for sources code with ctags management" >> package/$(PROG_NAME)/DEBIAN/control
	@echo "" >> package/$(PROG_NAME)/DEBIAN/control
	# Create the PostRm
	@echo "#!/bin/bash" > package/$(PROG_NAME)/DEBIAN/postrm
	@echo "rm ~/."$(PROG_NAME) >> package/$(PROG_NAME)/DEBIAN/postrm
	@echo "" >> package/$(PROG_NAME)/DEBIAN/postrm
	# Enable Execution in script
	@chmod 755 package/$(PROG_NAME)/DEBIAN/post*
	@#chmod 755 package/$(PROG_NAME)/DEBIAN/pre*
	# copy licence and information : 
	@cp README package/$(PROG_NAME)/usr/share/doc/README
	@cp licence.txt package/$(PROG_NAME)/usr/share/doc/copyright
	@echo "First generation in progress" >> package/$(PROG_NAME)/usr/share/doc/changelog
	@cp -vf $(PROG_NAME) package/$(PROG_NAME)/usr/bin/
	@cp -vf data/*.xml package/$(PROG_NAME)/usr/share/edn/
	@cd package; dpkg-deb --build $(PROG_NAME)



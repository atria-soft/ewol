



include $(EWOL_FOLDER)/Build/Makefile.common.mk

include $(EWOL_FOLDER)/Build/ewol.mk

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


###############################################################################
### Basic C flags                                                           ###
###############################################################################

# basic X11 librairy ==> show if we can une under lib ...
CXXFLAGS=  $(X11FLAGS) $(FREETYPE_CFLAGS) -D__PLATFORM__$(PLATFORM)

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

CFLAGS=    $(CXXFLAGS) -std=c99

# basic extern librairy
LDFLAGS=  $(X11FLAGS) $(FREETYPE_LDFLAGS)

# Dynamic connection of the CALLBACK of the GUI
LDFLAGS+= -Wl,--export-dynamic

# TODO : add the prefix ...
CXXFILES =  $(addprefix $(EWOL_FOLDER)/Sources/, $(EWOL_CXXFILES))  $(PROJECT_SOURCES)


###############################################################################
### Basic Project description Files                                         ###
###############################################################################
FILE_DIRECTORY=Sources
OUTPUT_NAME_RELEASE=$(PROJECT_NAME)_release
OUTPUT_NAME_DEBUG=$(PROJECT_NAME)_debug
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


# get all data file in the specific folder
DATA_FOLDER=dataTest
DATA_FILE=$(shell find $(DATA_FOLDER)/*)

###############################################################################
### Liste of folder where .h can be                                         ###
###############################################################################
LISTE_MODULES = $(sort $(dir $(CXXFILES)))
#$(info listeModule=$(LISTE_MODULES))
INCLUDE_DIRECTORY = $(addprefix -I$(FILE_DIRECTORY)/, $(LISTE_MODULES)) 
# overwrite if needed the directory folder : 
INCLUDE_DIRECTORY = -I$(FILE_DIRECTORY)/

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
	@rm -f $(OBJECT_DIRECTORY)/ewol/ewol.o

# build C++
$(OBJECT_DIRECTORY)/%.o: $(FILE_DIRECTORY)/%.cpp $(MAKE_DEPENDENCE)
	@echo $(F_VERT)"          (.o)  $<"$(F_NORMALE)
	@$(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD

# build binary Release Mode
$(OUTPUT_NAME_RELEASE): $(OBJ) $(MAKE_DEPENDENCE)
	@echo $(F_ROUGE)"          (bin) $@ "$(F_NORMALE)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@cp $@ $(PROJECT_NAME)

# build binary Debug Mode
ifeq ($(PLATFORM), Android)
$(OUTPUT_NAME_DEBUG): $(MAKE_DEPENDENCE)
	cd $(PROJECT_NDK) ; NDK_PROJECT_PATH=$(shell pwd) ./ndk-build
	# V=1
	PATH=$(PROJECT_SDK)/tools/:$(PROJECT_SDK)/platform-tools/:$(PATH) ant -Dsdk.dir=$(PROJECT_SDK) debug
else
$(OUTPUT_NAME_DEBUG): $(OBJ) $(MAKE_DEPENDENCE)
	@echo $(F_ROUGE)"          (bin) $@ "$(F_NORMALE)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@cp $@ $(PROJECT_NAME)
endif


clean:
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : $(F_VIOLET)$(OUTPUT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
ifeq ($(PLATFORM), Android)
	rm -rf bin libs gen obj
else 
	@echo Remove Folder : $(OBJECT_DIR)
	@rm -rf $(OBJECT_DIR) 
	@echo Remove File : $(PROJECT_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)
	@rm -f $(PROJECT_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)
	@echo Remove File : pngToCpp
	@rm -f pngToCpp
	@echo Remove File : $(FILE_DIRECTORY)/GuiTools/myImage.*
	@rm -f $(FILE_DIRECTORY)/GuiTools/myImage.*
	@echo Remove doxygen  files : doxygen/*
	@rm -rf doxygen
	@rm -f doxygen.log
	@echo Remove temporary files *.bck
	@rm -f `find . -name "*.bck"`
endif

count:
	wc -l Makefile `find $(FILE_DIRECTORY)/ -name "*.cpp"` `find $(FILE_DIRECTORY)/ -name "*.h"` 

ifeq ($(PLATFORM), Android)
install:
	sudo $(PROJECT_SDK)/platform-tools/adb  install -r ./bin/EwolActivity-debug.apk
else
install: .encadrer .versionFile $(OUTPUT_NAME_RELEASE)
	@echo $(CADRE_HAUT_BAS)
	@echo '           INSTALL : $(F_VIOLET)$(OUTPUT_NAME_RELEASE)=>$(PROJECT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@echo $(F_ROUGE)"          (stripped) $(OUTPUT_NAME_RELEASE) => $(PROJECT_NAME) "$(F_NORMALE)
	@cp $(OUTPUT_NAME_RELEASE) $(PROJECT_NAME)
	@strip -s $(PROJECT_NAME)
	@echo $(F_VERT)"          (copy) $(PROJECT_NAME) /usr/bin/ "$(F_NORMALE)
	@cp -vf $(PROJECT_NAME) /usr/bin/
	@echo $(F_VERT)"          (data) data/* ==> /usr/share/edn/ "$(F_NORMALE)
	@mkdir -p /usr/share/edn/
	@rm -rf /usr/share/edn/*
	@cp -vf data/*.xml /usr/share/edn/
	@mkdir -p /usr/share/edn/images/
	@cp -vf data/imagesSources/icone.png /usr/share/edn/images/
	@cp -vf data/imagesSources/delete-24px.png /usr/share/edn/images/
endif

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



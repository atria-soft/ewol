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

PLATFORM?=Linux
CXXFILES += base/guiX11.cpp

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
### Android Area                                                            ###
###############################################################################
ifeq ($(PLATFORM), Android)
    PROJECT_NDK=$(realpath ../android/ndk-r7/)
    PROJECT_SDK=$(realpath ../android/sdk-r15/)
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

#data File of the program :
ifeq ($(DATA_MODE), MEMORY)
CXXFILES +=		GeneratedData.cpp
endif


# tiny XML (extern OPEN Sources) :
CXXFILES +=		tinyXML/tinyxml.cpp \
				tinyXML/tinyxmlparser.cpp \
				tinyXML/tinyxmlerror.cpp \
				tinyXML/tinystr.cpp

# Ewol Tool Kit :
CXXFILES +=		etk/Debug.cpp \
				etk/DebugInternal.cpp \
				etk/Memory.cpp \
				etk/String.cpp \
				etk/Stream.cpp \
				etk/File.cpp \
				etk/RegExp.cpp

# Ewol Sources :
CXXFILES +=		ewol/ewol.cpp \
				ewol/Debug.cpp \
				ewol/OObject.cpp \
				ewol/OObject/2DText.cpp \
				ewol/OObject/2DColored.cpp \
				ewol/OObject/2DTextured.cpp \
				ewol/Texture.cpp
ifeq ("$(FREETYPE_CFLAGS)", "")
    CXXFILES +=	ewol/FontBitmap.cpp
else
    CXXFILES +=	ewol/FontFreeType.cpp
endif
CXXFILES +=		ewol/Widget.cpp \
				ewol/WidgetManager.cpp \
				ewol/Windows.cpp
# list of widgets :
CXXFILES +=		ewol/widget/Button.cpp \
				ewol/widget/CheckBox.cpp \
				ewol/widget/Entry.cpp \
				ewol/widget/List.cpp \
				ewol/widget/SizerHori.cpp \
				ewol/widget/SizerVert.cpp
# only to test  son internal element ...
CXXFILES +=		ewol/widget/Test.cpp
# theme management :
CXXFILES +=		ewol/themeManager.cpp \
				ewol/theme/Theme.cpp \
				ewol/theme/EolElement.cpp \
				ewol/theme/EolElementFrame.cpp \
				ewol/theme/EolColor.cpp \
				ewol/theme/EolBase.cpp \
				ewol/theme/EolBaseCircle.cpp \
				ewol/theme/EolBaseCirclePart.cpp \
				ewol/theme/EolBaseLine.cpp \
				ewol/theme/EolBasePolygone.cpp \
				ewol/theme/EolBaseRect.cpp \
				ewol/theme/EolBaseTriangle.cpp



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

build : .encadrer .versionFile $(OBJ) $(MAKE_DEPENDENCE)
	@echo $(F_ROUGE)"          (.a) lib$(PROG_NAME).a "$(F_NORMALE)
	@ar rcs lib$(PROG_NAME).a $(OBJ)


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


clean:
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : $(F_VIOLET)$(OUTPUT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@echo $(F_VERT)"          (rm)  $(OBJECT_DIR)"$(F_NORMALE)
	@rm -rf $(OBJECT_DIR) 
	@echo $(F_VERT)"          (rm)  $(PROJECT_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)"$(F_NORMALE)
	@rm -f $(PROG_NAME) $(OUTPUT_NAME_DEBUG) $(OUTPUT_NAME_RELEASE)
	@echo $(F_VERT)"          (rm)  doxygen/*"$(F_NORMALE)
	@rm -rf doxygen
	@rm -f doxygen.log
	@echo $(F_VERT)"          (rm)  *.bck"$(F_NORMALE)
	@rm -f `find . -name "*.bck"`


count:
	wc -l Makefile `find $(FILE_DIRECTORY)/ -name "*.cpp"` `find $(FILE_DIRECTORY)/ -name "*.h"` 

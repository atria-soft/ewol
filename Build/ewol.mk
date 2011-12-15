

EWOL_VERSION_TAG=$(shell cd $(EWOL_FOLDER); git describe --tags)
#$(info $(VERSION_TAG))

EWOL_VERSION_TAG_SHORT=$(shell cd $(EWOL_FOLDER); git describe --tags --abbrev=0)
#$(info $(VERSION_TAG_SHORT))

###############################################################################
### Compilation Define                                                      ###
###############################################################################
ifeq ("$(DEBUG)", "0")
    EWOL_CXXFLAGS = -DETK_DEBUG_LEVEL=1
    EWOL_CXXFLAGS+= -DEWOL_DEBUG_LEVEL=1
    EWOL_CXXFLAGS+= -DNDEBUG
    EWOL_CXXFLAGS+= -DEWOL_VERSION_TAG_NAME="\"$(EWOL_VERSION_TAG)-release\""
else
    EWOL_CXXFLAGS = -DETK_DEBUG_LEVEL=3
    EWOL_CXXFLAGS+= -DEWOL_DEBUG_LEVEL=3
    EWOL_CXXFLAGS+= -DEWOL_VERSION_TAG_NAME="\"$(EWOL_VERSION_TAG)-debug\""
endif
EWOL_CXXFLAGS+= -DVERSION_BUILD_TIME="\"$(BUILD_TIME)\""



# tiny XML (extern OPEN Sources) :
EWOL_CXXFILES +=		tinyXML/tinyxml.cpp \
						tinyXML/tinyxmlparser.cpp \
						tinyXML/tinyxmlerror.cpp \
						tinyXML/tinystr.cpp

# Ewol Tool Kit :
EWOL_CXXFILES +=		etk/Debug.cpp \
						etk/DebugInternal.cpp \
						etk/Memory.cpp \
						etk/String.cpp \
						etk/Stream.cpp \
						etk/File.cpp \
						etk/RegExp.cpp

# Ewol Sources :
EWOL_CXXFILES +=		ewol/ewol.cpp \
						ewol/Debug.cpp \
						ewol/OObject.cpp \
						ewol/OObject/2DText.cpp \
						ewol/OObject/2DColored.cpp \
						ewol/OObject/2DTextured.cpp \
						ewol/Texture.cpp
ifeq ("$(FREETYPE_CFLAGS)", "")
    EWOL_CXXFILES +=	ewol/FontBitmap.cpp
else
    EWOL_CXXFILES +=	ewol/FontFreeType.cpp
endif
EWOL_CXXFILES +=		ewol/Widget.cpp \
						ewol/WidgetManager.cpp \
						ewol/Windows.cpp
# list of widgets :
EWOL_CXXFILES +=		ewol/widget/Button.cpp \
						ewol/widget/CheckBox.cpp \
						ewol/widget/Entry.cpp \
						ewol/widget/List.cpp \
						ewol/widget/SizerHori.cpp \
						ewol/widget/SizerVert.cpp
# only to test  son internal element ...
EWOL_CXXFILES +=		ewol/widget/Test.cpp
# theme management :
EWOL_CXXFILES +=		ewol/themeManager.cpp \
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


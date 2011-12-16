##################################################################################################################
#                                                                                                                #
#   Fichier     :   Makefile.common                                                                              #
#                                                                                                                #
#   Type        :   common part of the ewol Makefile                                                             #
#                                                                                                                #
#   Auteur      :   Heero Yui                                                                                    #
#                                                                                                                #
#   Evolutions  :   Date          Auteur        Raison                                                           #
#                2011-12-15      Heero Yui      Mise en place du système de couleur et d'encart                  #
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

###############################################################################
### Platform specificity :                                                  ###
###############################################################################
SUPPORTED_PLATFORM=Linux IMac IPhone IPad Android AndroidTablet
DEFAULT_PLATFORM=Linux

# default platform can be overridden
PLATFORM?=$(DEFAULT_PLATFORM)

ifeq ($(PLATFORM), Linux)
    CXXFILES += base/guiX11.cpp
else ifeq ($(PLATFORM), IMac)
    CXXFILES += base/guiIMac.cpp
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


BUILD_TIME=$(shell date)



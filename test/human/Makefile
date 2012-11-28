#############################################################################"
# main makefile for a basic application
# this is minimal application main makefile
#############################################################################"

# Project name ==> generate the application name [a-z] <== make attention at the case this generate error on Android compilation
PROJECT_NAME=human

#Can be manny things, but limit whith no  space no special char and no Maj ... [a-z]
#	com : Commercial
#	net : Network??
#	org : Organisation
#	gov : Governement
#	mil : Military
#	edu : Education
#	pri : Private
#	museum : ...
PROJECT_COMPAGNY_TYPE=org

# Compagny name of the project [a-zA-Z0-9 \-]
PROJECT_COMPAGNY_NAME=Edouard DUPIN

# List of mainainer that might be contact in problem case : "Mr NAME Surname<mail@host.com>" "second ..."
PROJECT_MAINTAINER="Mr DUPIN Edouard <yui.heero@gmail.com>"

# the icon of the project is all time needed ... if it is not present the ewol icon might be set (must be a .png file) (and no space in the fileName and filePath)
PROJECT_ICON=$(shell pwd)/data/icon.png

# project section : (must be separate by coma
#    refer to : http://packages.debian.org/sid/
#        admin cli-mono comm database debian-installer
#        debug doc editors electronics devel embedded
#        fonts games gnome gnu-r gnustep graphics
#        hamradio haskell httpd interpreters java
#        kde kernel libdevel libs lisp localization
#        mail math misc net news ocaml oldlibs otherosfs
#        perl php python ruby science shells sound tex
#        text utils vcs video virtual web x11 xfce zope ...
PROJECT_SECTION=test

# project prority
#    required : Packages which are necessary for the proper functioning of the system (usually, this means that dpkg functionality depends on these packages). Removing a required package may cause your system to become totally broken and you may not even be able to use dpkg to put things back, so only do so if you know what you are doing. Systems with only the required packages are probably unusable, but they do have enough functionality to allow the sysadmin to boot and install more software.
#    important : Important programs, including those which one would expect to find on any Unix-like system. If the expectation is that an experienced Unix person who found it missing would say "What on earth is going on, where is foo?", it must be an important package.[6] Other packages without which the system will not run well or be usable must also have priority important. This does not include Emacs, the X Window System, TeX or any other large applications. The important packages are just a bare minimum of commonly-expected and necessary tools.
#    standard : These packages provide a reasonably small but not too limited character-mode system. This is what will be installed by default if the user doesn't select anything else. It doesn't include many large applications.
#    optional : (In a sense everything that isn't required is optional, but that's not what is meant here.) This is all the software that you might reasonably want to install if you didn't know what it was and don't have specialized requirements. This is a much larger system and includes the X Window System, a full TeX distribution, and many applications. Note that optional packages should not conflict with each other.
#    extra : This contains all packages that conflict with others with required, important, standard or optional priorities, or are only likely to be useful if you already know what they are or have specialized requirements (such as packages containing only detached debugging symbols).
PROJECT_PRIORITY=extra

# description of the current project inside quote and no \n
PROJECT_DESCRIPTION="Test software"

# Add package needed :
# current user packages
USER_PACKAGES =$(shell pwd)/

# include the basic makefile of ewol :
include $(shell pwd)/../../Makefile.mk

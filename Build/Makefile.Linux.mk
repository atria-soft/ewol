


include $(EWOL_FOLDER)/Build/coreLinux/main.mk




#include $(EWOL_FOLDER)/Build/Makefile.common.mk




#all:
#	@echo $(CADRE_HAUT_BAS)
#	@echo $(CADRE_COTERS)
#	@echo '
#	@echo '
#	@echo '
#	@echo '
#	@echo '
#	@echo $(CADRE_COTERS)
#	@echo $(CADRE_HAUT_BAS)
#	
#	@echo $(F_ROUGE)"
#	cd $(PROJECT_NDK) ; NDK_PROJECT_PATH=$(PROJECT_PATH) NDK_MODULE_PATH=$(PROJECT_MODULE) ./ndk-build
#	#cd $(PROJECT_NDK) ; NDK_PROJECT_PATH=$(PROJECT_PATH) NDK_MODULE_PATH=$(PROJECT_MODULE) NDK_LOG=~/.ndklog.txt make -f $(EWOL_FOLDER)/Build/coreLinux/build-local.mk

#install: all
#	@echo $(CADRE_HAUT_BAS)
#	@echo '           INSTALL : $(F_VIOLET)./bin/$(PROJECT_NAME)-debug.apk$(F_NORMALE)'$(CADRE_COTERS)
#	@echo $(CADRE_HAUT_BAS)
#	# TODO : Later ....

#clean:
#	@echo $(CADRE_HAUT_BAS)
#	@echo '           CLEANING : bin libs gen obj'$(CADRE_COTERS)
#	@echo $(CADRE_HAUT_BAS)
#	# TODO : Later ....
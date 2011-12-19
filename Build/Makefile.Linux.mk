

include $(EWOL_FOLDER)/Build/Makefile.common.mk




all:
	@echo $(CADRE_HAUT_BAS)
	@echo $(CADRE_COTERS)
	@echo '           DEBUT DE COMPILATION DU PROGRAMME :'$(CADRE_COTERS)
	@echo '             Project name    : $(F_BLUE)$(PROJECT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo '             Project Vendor  : $(F_CYAN)$(PROJECT_VENDOR)$(F_NORMALE)'$(CADRE_COTERS)
	@echo '             Build date      : $(F_ROUGE)$(BUILD_TIME) $(F_NORMALE)'$(CADRE_COTERS)
	@echo '             Tag             : $(F_VIOLET)$(PROJECT_VERSION_TAG) $(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	
	@echo $(F_ROUGE)"          (ndk-build) build native code"$(F_NORMALE)
	cd $(PROJECT_NDK) ; NDK_PROJECT_PATH=$(PROJECT_PATH) NDK_MODULE_PATH=$(PROJECT_MODULE) ./ndk-build
	#cd $(PROJECT_NDK) ; NDK_PROJECT_PATH=$(PROJECT_PATH) NDK_MODULE_PATH=$(PROJECT_MODULE) NDK_LOG=~/.ndklog.txt make -f $(EWOL_FOLDER)/Build/coreLinux/build-local.mk

install: all
	@echo $(CADRE_HAUT_BAS)
	@echo '           INSTALL : $(F_VIOLET)./bin/$(PROJECT_NAME)-debug.apk$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	# TODO : Later ....

clean:
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : bin libs gen obj'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	# TODO : Later ....

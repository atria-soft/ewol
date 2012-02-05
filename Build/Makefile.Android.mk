

include $(EWOL_FOLDER)/Build/Makefile.common.mk


PROJECT_PACKAGE=$(PROJECT_NAME)package
JAVA_FOLDER=src/com/$(PROJECT_VENDOR)/$(PROJECT_NAME)


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
	
	@rm -rf bin
	
	@echo $(F_ROUGE)"          (sh) Clear previous sources "$(F_NORMALE)
	@rm -rf src jni/ewolAndroidAbstraction.cpp
	@echo $(F_ROUGE)"          (sh) Create folder : $(JAVA_FOLDER)/ "$(F_NORMALE)
	@mkdir -p $(JAVA_FOLDER)/
	
	@echo $(F_ROUGE)"          (sh) copy the java File : "$(F_NORMALE)
	@cp $(EWOL_FOLDER)/Java/PROJECT_NAME.java $(JAVA_FOLDER)/$(PROJECT_NAME).java
	@echo $(F_ROUGE)"          (sh) Replace __PROJECT_VENDOR__, __PROJECT_NAME__ and __PROJECT_PACKAGE__ with the correct intance "$(F_NORMALE)
	@sed -i "s|__PROJECT_VENDOR__|$(PROJECT_VENDOR)|" $(JAVA_FOLDER)/$(PROJECT_NAME).java
	@sed -i "s|__PROJECT_NAME__|$(PROJECT_NAME)|" $(JAVA_FOLDER)/$(PROJECT_NAME).java
	@sed -i "s|__PROJECT_PACKAGE__|$(PROJECT_PACKAGE)|" $(JAVA_FOLDER)/$(PROJECT_NAME).java
	
	@echo $(F_ROUGE)"          (sh) copy the cpp for jni File : $(EWOL_FOLDER)/SourcesJava/ewolAndroidAbstraction.cpp"$(F_NORMALE)
	@cp $(EWOL_FOLDER)/Java/ewolAndroidAbstraction.cpp jni/
	@echo $(F_ROUGE)"          (sh) Replace __PROJECT_VENDOR__, __PROJECT_NAME__ and __PROJECT_PACKAGE__ with the correct intance "$(F_NORMALE)
	@sed -i "s|__PROJECT_VENDOR__|$(PROJECT_VENDOR)|" jni/ewolAndroidAbstraction.cpp
	@sed -i "s|__PROJECT_NAME__|$(PROJECT_NAME)|" jni/ewolAndroidAbstraction.cpp
	@sed -i "s|__PROJECT_PACKAGE__|$(PROJECT_PACKAGE)|" jni/ewolAndroidAbstraction.cpp
	
	@echo $(F_ROUGE)"          (ndk-build) build native code"$(F_NORMALE)
	cd $(PROJECT_NDK) ; NDK_PROJECT_PATH=$(PROJECT_PATH) NDK_MODULE_PATH=$(PROJECT_MODULE) ./ndk-build
	@echo $(F_ROUGE)"          (ant) build java code"$(F_NORMALE)
	PATH=$(PROJECT_SDK)/tools/:$(PROJECT_SDK)/platform-tools/:$(PATH) ant -Dsdk.dir=$(PROJECT_SDK) debug
	@echo $(F_ROUGE)"          (sh) Clear previous sources "$(F_NORMALE)
	@rm -rf src jni/ewolAndroidAbstraction.cpp

install: all
	@echo $(CADRE_HAUT_BAS)
	@echo '           INSTALL : $(F_VIOLET)./bin/$(PROJECT_NAME)-debug.apk$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@# $(PROJECT_SDK)/platform-tools/adb kill-server
	@# install application
	$(PROJECT_SDK)/platform-tools/adb  install -r ./bin/$(PROJECT_NAME)-debug.apk

clean:
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : bin libs gen obj'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	rm -rf bin libs gen obj

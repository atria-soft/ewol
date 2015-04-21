#!/usr/bin/python
import lutinModule as module
import lutinTools as tools
import datetime

def get_desc():
	return "Tutorial xxx example wallpaper"


def create(target):
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'examplewallpaper', 'PACKAGE')
	# add the file to compile:
	myModule.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/WidgetDisplay.cpp',
		'appl/Windows.cpp',
		])
	
	myModule.add_module_depend(['ewol'])
	
	myModule.compile_flags_CC([
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""])
	
	myModule.copy_folder('data/SnowFlake.svg','')
	
	myModule.add_path(tools.get_current_path(__file__))
	
	
	now = datetime.datetime.now()
	versionID=str(now.year-2013)+"."+str(now.month)+"."+str(now.day)
	
	# set the package properties :
	myModule.pkg_set("VERSION", versionID)
	myModule.pkg_set("COMPAGNY_TYPE", "org")
	myModule.pkg_set("COMPAGNY_NAME", "EWOL")
	myModule.pkg_set("MAINTAINER", ["Mr DUPIN Edouard <yui.heero@gmail.com>"])
	myModule.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.png")
	myModule.pkg_set("SECTION", "example")
	myModule.pkg_set("PRIORITY", "extra")
	myModule.pkg_set("DESCRIPTION", "EWOL example for Wallpaper on Android")
	myModule.pkg_set("NAME", "ewol Wallpaper ewample")
	
	myModule.pkg_set("ANDROID_APPL_TYPE", "WALLPAPER")
	
	#for the exemple : 
	myModule.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["list", "testpattern", "Select test pattern", "Choose which test pattern to display", \
	                                                         [ ["key","value display"],\
	                                                           ["key2","value display 2"]\
	                                                         ] \
	                                                ])
	myModule.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["bool", "movement", "Motion", "Apply movement to test pattern", ["Moving test pattern", "Still test pattern"]])
	
	
	# add the currrent module at the 
	return myModule





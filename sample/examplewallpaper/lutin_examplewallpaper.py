#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime

def get_desc():
	return "Tutorial xxx example wallpaper"


def create(target):
	# module name is 'edn' and type binary.
	my_module = module.Module(__file__, 'examplewallpaper', 'PACKAGE')
	# add the file to compile:
	my_module.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/WidgetDisplay.cpp',
		'appl/Windows.cpp',
		])
	
	my_module.add_module_depend(['ewol'])
	
	my_module.compile_flags('c++', [
		"-DPROJECT_NAME=\"\\\""+my_module.name+"\\\"\""])
	
	my_module.copy_path('data/SnowFlake.svg','')
	
	my_module.add_path(tools.get_current_path(__file__))
	
	
	now = datetime.datetime.now()
	versionID=str(now.year-2013)+"."+str(now.month)+"."+str(now.day)
	
	# set the package properties :
	my_module.pkg_set("VERSION", versionID)
	my_module.pkg_set("COMPAGNY_TYPE", "org")
	my_module.pkg_set("COMPAGNY_NAME", "EWOL")
	my_module.pkg_set("MAINTAINER", ["Mr DUPIN Edouard <yui.heero@gmail.com>"])
	my_module.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.png")
	my_module.pkg_set("SECTION", "example")
	my_module.pkg_set("PRIORITY", "extra")
	my_module.pkg_set("DESCRIPTION", "EWOL example for Wallpaper on Android")
	my_module.pkg_set("NAME", "ewol Wallpaper ewample")
	
	my_module.pkg_set("ANDROID_APPL_TYPE", "WALLPAPER")
	
	#for the exemple : 
	my_module.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["list", "testpattern", "Select test pattern", "Choose which test pattern to display", \
	                                                         [ ["key","value display"],\
	                                                           ["key2","value display 2"]\
	                                                         ] \
	                                                ])
	my_module.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["bool", "movement", "Motion", "Apply movement to test pattern", ["Moving test pattern", "Still test pattern"]])
	
	
	# add the currrent module at the 
	return my_module





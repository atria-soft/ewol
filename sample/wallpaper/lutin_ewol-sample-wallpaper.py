#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_type():
	return "BINARY"

def get_name():
	return "ewol Wallpaper sample"

def get_sub_type():
	return "SAMPLE"

def get_desc():
	return "Tutorial xxx example wallpaper"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/WidgetDisplay.cpp',
		'appl/Windows.cpp',
		])
	my_module.add_module_depend(['ewol'])
	my_module.compile_flags('c++', [
		"-DPROJECT_NAME=\"\\\""+my_module.name+"\\\"\"",
		"-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	my_module.copy_path('data/SnowFlake.svg','')
	my_module.add_path(tools.get_current_path(__file__))
	# set the package properties :
	my_module.pkg_set("COMPAGNY_TYPE", get_compagny_type())
	my_module.pkg_set("COMPAGNY_NAME", get_compagny_name())
	my_module.pkg_set("MAINTAINER", get_maintainer())
	my_module.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.png")
	my_module.pkg_set("SECTION", "example")
	my_module.pkg_set("PRIORITY", "extra")
	my_module.pkg_set("DESCRIPTION", get_desc())
	my_module.pkg_set("NAME", get_name())
	
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




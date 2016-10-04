#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


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

def configure(target, my_module):
	my_module.add_src_file([
	    'appl/Main.cpp',
	    'appl/debug.cpp',
	    'appl/WidgetDisplay.cpp',
	    'appl/Windows.cpp',
	    ])
	my_module.add_depend(['ewol'])
	my_module.add_flag('c++', [
	    "-DPROJECT_NAME=\"\\\""+my_module.get_name()+"\\\"\"",
	    "-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
	    ])
	my_module.copy_path('data/SnowFlake.svg','')
	my_module.add_path(".")
	# set the package properties :
	my_module.set_pkg("ICON", "../data/icon.png")
	my_module.set_pkg("SECTION", "example")
	my_module.set_pkg("PRIORITY", "extra")
	
	my_module.set_pkg("ANDROID_APPL_TYPE", "WALLPAPER")
	
	#for the exemple : 
	my_module.add_pkg("ANDROID_WALLPAPER_PROPERTIES", ["list", "testpattern", "Select test pattern", "Choose which test pattern to display", \
	                                                         [ ["key","value display"],\
	                                                           ["key2","value display 2"]\
	                                                         ] \
	                                                ])
	my_module.add_pkg("ANDROID_WALLPAPER_PROPERTIES", ["bool", "movement", "Motion", "Apply movement to test pattern", ["Moving test pattern", "Still test pattern"]])
	
	
	# add the currrent module at the 
	return True





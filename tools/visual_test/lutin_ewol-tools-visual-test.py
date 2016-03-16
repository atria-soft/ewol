#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import os

def get_type():
	return "BINARY"

def get_sub_type():
	return "TOOL"

def get_desc():
	return "ewol tools software (visual)"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	# add the file to compile:
	my_module.add_src_file([
	    'appl/debug.cpp',
	    'appl/init.cpp',
	    'appl/MainWindows.cpp',
	    'appl/TestDistanceField.cpp',
	    'appl/widget/SizerColor.cpp'
	    ])
	
	my_module.add_module_depend(['ewol'])
	
	my_module.copy_file('data/icon.png','icon.png')
	
	my_module.copy_path('data/icon.*','')
	my_module.copy_path('data/cube.*','')
	my_module.copy_path('data/grass.*','')
	my_module.copy_path('data/stone*','')
	my_module.copy_path('data/sphere.png','')
	my_module.copy_path('data/sphere.obj','')
	my_module.copy_path('data/gui.xml','')
	
	my_module.add_path(tools.get_current_path(__file__))
	
	my_module.copy_path("data/FreeSerif*","fonts/")
	
	"""
	# set the package properties :
	my_module.pkg_set("VERSION", versionID)
	my_module.pkg_set("COMPAGNY_TYPE", "org")
	my_module.pkg_set("COMPAGNY_NAME", "Edouard DUPIN")
	my_module.pkg_set("MAINTAINER", ["Mr DUPIN Edouard <yui.heero@gmail.com>"])
	my_module.pkg_set("ICON", tools.get_current_path(__file__) + "/../data/icon.png")
	my_module.pkg_set("SECTION", ["Development"])
	my_module.pkg_set("PRIORITY", "optional")
	my_module.pkg_set("DESCRIPTION", "ewol test software")
	my_module.pkg_set("NAME", "test software")
	
	my_module.pkg_add("RIGHT", "SET_ORIENTATION")
	my_module.pkg_add("RIGHT", "VIBRATE")
	"""
	
	return my_module





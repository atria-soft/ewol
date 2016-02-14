#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import os

def get_type():
	return "BINARY"

def get_sub_type():
	return "TOOL"

def get_desc():
	return "ewol test software (visual)"

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
	    'tools/visual_test/appl/debug.cpp',
	    'tools/visual_test/appl/init.cpp',
	    'tools/visual_test/appl/MainWindows.cpp',
	    'tools/visual_test/appl/TestDistanceField.cpp'
	    ])
	
	my_module.add_module_depend(['ewol'])
	
	my_module.copy_file('tools/visual_test/data/icon.png','icon.png')
	
	my_module.copy_path('tools/visual_test/data/icon.*','')
	my_module.copy_path('tools/visual_test/data/cube.*','')
	my_module.copy_path('tools/visual_test/data/grass.*','')
	my_module.copy_path('tools/visual_test/data/stone*','')
	my_module.copy_path('tools/visual_test/data/sphere.png','')
	my_module.copy_path('tools/visual_test/data/sphere.obj','')
	my_module.copy_path('tools/visual_test/data/gui.xml','')
	
	my_module.add_path(os.path.join(tools.get_current_path(__file__), "tools/visual_test/"))
	
	my_module.copy_path("tools/visual_test/data/FreeSerif*","fonts/")
	
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





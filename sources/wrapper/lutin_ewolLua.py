#!/usr/bin/python
import lutinModule as module
import lutinTools as tools
import os
import lutinMultiprocess

def get_desc():
	return "ewol lua Wrapper"

def get_license():
	return "APACHE v2.0"

def create(target):
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'ewolLua', 'LIBRARY')
	
	# add extra compilation flags :
	myModule.add_extra_compile_flags()
	# add the file to compile:
	myModule.add_src_file([
		'ewolLua/object.cpp',
		'ewolLua/objectManager.cpp',
		'ewolLua/ewolLua.cpp',
		'ewolLua/debug.cpp',
		])
	
	# name of the dependency
	myModule.add_module_depend(['ewol', 'luaWrapper'])
	myModule.add_export_path(tools.get_current_path(__file__))

	# add the currrent module at the 
	return myModule


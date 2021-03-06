#!/usr/bin/python
import realog.debug as debug
import lutin.tools as tools

def get_type():
	return "BINARY"

def get_sub_type():
	return "SAMPLE"

def get_desc():
	return "Tutorial 0XX : bsic tree example"

def get_licence():
	return "MPL-2"

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
	    'appl/Windows.cpp',
	    'appl/widget/BasicTree.cpp',
	    ])
	my_module.add_depend([
	    'ewol',
	    'test-debug',
	    ])
	my_module.add_flag('c++', [
	    "-DPROJECT_NAME=\"\\\""+my_module.get_name()+"\\\"\"",
	    "-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
	    ])
	my_module.add_path(".")
	return True





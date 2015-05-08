#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


# optionnal : Describe in the "lutin.py --help"
def get_desc():
	return "Tutorial 0XX : Create custom Widget"

# Module creation instance (not optionnal)
def create(target):
	# module name is '001_HelloWord' and type binary.
	myModule = module.Module(__file__, '0XX_customwidget', 'BINARY')
	# add the file to compile:
	myModule.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/Windows.cpp',
		'appl/widget/VectorDisplay.cpp',
		])
	
	# add Library dependency name
	myModule.add_module_depend(['ewol'])
	# add application C flags
	myModule.compile_flags('c++', [
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""])
	# Add current include Path
	myModule.add_path(tools.get_current_path(__file__))
	# return the created module
	return myModule





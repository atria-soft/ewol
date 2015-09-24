#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


# optionnal : Describe in the "lutin.py --help"
def get_desc():
	return "Tutorial 0XX : Create custom Widget"

# Module creation instance (not optionnal)
def create(target):
	# module name is '001_HelloWord' and type binary.
	my_module = module.Module(__file__, '0XX_customwidget', 'BINARY')
	# add the file to compile:
	my_module.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/Windows.cpp',
		'appl/widget/VectorDisplay.cpp',
		])
	
	# add Library dependency name
	my_module.add_module_depend(['ewol'])
	# add application C flags
	my_module.compile_flags('c++', [
		"-DPROJECT_NAME=\"\\\""+my_module.name+"\\\"\""])
	# Add current include Path
	my_module.add_path(tools.get_current_path(__file__))
	# return the created module
	return my_module





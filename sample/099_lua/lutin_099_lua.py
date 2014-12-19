#!/usr/bin/python
import lutinModule as module
import lutinTools as tools


# optionnal : Describe in the "lutin.py --help"
def get_desc():
	return "Tutorial 099 : lua example"

# Module creation instance (not optionnal)
def create(target):
	# module name is '001_HelloWord' and type binary.
	myModule = module.Module(__file__, '099_lua', 'BINARY')
	# add the file to compile:
	myModule.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/Windows.cpp',
		])
	# add Library dependency name
	myModule.add_module_depend(['ewolLua'])
	# add application C flags
	myModule.compile_flags_CC([
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""])
	# Add current include Path
	myModule.add_path(tools.get_current_path(__file__))
	# return the created module
	return myModule





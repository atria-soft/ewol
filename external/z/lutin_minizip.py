#!/usr/bin/python
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "MINIZIP : Small zip interface"


def create(target):
	myModule = module.Module(__file__, 'minizip', 'LIBRARY')
	myModule.add_src_file([
		"zlib/contrib/minizip/unzip.c",
		"zlib/contrib/minizip/zip.c",
		"zlib/contrib/minizip/miniunz.c",
		"zlib/contrib/minizip/ioapi.c"])
	
	myModule.add_export_path(tools.get_current_path(__file__) + "/zlib/contrib/")
	
	myModule.add_module_depend('z')
	
	myModule.compile_flags_CC([
		"-DNOCRYPT",
		"-DIOAPI_NO_64"])
	# add the currrent module at the 
	return myModule



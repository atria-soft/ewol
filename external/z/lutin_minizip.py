#!/usr/bin/python
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'minizip', 'LIBRARY')
	myModule.AddSrcFile([
		"zlib/contrib/minizip/unzip.c",
		"zlib/contrib/minizip/zip.c",
		"zlib/contrib/minizip/miniunz.c",
		"zlib/contrib/minizip/ioapi.c"])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__) + "/zlib/contrib/")
	
	myModule.AddModuleDepend('z')
	
	# add the currrent module at the 
	return myModule



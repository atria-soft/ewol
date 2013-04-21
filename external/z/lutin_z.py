#!/usr/bin/python
import lutinModule

def Create(target):
	myModule = lutinModule.module(__file__, 'z', 'PREBUILD')
	
	myModule.AddExportflag_LD('-lz')
	
	# add the currrent module at the 
	return myModule



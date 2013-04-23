#!/usr/bin/python
import lutinModule
import lutinTools

def Create(target):
	if target.name=="Windows":
		#http://glew.sourceforge.net/index.html
		myModule = lutinModule.module(__file__, 'glew', 'PREBUILD')
		
		myModule.AddExportPath(lutinTools.GetCurrentPath(__file__) + "/glew/include/")
		myModule.AddExportflag_LD([
			lutinTools.GetCurrentPath(__file__) + "/glew/lib/glew32s.lib",
			"-lopengl32",
			"-lgdi32"])
		
		# add the currrent module at the 
		return myModule
	else:
		return None



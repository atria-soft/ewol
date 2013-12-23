#!/usr/bin/python
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "Glew generic glew interface (for windows only)"


def create(target):
	if target.name=="Windows":
		#http://glew.sourceforge.net/index.html
		myModule = module.Module(__file__, 'glew', 'PREBUILD')
		
		myModule.add_export_path(tools.get_current_path(__file__) + "/glew/include/")
		myModule.add_export_flag_LD([
			tools.get_current_path(__file__) + "/glew/lib/glew32s.lib",
			"-lopengl32",
			"-lgdi32",
			"-static-libgcc",
			"-static-libstdc++"])
		
		# add the currrent module at the 
		return myModule
	else:
		return None



#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_plugin_OctreeZone', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main', 'ogre_plugin_PCZSceneManager'])
	
	myModule.CompileFlags_CC([
		'-DPlugin_OctreeZone_EXPORTS',
		'-msse',
		'-Wno-deprecated',
		'-Wall',
		'-Wctor-dtor-privacy',
		'-Winit-self',
		'-Wno-overloaded-virtual',
		'-Wcast-qual',
		'-Wwrite-strings',
		'-Wextra',
		'-Wno-unused-parameter',
		'-pedantic',
		'-Wshadow',
		'-Wno-missing-field-initializers',
		'-Wno-long-long',
		'-Wno-unused-but-set-parameter',
		'-O2',
		'-DNDEBUG'])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/PlugIns/OctreeZone/include")
	
	myModule.AddSrcFile([
		'ogre/PlugIns/OctreeZone/src/OgreOctreeZone.cpp',
		'ogre/PlugIns/OctreeZone/src/OgreOctreeZoneDll.cpp',
		'ogre/PlugIns/OctreeZone/src/OgreOctreeZoneOctree.cpp',
		'ogre/PlugIns/OctreeZone/src/OgreOctreeZonePlugin.cpp'])
	
	# add the currrent module at the 
	return myModule


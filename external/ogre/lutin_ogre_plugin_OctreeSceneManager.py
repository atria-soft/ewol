#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_plugin_OctreeSceneManager', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DPlugin_OctreeSceneManager_EXPORTS',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/PlugIns/OctreeSceneManager/include")
	
	myModule.AddSrcFile([
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctree.cpp',
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctreeCamera.cpp',
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctreeNode.cpp',
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctreePlugin.cpp',
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctreeSceneManager.cpp',
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctreeSceneManagerDll.cpp',
		'ogre/PlugIns/OctreeSceneManager/src/OgreOctreeSceneQuery.cpp'])
	
	# add the currrent module at the 
	return myModule





#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_plugin_BSPSceneManager', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DPlugin_BSPSceneManager_EXPORTS',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/PlugIns/BSPSceneManager/include")
	
	myModule.AddSrcFile([
		'ogre/PlugIns/BSPSceneManager/src/OgreBspLevel.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreBspNode.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreBspResourceManager.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreBspSceneManager.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreBspSceneManagerDll.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreBspSceneManagerPlugin.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreBspSceneNode.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreQuake3Level.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreQuake3Shader.cpp',
		'ogre/PlugIns/BSPSceneManager/src/OgreQuake3ShaderManager.cpp'])
	
	# add the currrent module at the 
	return myModule


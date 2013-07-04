#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre', 'LIBRARY')
	
	myModule.AddModuleDepend([
		'ogre_main',
		'ogre_paging',
		'ogre_plugin_BSPSceneManager',
		'ogre_plugin_OctreeSceneManager',
		'ogre_plugin_OctreeZone',
		'ogre_plugin_PCZSceneManager',
		'ogre_plugin_particuleFX',
		'ogre_RenderGL',
		'ogre_RTShaderSystem',
		'ogre_terrain'])
	"""
	myModule.CompileFlags_CC([
		
		])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/OgreMain/include")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/ogre/include")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/ogre")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/ogre/OgreMain/include/Threading")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/ogre/OgreMain/include/GLX")
	myModule.AddPath(lutinTools.GetCurrentPath(__file__)+"/ogre/OgreMain/src/nedmalloc")
	
	myModule.AddSrcFile([
		
		])
	"""
	# add the currrent module at the 
	return myModule


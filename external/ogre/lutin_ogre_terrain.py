#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_terrain', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main', 'ogre_paging'])
	
	myModule.CompileFlags_CC([
		'-DOgreTerrain_EXPORTS',
		'-D_MT',
		'-D_USRDLL',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/Components/Terrain/include")
	
	myModule.AddSrcFile([
		'ogre/Components/Terrain/src/OgreTerrain.cpp',
		'ogre/Components/Terrain/src/OgreTerrainGroup.cpp',
		'ogre/Components/Terrain/src/OgreTerrainLayerBlendMap.cpp',
		'ogre/Components/Terrain/src/OgreTerrainMaterialGenerator.cpp',
		'ogre/Components/Terrain/src/OgreTerrainMaterialGeneratorA.cpp',
		'ogre/Components/Terrain/src/OgreTerrainQuadTreeNode.cpp',
		'ogre/Components/Terrain/src/OgreTerrainPaging.cpp',
		'ogre/Components/Terrain/src/OgreTerrainPagedWorldSection.cpp'])
	
	# add the currrent module at the 
	return myModule



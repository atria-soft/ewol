#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_paging', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DOgrePaging_EXPORTS',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/Components/Paging/include")
	
	myModule.AddSrcFile([
		'ogre/Components/Paging/src/OgreGrid2DPageStrategy.cpp',
		'ogre/Components/Paging/src/OgreGrid3DPageStrategy.cpp',
		'ogre/Components/Paging/src/OgrePage.cpp',
		'ogre/Components/Paging/src/OgrePageContent.cpp',
		'ogre/Components/Paging/src/OgrePageContentCollection.cpp',
		'ogre/Components/Paging/src/OgrePagedWorld.cpp',
		'ogre/Components/Paging/src/OgrePagedWorldSection.cpp',
		'ogre/Components/Paging/src/OgrePageManager.cpp',
		'ogre/Components/Paging/src/OgreSimplePageContentCollection.cpp'])
	
	# add the currrent module at the 
	return myModule



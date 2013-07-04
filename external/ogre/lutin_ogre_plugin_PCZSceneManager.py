#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_plugin_PCZSceneManager', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DPlugin_PCZSceneManager_EXPORTS',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/PlugIns/PCZSceneManager/include")
	
	myModule.AddSrcFile([
		'ogre/PlugIns/PCZSceneManager/src/OgreAntiPortal.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgreCapsule.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgreDefaultZone.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCPlane.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZCamera.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZFrustum.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZLight.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZone.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZoneFactory.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZPlugin.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZSceneManager.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZSceneManagerDll.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZSceneNode.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePCZSceneQuery.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePortal.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgrePortalBase.cpp',
		'ogre/PlugIns/PCZSceneManager/src/OgreSegment.cpp'])
	
	# add the currrent module at the 
	return myModule

#/usr/bin/c++  -fPIC  -msse -Wno-deprecated -Wall -Wctor-dtor-privacy -Winit-self -Wno-overloaded-virtual -Wcast-qual -Wwrite-strings -Wextra -Wno-unused-parameter -pedantic -Wshadow -Wno-missing-field-initializers -Wno-long-long -Wno-unused-but-set-parameter -O2 -g -DNDEBUG   -shared -Wl,-soname,Plugin_PCZSceneManager.so.1.8.2 -o ../../lib/Plugin_PCZSceneManager.so.1.8.2 CMakeFiles/Plugin_PCZSceneManager.dir/src/OgreAntiPortal.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgreCapsule.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgreDefaultZone.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCPlane.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZCamera.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZFrustum.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZLight.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZone.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZoneFactory.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZPlugin.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZSceneManager.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZSceneManagerDll.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZSceneNode.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePCZSceneQuery.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePortal.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgrePortalBase.cpp.o CMakeFiles/Plugin_PCZSceneManager.dir/src/OgreSegment.cpp.o ../../lib/libOgreMain.so.1.8.2 -lfreetype -lSM -lICE -lX11 -lXext -lXt -lXaw -lpthread -ldl -Wl,-rpath,ogre/lib: 


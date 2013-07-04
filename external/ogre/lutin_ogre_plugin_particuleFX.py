#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_plugin_particuleFX', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DPlugin_ParticleFX_EXPORTS',
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
		'-Wno-long-long -Wno-unused-but-set-parameter',
		'-O2',
		'-DNDEBUG'])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/PlugIns/ParticleFX/include")
	
	myModule.AddSrcFile([
		'ogre/PlugIns/ParticleFX/src/OgreAreaEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreBoxEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreColourFaderAffector2.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreColourFaderAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreColourImageAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreColourInterpolatorAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreCylinderEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreDeflectorPlaneAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreDirectionRandomiserAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreEllipsoidEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreHollowEllipsoidEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreLinearForceAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreParticleFX.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreParticleFXPlugin.cpp',
		'ogre/PlugIns/ParticleFX/src/OgrePointEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreRingEmitter.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreRotationAffector.cpp',
		'ogre/PlugIns/ParticleFX/src/OgreScaleAffector.cpp'])
	
	# add the currrent module at the 
	return myModule



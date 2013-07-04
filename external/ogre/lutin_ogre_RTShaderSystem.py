#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_RTShaderSystem', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DOgreRTShaderSystem_EXPORTS',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/Components/RTShaderSystem/include")
	
	myModule.AddSrcFile([
		'ogre/Components/RTShaderSystem/src/OgreShaderFFPColour.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFFPFog.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFFPLighting.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFFPRenderStateBuilder.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFFPTexturing.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFFPTransform.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFunction.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderFunctionAtom.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderGenerator.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderParameter.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderProgram.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderProgramManager.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderProgramSet.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderProgramWriter.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderRenderState.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderSubRenderState.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExNormalMapLighting.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExPerPixelLighting.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExIntegratedPSSM3.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderScriptTranslator.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderMaterialSerializerListener.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderProgramProcessor.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderCGProgramProcessor.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderCGProgramWriter.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderGLSLProgramProcessor.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderGLSLProgramWriter.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderGLSLESProgramProcessor.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderGLSLESProgramWriter.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderProgramWriterManager.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderHLSLProgramProcessor.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderHLSLProgramWriter.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExLayeredBlending.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExHardwareSkinning.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExHardwareSkinningTechnique.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExDualQuaternionSkinning.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExLinearSkinning.cpp',
		'ogre/Components/RTShaderSystem/src/OgreShaderExTextureAtlasSampler.cpp'])
	
	# add the currrent module at the 
	return myModule



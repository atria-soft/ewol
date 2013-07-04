#!/usr/bin/python
# --------------------------------------------------------
# -- Ogre_main librairy
# --------------------------------------------------------
import lutinModule
import lutinTools

def Create(target):
	myModule = lutinModule.module(__file__, 'ogre_RenderGL', 'LIBRARY')
	
	myModule.AddModuleDepend(['ogre_main'])
	
	myModule.CompileFlags_CC([
		'-DRenderSystem_GL_EXPORTS',
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
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/RenderSystems/GL/src/GLX")
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/RenderSystems/GL/include/GLX")
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/RenderSystems/GL/include")
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/RenderSystems/GL/src/GLSL/include")
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/RenderSystems/GL/src/atifs/include")
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__)+"/ogre/RenderSystems/GL/src/nvparse")
	
	myModule.AddSrcFile([
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLExtSupport.cpp',
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLGpuProgram.cpp',
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLLinkProgram.cpp',
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLLinkProgramManager.cpp',
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLPreprocessor.cpp',
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLProgram.cpp',
		'ogre/RenderSystems/GL/src/GLSL/src/OgreGLSLProgramFactory.cpp',
		'ogre/RenderSystems/GL/src/atifs/src/ATI_FS_GLGpuProgram.cpp',
		'ogre/RenderSystems/GL/src/atifs/src/Compiler2Pass.cpp',
		'ogre/RenderSystems/GL/src/atifs/src/ps_1_4.cpp',
		'ogre/RenderSystems/GL/src/nvparse/avp1.0_impl.cpp',
		'ogre/RenderSystems/GL/src/nvparse/nvparse.cpp',
		'ogre/RenderSystems/GL/src/nvparse/nvparse_errors.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_ps1.0_lexer.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_ps1.0_parser.cpp',
		'ogre/RenderSystems/GL/src/nvparse/ps1.0_program.cpp',
		'ogre/RenderSystems/GL/src/nvparse/rc1.0_combiners.cpp',
		'ogre/RenderSystems/GL/src/nvparse/rc1.0_final.cpp',
		'ogre/RenderSystems/GL/src/nvparse/rc1.0_general.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_rc1.0_lexer.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_rc1.0_parser.cpp',
		'ogre/RenderSystems/GL/src/nvparse/ts1.0_inst.cpp',
		'ogre/RenderSystems/GL/src/nvparse/ts1.0_inst_list.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_ts1.0_lexer.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_ts1.0_parser.cpp',
		'ogre/RenderSystems/GL/src/nvparse/vcp1.0_impl.cpp',
		'ogre/RenderSystems/GL/src/nvparse/vp1.0_impl.cpp',
		'ogre/RenderSystems/GL/src/nvparse/vs1.0_inst.cpp',
		'ogre/RenderSystems/GL/src/nvparse/vs1.0_inst_list.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_vs1.0_lexer.cpp',
		'ogre/RenderSystems/GL/src/nvparse/_vs1.0_parser.cpp',
		'ogre/RenderSystems/GL/src/nvparse/vsp1.0_impl.cpp',
		'ogre/RenderSystems/GL/src/GLX/OgreGLXContext.cpp',
		'ogre/RenderSystems/GL/src/GLX/OgreGLXGLSupport.cpp',
		'ogre/RenderSystems/GL/src/GLX/OgreGLXRenderTexture.cpp',
		'ogre/RenderSystems/GL/src/GLX/OgreGLXWindow.cpp',
		'ogre/RenderSystems/GL/src/OgreGLATIFSInit.cpp',
		'ogre/RenderSystems/GL/src/OgreGLContext.cpp',
		'ogre/RenderSystems/GL/src/OgreGLDefaultHardwareBufferManager.cpp',
		'ogre/RenderSystems/GL/src/OgreGLDepthBuffer.cpp',
		'ogre/RenderSystems/GL/src/OgreGLEngineDll.cpp',
		'ogre/RenderSystems/GL/src/OgreGLFBOMultiRenderTarget.cpp',
		'ogre/RenderSystems/GL/src/OgreGLFBORenderTexture.cpp',
		'ogre/RenderSystems/GL/src/OgreGLFrameBufferObject.cpp',
		'ogre/RenderSystems/GL/src/OgreGLGpuNvparseProgram.cpp',
		'ogre/RenderSystems/GL/src/OgreGLGpuProgram.cpp',
		'ogre/RenderSystems/GL/src/OgreGLGpuProgramManager.cpp',
		'ogre/RenderSystems/GL/src/OgreGLHardwareBufferManager.cpp',
		'ogre/RenderSystems/GL/src/OgreGLHardwareIndexBuffer.cpp',
		'ogre/RenderSystems/GL/src/OgreGLHardwareOcclusionQuery.cpp',
		'ogre/RenderSystems/GL/src/OgreGLHardwarePixelBuffer.cpp',
		'ogre/RenderSystems/GL/src/OgreGLHardwareVertexBuffer.cpp',
		'ogre/RenderSystems/GL/src/OgreGLPBRenderTexture.cpp',
		'ogre/RenderSystems/GL/src/OgreGLPixelFormat.cpp',
		'ogre/RenderSystems/GL/src/OgreGLPlugin.cpp',
		'ogre/RenderSystems/GL/src/OgreGLRenderSystem.cpp',
		'ogre/RenderSystems/GL/src/OgreGLRenderTexture.cpp',
		'ogre/RenderSystems/GL/src/OgreGLRenderToVertexBuffer.cpp',
		'ogre/RenderSystems/GL/src/OgreGLSupport.cpp',
		'ogre/RenderSystems/GL/src/OgreGLTexture.cpp',
		'ogre/RenderSystems/GL/src/OgreGLTextureManager.cpp',
		'ogre/RenderSystems/GL/src/glew.cpp'])
	
	# add the currrent module at the 
	return myModule




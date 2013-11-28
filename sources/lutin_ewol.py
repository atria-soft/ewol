#!/usr/bin/python
import lutinModule
import lutinTools
import os
import lutinMultiprocess

def Create(target):
	# set the ewol folder for Android basic sources ...
	target.SetEwolFolder(lutinTools.GetCurrentPath(__file__) + "/../")
	
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'ewol', 'LIBRARY')
	# add extra compilation flags :
	#myModule.add_extra_compile_flags()
	# add the file to compile:
	myModule.AddSrcFile([
		'ewol/ewol.cpp',
		'ewol/clipBoard.cpp',
		'ewol/debug.cpp',
		'ewol/commandLine.cpp',
		'ewol/key.cpp',
		'ewol/cursor.cpp',
		'ewol/Dimension.cpp',
		'ewol/Light.cpp',
		'ewol/Material.cpp'])
	
	#openGl Basic access abstraction (for the model matrix and include
	myModule.AddSrcFile([
		'ewol/renderer/EConfig.cpp',
		'ewol/renderer/EMessage.cpp',
		'ewol/renderer/EObject.cpp',
		'ewol/renderer/EObjectManager.cpp',
		'ewol/renderer/EMultiCast.cpp',
		'ewol/renderer/openGL.cpp',
		'ewol/renderer/ConfigFont.cpp',
		'ewol/renderer/EventInput.cpp',
		'ewol/renderer/EventEntry.cpp',
		'ewol/renderer/EventTime.cpp',
		'ewol/renderer/eContext.cpp',
		'ewol/renderer/eInput.cpp'])
	
	# resources : 
	myModule.AddSrcFile([
		'ewol/resources/Shader.cpp',
		'ewol/resources/Program.cpp',
		'ewol/resources/VirtualBufferObject.cpp',
		'ewol/resources/ConfigFile.cpp',
		'ewol/resources/FontFreeType.cpp',
		'ewol/resources/TexturedFont.cpp',
		'ewol/resources/Mesh.cpp',
		'ewol/resources/Texture.cpp',
		'ewol/resources/Colored3DObject.cpp',
		'ewol/resources/Image.cpp',
		'ewol/resources/Resource.cpp',
		'ewol/resources/ResourceManager.cpp'])
	
	# physical shape parser
	myModule.AddSrcFile([
		'ewol/physicsShape/PhysicsShape.cpp',
		'ewol/physicsShape/PhysicsBox.cpp',
		'ewol/physicsShape/PhysicsCapsule.cpp',
		'ewol/physicsShape/PhysicsCone.cpp',
		'ewol/physicsShape/PhysicsConvexHull.cpp',
		'ewol/physicsShape/PhysicsCylinder.cpp',
		'ewol/physicsShape/PhysicsSphere.cpp'])
	
	# Audio system
	myModule.AddSrcFile([
		'ewol/renderer/audio/audio.cpp',
		'ewol/renderer/audio/decWav.cpp'])
	
	# Compositing
	myModule.AddSrcFile([
		'ewol/compositing/Compositing.cpp',
		'ewol/compositing/Text.cpp',
		'ewol/compositing/Drawing.cpp',
		'ewol/compositing/Image.cpp',
		'ewol/compositing/Sprite.cpp',
		'ewol/compositing/Mesh.cpp',
		'ewol/compositing/Shaper.cpp',
		'ewol/compositing/Area.cpp'])
	
	# all widgets
	myModule.AddSrcFile([
		'ewol/widget/Widget.cpp',
		'ewol/widget/WidgetManager.cpp',
		'ewol/widget/Windows.cpp',
		'ewol/widget/Button.cpp',
		'ewol/widget/Gird.cpp',
		'ewol/widget/Image.cpp',
		'ewol/widget/ButtonColor.cpp',
		'ewol/widget/CheckBox.cpp',
		'ewol/widget/ColorBar.cpp',
		'ewol/widget/ContextMenu.cpp',
		'ewol/widget/Composer.cpp',
		'ewol/widget/Container.cpp',
		'ewol/widget/ContainerN.cpp',
		'ewol/widget/Entry.cpp',
		'ewol/widget/Joystick.cpp',
		'ewol/widget/Label.cpp',
		'ewol/widget/List.cpp',
		'ewol/widget/ListFileSystem.cpp',
		'ewol/widget/Layer.cpp',
		'ewol/widget/Menu.cpp',
		'ewol/widget/Mesh.cpp',
		'ewol/widget/PopUp.cpp',
		'ewol/widget/ProgressBar.cpp',
		'ewol/widget/Scroll.cpp',
		'ewol/widget/Sizer.cpp',
		'ewol/widget/Slider.cpp',
		'ewol/widget/WSlider.cpp',
		'ewol/widget/Spacer.cpp',
		'ewol/widget/WidgetScrolled.cpp',
		'ewol/widget/meta/StdPopUp.cpp',
		'ewol/widget/meta/FileChooser.cpp',
		'ewol/widget/meta/ColorChooser.cpp',
		'ewol/widget/meta/Parameter.cpp',
		'ewol/widget/meta/ParameterList.cpp'])
	
	myModule.CopyFolder('../data/theme/default/widgetEntry.*','theme/default')
	myModule.CopyFolder('../data/theme/rounded/widgetEntry.*','theme/rounded')
	myModule.CopyFolder('../data/theme/default/widgetButton.*','theme/default')
	myModule.CopyFolder('../data/theme/rounded/widgetButton.*','theme/rounded')
	myModule.CopyFolder('../data/theme/default/widgetContextMenu.*','theme/default')
	myModule.CopyFolder('../data/theme/rounded/widgetContextMenu.*','theme/rounded')
	myModule.CopyFolder('../data/theme/default/widgetPopUp.*','theme/default')
	myModule.CopyFolder('../data/theme/rounded/widgetPopUp.*','theme/rounded')
	myModule.CopyFolder('../data/textured.*','')
	myModule.CopyFolder('../data/texturedNoMaterial.*','')
	myModule.CopyFolder('../data/text.*','')
	myModule.CopyFolder('../data/simple3D.*','')
	myModule.CopyFolder('../data/color.*','')
	myModule.CopyFolder('../data/color3.*','')
	myModule.CopyFolder('../data/textured3D2.*','')
	myModule.CopyFolder('../data/textured3D.*','')
	
	# name of the dependency
	myModule.AddModuleDepend(['etk', 'freetype', 'exml', 'ejson', 'egami', 'date'])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__))
	
	tagFile = lutinTools.GetCurrentPath(__file__) + "/tag"
	ewolVersionID = lutinTools.FileReadData(tagFile)
	myModule.CompileFlags_CC([
		"-DEWOL_VERSION=\"\\\""+ewolVersionID+"\\\"\""
		])
	
	if target.name=="Linux":
		myModule.AddExportflag_LD('-lGL')
		
		#`pkg-config --cflags directfb` `pkg-config --libs directfb`
		
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_X11__)","y")
		myModule.AddExportflag_LD('-lX11')
		#endif
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_DIRECT_FB__)","y")
		#myModule.AddExportflag_LD(['-L/usr/local/lib', '-ldirectfb', '-lfusion', '-ldirect'])
		#endif
		
		#http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
		# needed package on linux : libgl1-mesa-dev libglew1.5-dev
		
		#ifeq ("$(CONFIG_BUILD_PORTAUDIO)","y")
		#myModule.AddSrcFile('ewol/renderer/audio/interfacePortAudio.cpp')
		#endif
		
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_X11__)","y")
		myModule.AddSrcFile('ewol/renderer/X11/Context.cpp')
		#endif
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_DIRECT_FB__)","y")
		#myModule.CompileFlags_CC('-I/usr/local/include/directfb')
		#myModule.AddSrcFile('ewol/renderer/os/gui.directFB.cpp')
		#endif
	
	elif target.name=="Android":
		myModule.AddExportflag_LD("-lGLESv2")
		
		myModule.AddExportflag_LD("-ldl")
		myModule.AddExportflag_LD("-llog")
		myModule.AddExportflag_LD("-landroid")

		java_tmp_dir = lutinTools.GetCurrentPath(__file__) + "/../../ewol/sources/android/src/"
		cpp_tmp_dir = lutinTools.GetCurrentPath(__file__) + "/ewol/renderer/Android/"
		java_tmp_src = java_tmp_dir + "org/ewol/EwolConstants"
		lutinMultiprocess.RunCommand("javac " + java_tmp_src + ".java")
		lutinMultiprocess.RunCommand("cd " + java_tmp_dir + " && javah org.ewol.EwolConstants")
		lutinTools.CopyFile(java_tmp_dir + "org_ewol_EwolConstants.h", cpp_tmp_dir + "org_ewol_EwolConstants.h", True)
		lutinTools.RemoveFile(java_tmp_src + ".class")
		
		myModule.AddSrcFile("ewol/renderer/Android/Context.cpp")
	
	elif target.name=="Windows":
		myModule.AddModuleDepend("glew")
		myModule.AddSrcFile("ewol/renderer/Windows/Context.cpp")
	elif target.name=="MacOs":
		myModule.AddExportflag_LD([
			"-framework Cocoa",
			"-framework OpenGL",
			"-framework QuartzCore",
			"-framework AppKit"])
		myModule.AddSrcFile([
			"ewol/renderer/MacOs/Context.cpp",
			"ewol/renderer/MacOs/Interface.mm",
			"ewol/renderer/MacOs/AppDelegate.mm",
			"ewol/renderer/MacOs/OpenglView.mm"])
	else:
		debug.error("unknow mode...")
	
	# add the currrent module at the 
	return myModule


def GetDesc():
	return "ewol is a main library to use widget in the openGl environement and manage all the wraping os"


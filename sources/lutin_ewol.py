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
	# enable doculentation :
	myModule.doc_enable()
	myModule.documentation.set_website("http://HeeroYui.github.io/ewol/")
	myModule.documentation.set_path(lutinTools.GetCurrentPath(__file__) + "/ewol/")
	myModule.documentation.set_path_general_doc(lutinTools.GetCurrentPath(__file__) + "/../doc/")
	myModule.documentation.set_external_link(['etk', 'freetype', 'exml', 'ejson', 'egami', 'date'])
	# add extra compilation flags :
	#myModule.add_extra_compile_flags()
	# add the file to compile:
	myModule.AddSrcFile([
		'ewol/ewol.cpp',
		'ewol/debug.cpp'
		])
	
	# compositing :
	myModule.AddSrcFile([
		'ewol/compositing/Compositing.cpp',
		'ewol/compositing/Text.cpp',
		'ewol/compositing/Drawing.cpp',
		'ewol/compositing/Image.cpp',
		'ewol/compositing/Sprite.cpp',
		'ewol/compositing/Shaper.cpp',
		'ewol/compositing/Area.cpp'
		])
	
	# context :
	myModule.AddSrcFile([
		'ewol/context/clipBoard.cpp',
		'ewol/context/commandLine.cpp',
		'ewol/context/ConfigFont.cpp',
		'ewol/context/Context.cpp',
		'ewol/context/cursor.cpp',
		'ewol/context/Dimension.cpp',
		'ewol/context/InputManager.cpp'
		])
	if target.name=="Linux":
		myModule.AddSrcFile('ewol/context/X11/Context.cpp')
	elif target.name=="Android":
		myModule.AddSrcFile("ewol/context/Android/Context.cpp")
	elif target.name=="Windows":
		myModule.AddSrcFile("ewol/context/Windows/Context.cpp")
	elif target.name=="MacOs":
		myModule.AddSrcFile([
			"ewol/context/MacOs/Context.cpp",
			"ewol/context/MacOs/Interface.mm",
			"ewol/context/MacOs/AppDelegate.mm",
			"ewol/context/MacOs/OpenglView.mm"])
	else:
		debug.error("unknow mode...")
	
	# Key properties :
	myModule.AddSrcFile([
		'ewol/key/keyboard.cpp',
		'ewol/key/Special.cpp',
		'ewol/key/status.cpp',
		'ewol/key/type.cpp'
		])
	
	# object :
	myModule.AddSrcFile([
		'ewol/object/Config.cpp',
		'ewol/object/ConfigElement.cpp',
		'ewol/object/Manager.cpp',
		'ewol/object/Message.cpp',
		'ewol/object/MultiCast.cpp',
		'ewol/object/Object.cpp'
		])
	
	# OpenGL interface :
	myModule.AddSrcFile([
		'ewol/openGL/openGL.cpp'
		])
	
	# resources :
	myModule.AddSrcFile([
		'ewol/resource/Colored3DObject.cpp',
		'ewol/resource/ConfigFile.cpp',
		'ewol/resource/FontFreeType.cpp',
		'ewol/resource/Image.cpp',
		'ewol/resource/Manager.cpp',
		'ewol/resource/Program.cpp',
		'ewol/resource/Resource.cpp',
		'ewol/resource/Shader.cpp',
		'ewol/resource/Texture.cpp',
		'ewol/resource/TexturedFont.cpp',
		'ewol/resource/VirtualBufferObject.cpp'
		])
	
	# widget :
	myModule.AddSrcFile([
		'ewol/widget/ButtonColor.cpp',
		'ewol/widget/Button.cpp',
		'ewol/widget/CheckBox.cpp',
		'ewol/widget/ColorBar.cpp',
		'ewol/widget/Composer.cpp',
		'ewol/widget/Container.cpp',
		'ewol/widget/ContainerN.cpp',
		'ewol/widget/ContextMenu.cpp',
		'ewol/widget/Entry.cpp',
		'ewol/widget/Gird.cpp',
		'ewol/widget/Image.cpp',
		'ewol/widget/Joystick.cpp',
		'ewol/widget/Label.cpp',
		'ewol/widget/Layer.cpp',
		'ewol/widget/List.cpp',
		'ewol/widget/ListFileSystem.cpp',
		'ewol/widget/Manager.cpp',
		'ewol/widget/Menu.cpp',
		'ewol/widget/meta/ColorChooser.cpp',
		'ewol/widget/meta/FileChooser.cpp',
		'ewol/widget/meta/Parameter.cpp',
		'ewol/widget/meta/ParameterList.cpp',
		'ewol/widget/meta/StdPopUp.cpp',
		'ewol/widget/PopUp.cpp',
		'ewol/widget/ProgressBar.cpp',
		'ewol/widget/Scroll.cpp',
		'ewol/widget/Sizer.cpp',
		'ewol/widget/Slider.cpp',
		'ewol/widget/Spacer.cpp',
		'ewol/widget/WidgetScrolled.cpp',
		'ewol/widget/Windows.cpp',
		'ewol/widget/WSlider.cpp',
		])
	
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


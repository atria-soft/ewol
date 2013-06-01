#!/usr/bin/python
import lutinModule
import lutinTools
import os

def Create(target):
	# set the ewol folder for Android basic sources ...
	target.SetEwolFolder(lutinTools.GetCurrentPath(__file__) + "/../")
	
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'ewol', 'LIBRARY')
	# add the file to compile:
	myModule.AddSrcFile([
		'ewol/ewol.cpp',
		'ewol/clipBoard.cpp',
		'ewol/debug.cpp',
		'ewol/config.cpp',
		'ewol/commandLine.cpp',
		'ewol/key.cpp',
		'ewol/cursor.cpp',
		'ewol/Dimension.cpp',
		'ewol/UserConfig.cpp'])
	
	# Basic Eobject of EWOL
	myModule.AddSrcFile([
		'ewol/eObject/EConfig.cpp',
		'ewol/eObject/EMessage.cpp',
		'ewol/eObject/EObject.cpp',
		'ewol/eObject/EObjectManager.cpp'])
	
	#openGl Basic access abstraction (for the model matrix and include
	myModule.AddSrcFile([
		'ewol/renderer/openGL.cpp',
		'ewol/renderer/EventInput.cpp',
		'ewol/renderer/EventEntry.cpp',
		'ewol/renderer/EventTime.cpp',
		'ewol/renderer/Light.cpp',
		'ewol/renderer/Material.cpp'])
	
	# Operating System interface
	myModule.AddSrcFile([
		'ewol/renderer/os/eSystem.cpp',
		'ewol/renderer/os/eSystemInput.cpp'])
	
	# renderer : 
	myModule.AddSrcFile([
		'ewol/renderer/resources/Shader.cpp',
		'ewol/renderer/resources/Program.cpp',
		'ewol/renderer/resources/VirtualBufferObject.cpp',
		'ewol/renderer/resources/ConfigFile.cpp',
		'ewol/renderer/resources/FontFreeType.cpp',
		'ewol/renderer/resources/TexturedFont.cpp',
		'ewol/renderer/resources/Mesh.cpp',
		'ewol/renderer/resources/MeshObj.cpp',
		'ewol/renderer/resources/Texture.cpp',
		'ewol/renderer/resources/Colored3DObject.cpp',
		'ewol/renderer/resources/Image.cpp',
		'ewol/renderer/resources/image/ImageBMP.cpp',
		'ewol/renderer/resources/image/ImagePNG.cpp',
		'ewol/renderer/ResourceManager.cpp'])
	
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
		'ewol/widget/Drawable.cpp',
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
		'ewol/widget/meta/FileChooser.cpp',
		'ewol/widget/meta/ColorChooser.cpp',
		'ewol/widget/meta/Parameter.cpp',
		'ewol/widget/meta/ParameterList.cpp'])
	
	# game mode area :
	myModule.AddSrcFile([
		'ewol/widget/Scene.cpp',
		'ewol/game/Camera.cpp'])
	
	
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
	
	# add the configuration files
	#myModule.SetConfig(['Config.in','ConfigLinux.in'])
	
	# name of the dependency
	myModule.AddModuleDepend(['etk', 'freetype', 'tinyxml', 'png', 'parsersvg', 'date'])
	
	#ifeq ("$(CONFIG_BUILD_BULLET)","y")
	#myModule.AddModuleDepend('bullet')
	#endif
	#ifeq ("$(CONFIG_BUILD_LUA)","y")
	#myModule.AddModuleDepend('lua')
	#endif
	#ifeq ("$(CONFIG_BUILD_PORTAUDIO)","y")
	#myModule.AddModuleDepend('portaudio')
	#endif
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__))
	
	myModule.CompileFlags_CC([
		'-Wno-write-strings',
		'-Wall'])
	
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
		myModule.AddSrcFile('ewol/renderer/os/gui.X11.cpp')
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
		
		#tmp_src=lutinTools.GetCurrentPath(__file__) + "/ewol/renderer/os/gui.Android.base.cpp"
		#tmp_dst=lutinTools.GetCurrentPath(__file__) + "/ewol/renderer/os/gui.Android.tmp.cpp"
		
		# TODO : A really work to do here ...
		#lutinTools.CopyFile(tmp_src,tmp_dst)
		#os.system("sed -i \"s|__PROJECT_ORG_TYPE__|org|\" " + tmp_dst)
		#os.system("sed -i \"s|__PROJECT_VENDOR__|edouarddupin|\" " + tmp_dst)
		#os.system("sed -i \"s|__PROJECT_NAME__|edn|\" "+ tmp_dst)
		#os.system("sed -i \"s|__PROJECT_PACKAGE__|edn|\" " + tmp_dst)
		myModule.AddSrcFile("ewol/renderer/os/gui.Android.cpp")
	
	elif target.name=="Windows":
		myModule.AddModuleDepend("glew")
		myModule.AddSrcFile("ewol/renderer/os/gui.Windows.cpp")
	elif target.name=="MacOs":
		myModule.AddExportflag_LD([
			"-framework Cocoa",
			"-framework OpenGL",
			"-framework QuartzCore",
			"-framework AppKit"])
		myModule.AddSrcFile([
			"ewol/renderer/os/gui.MacOs.cpp",
			"ewol/renderer/os/gui.MacOs.Interface.mm",
			"ewol/renderer/os/gui.MacOs.AppDelegate.mm",
			"ewol/renderer/os/gui.MacOs.OpenglView.mm"])
	else:
		debug.error("unknow mode...")
	
	# add the currrent module at the 
	return myModule


def GetDesc():
	return "ewol is a main library to use widget in the openGl environement and manage all the wraping os"


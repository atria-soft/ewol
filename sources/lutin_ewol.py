#!/usr/bin/python
import lutinModule as module
import lutinTools as tools
import os
import lutinMultiprocess

def get_desc():
	return "ewol is a main library to use widget in the openGl environement and manage all the wraping os"



def create(target):
	# set the ewol folder for Android basic sources ...
	target.set_ewol_folder(tools.get_current_path(__file__) + "/../")
	
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'ewol', 'LIBRARY')
	
	# add extra compilation flags :
	myModule.add_extra_compile_flags()
	# add the file to compile:
	myModule.add_src_file([
		'ewol/ewol.cpp',
		'ewol/debug.cpp',
		'ewol/Padding.cpp',
		'ewol/Dimension.cpp',
		'ewol/translate.cpp'
		])
	
	# compositing :
	myModule.add_src_file([
		'ewol/compositing/Compositing.cpp',
		'ewol/compositing/TextBase.cpp',
		'ewol/compositing/Text.cpp',
		'ewol/compositing/TextDF.cpp',
		'ewol/compositing/Drawing.cpp',
		'ewol/compositing/Image.cpp',
		'ewol/compositing/Sprite.cpp',
		'ewol/compositing/Shaper.cpp',
		'ewol/compositing/Area.cpp'
		])
	
	# context :
	myModule.add_src_file([
		'ewol/context/clipBoard.cpp',
		'ewol/context/commandLine.cpp',
		'ewol/context/ConfigFont.cpp',
		'ewol/context/Context.cpp',
		'ewol/context/cursor.cpp',
		'ewol/context/InputManager.cpp'
		])
	if target.name=="Linux":
		myModule.add_src_file('ewol/context/X11/Context.cpp')
	elif target.name=="Windows":
		myModule.add_src_file('ewol/context/Windows/Context.cpp')
	elif target.name=="Android":
		myModule.add_src_file('ewol/context/Android/Context.cpp')
	elif target.name=="MacOs":
		myModule.add_src_file([
			'ewol/context/MacOs/Context.cpp',
			'ewol/context/MacOs/Interface.mm',
			'ewol/context/MacOs/Windows.mm',
			'ewol/context/MacOs/OpenglView.mm',
			'ewol/context/MacOs/AppDelegate.mm'])
	elif target.name=="IOs":
		myModule.add_src_file([
			'ewol/context/IOs/Context.cpp',
			'ewol/context/IOs/Interface.m',
			'ewol/context/IOs/OpenglView.mm',
			'ewol/context/IOs/AppDelegate.mm'])
	else:
		debug.error("unknow mode...")
	
	# event properties :
	myModule.add_src_file([
		'ewol/event/Entry.cpp',
		'ewol/event/Time.cpp',
		'ewol/event/Input.cpp'
		])
	
	# Key properties :
	myModule.add_src_file([
		'ewol/key/keyboard.cpp',
		'ewol/key/Special.cpp',
		'ewol/key/status.cpp',
		'ewol/key/type.cpp'
		])
	
	# object :
	myModule.add_src_file([
		'ewol/object/Config.cpp',
		'ewol/object/ConfigElement.cpp',
		'ewol/object/Manager.cpp',
		'ewol/object/Message.cpp',
		'ewol/object/MultiCast.cpp',
		'ewol/object/Object.cpp',
		'ewol/object/RemoveEvent.cpp',
		'ewol/object/Parameter.cpp',
		'ewol/object/ParameterList.cpp',
		'ewol/object/ParamList.cpp',
		'ewol/object/Param.cpp',
		'ewol/object/ParamRange.cpp'
		])
	
	# OpenGL interface :
	myModule.add_src_file([
		'ewol/openGL/openGL.cpp'
		])
	
	# resources :
	myModule.add_src_file([
		'ewol/resource/Colored3DObject.cpp',
		'ewol/resource/ColorFile.cpp',
		'ewol/resource/ConfigFile.cpp',
		'ewol/resource/FontFreeType.cpp',
		'ewol/resource/Image.cpp',
		'ewol/resource/ImageDF.cpp',
		'ewol/resource/Manager.cpp',
		'ewol/resource/Program.cpp',
		'ewol/resource/Resource.cpp',
		'ewol/resource/Shader.cpp',
		'ewol/resource/Texture.cpp',
		'ewol/resource/TexturedFont.cpp',
		'ewol/resource/DistanceFieldFont.cpp',
		'ewol/resource/VirtualBufferObject.cpp'
		])
	
	# widget :
	myModule.add_src_file([
		'ewol/widget/ButtonColor.cpp',
		'ewol/widget/Button.cpp',
		'ewol/widget/CheckBox.cpp',
		'ewol/widget/ColorBar.cpp',
		'ewol/widget/Composer.cpp',
		'ewol/widget/Container.cpp',
		'ewol/widget/Container2.cpp',
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
		'ewol/widget/Widget.cpp',
		'ewol/widget/WidgetScrolled.cpp',
		'ewol/widget/Windows.cpp',
		'ewol/widget/WSlider.cpp',
		])
	
	myModule.copy_folder('../data/theme/shape/square/*','theme/shape/square')
	myModule.copy_folder('../data/theme/shape/round/*','theme/shape/round')
	myModule.copy_folder('../data/theme/color/black/*','theme/color/black')
	myModule.copy_folder('../data/theme/color/white/*','theme/color/white')
	myModule.copy_folder('../data/textured.*','')
	myModule.copy_folder('../data/texturedNoMaterial.*','')
	myModule.copy_folder('../data/text.*','')
	myModule.copy_folder('../data/simple3D.*','')
	myModule.copy_folder('../data/color.*','')
	myModule.copy_folder('../data/color3.*','')
	myModule.copy_folder('../data/textured3D2.*','')
	myModule.copy_folder('../data/textured3D.*','')
	myModule.copy_folder('../data/texturedDF.*','')
	myModule.copy_folder('../data/fontDistanceField/*','fontDistanceField')
	myModule.copy_folder('../data/translate/*','translate/ewol/')
	
	# name of the dependency
	myModule.add_module_depend(['etk', 'freetype', 'exml', 'ejson', 'egami', 'edtaa3', 'date'])
	
	myModule.add_export_path(tools.get_current_path(__file__))

	tagFile = tools.get_current_path(__file__) + "/tag"
	ewolVersionID = tools.file_read_data(tagFile)
	myModule.compile_flags_CC([
		"-DEWOL_VERSION=\"\\\""+ewolVersionID+"\\\"\""
		])
	
	if target.name=="Linux":
		myModule.add_export_flag_LD('-lGL')
		
		#`pkg-config --cflags directfb` `pkg-config --libs directfb`
		
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_X11__)","y")
		myModule.add_export_flag_LD('-lX11')
		#endif
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_DIRECT_FB__)","y")
		#myModule.add_export_flag_LD(['-L/usr/local/lib', '-ldirectfb', '-lfusion', '-ldirect'])
		#endif
		
		#http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
		# needed package on linux : libgl1-mesa-dev libglew1.5-dev
		
		#ifeq ("$(CONFIG_BUILD_PORTAUDIO)","y")
		#myModule.add_src_file('ewol/renderer/audio/interfacePortAudio.cpp')
		#endif
	
	elif target.name=="Android":
		myModule.add_export_flag_LD("-lGLESv2")
		
		myModule.add_export_flag_LD("-ldl")
		myModule.add_export_flag_LD("-llog")
		myModule.add_export_flag_LD("-landroid")
		java_tmp_dir = tools.get_current_path(__file__) + "/../../ewol/sources/android/src/"
		cpp_tmp_dir = tools.get_current_path(__file__) + "/ewol/renderer/Android/"
		java_tmp_src = java_tmp_dir + "org/ewol/EwolConstants"
		lutinMultiprocess.run_command("javac " + java_tmp_src + ".java")
		lutinMultiprocess.run_command("cd " + java_tmp_dir + " && javah org.ewol.EwolConstants")
		tools.copy_file(java_tmp_dir + "org_ewol_EwolConstants.h", cpp_tmp_dir + "org_ewol_EwolConstants.h", force=True)
		tools.remove_file(java_tmp_src + ".class")
	elif target.name=="Windows":
		myModule.add_module_depend("glew")
	elif target.name=="MacOs":
		myModule.add_export_flag_LD([
			"-framework Cocoa",
			"-framework OpenGL",
			"-framework QuartzCore",
			"-framework AppKit"])
	elif target.name=="IOs":
		myModule.add_export_flag_LD([
			"-framework OpenGLES",
			"-framework CoreGraphics",
			"-framework UIKit",
			"-framework GLKit",
			"-framework Foundation",
			"-framework QuartzCore"])

	# add the currrent module at the 
	return myModule


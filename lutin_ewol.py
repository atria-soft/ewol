#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug
import os
import lutin.multiprocess as lutinMultiprocess

def get_desc():
	return "ewol is a main library to use widget in the openGl environement and manage all the wraping os"

def get_license():
	return "APACHE v2.0"

def create(target):
	# set the ewol folder for Android basic sources ...
	# TODO: Remove this really bad interface :
	target.set_ewol_folder(tools.get_current_path(__file__))
	
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
		'ewol/object/Manager.cpp',
		'ewol/object/Object.cpp',
		'ewol/object/Worker.cpp'
		])
	# parameter :
	myModule.add_src_file([
		'ewol/parameter/Parameter.cpp',
		'ewol/parameter/Interface.cpp',
		])
	# Signal :
	myModule.add_src_file([
		'ewol/signal/Interface.cpp',
		'ewol/signal/Base.cpp'
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
		'ewol/resource/Resource.cpp',
		'ewol/resource/TexturedFont.cpp',
		'ewol/resource/DistanceFieldFont.cpp'
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
	
	myModule.copy_folder('data/theme/shape/square/*','theme/shape/square')
	myModule.copy_folder('data/theme/shape/round/*','theme/shape/round')
	myModule.copy_folder('data/theme/color/black/*','theme/color/black')
	myModule.copy_folder('data/theme/color/white/*','theme/color/white')
	myModule.copy_folder('data/textured.*','')
	myModule.copy_folder('data/texturedNoMaterial.*','')
	myModule.copy_folder('data/text.*','')
	myModule.copy_folder('data/simple3D.*','')
	myModule.copy_folder('data/color.*','')
	myModule.copy_folder('data/color3.*','')
	myModule.copy_folder('data/textured3D2.*','')
	myModule.copy_folder('data/textured3D.*','')
	myModule.copy_folder('data/texturedDF.*','')
	myModule.copy_folder('data/fontDistanceField/*','fontDistanceField')
	myModule.copy_folder('data/translate/*','translate/ewol/')
	
	# name of the dependency
	myModule.add_module_depend(['etk', 'gale', 'freetype', 'exml', 'ejson', 'egami', 'edtaa3', 'date'])
	
	myModule.add_export_path(tools.get_current_path(__file__))

	tagFile = tools.get_current_path(__file__) + "/tag"
	ewolVersionID = tools.file_read_data(tagFile)
	myModule.compile_flags('c++', [
		"-DEWOL_VERSION=\"\\\""+ewolVersionID+"\\\"\""
		])
	
	return myModule


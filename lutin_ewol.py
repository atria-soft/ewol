#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug
import os
import lutin.multiprocess as lutinMultiprocess


def get_type():
	return "LIBRARY"

def get_desc():
	return "ewol is a widget management library"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def create(target, module_name):
	# module name is 'edn' and type binary.
	my_module = module.Module(__file__, module_name, get_type())
	
	# add extra compilation flags:
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
		'ewol/ewol.cpp',
		'ewol/debug.cpp',
		'ewol/Padding.cpp',
		'ewol/translate.cpp',
		'ewol/DrawProperty.cpp',
		'ewol/gravity.cpp'
		])
	my_module.add_header_file([
		'ewol/debug.h', # TODO : Remove this ...
		'ewol/ewol.h',
		'ewol/Padding.h',
		'ewol/translate.h',
		'ewol/DrawProperty.h',
		'ewol/gravity.h'
		])
	
	# compositing:
	my_module.add_src_file([
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
	my_module.add_header_file([
		'ewol/compositing/Text.h',
		'ewol/compositing/Drawing.h',
		'ewol/compositing/Sprite.h',
		'ewol/compositing/Area.h',
		'ewol/compositing/Shaper.h',
		'ewol/compositing/TextDF.h',
		'ewol/compositing/TextBase.h',
		'ewol/compositing/Compositing.h',
		'ewol/compositing/Image.h'
		])
	
	# context:
	my_module.add_src_file([
		'ewol/context/ConfigFont.cpp',
		'ewol/context/Context.cpp',
		'ewol/context/Application.cpp',
		'ewol/context/InputManager.cpp'
		])
	my_module.add_header_file([
		'ewol/context/ConfigFont.h',
		'ewol/context/Context.h',
		'ewol/context/Application.h',
		'ewol/context/InputManager.h'
		])
	
	# event properties:
	my_module.add_src_file([
		'ewol/event/Entry.cpp',
		'ewol/event/Time.cpp',
		'ewol/event/Input.cpp'
		])
	my_module.add_header_file([
		'ewol/event/Time.h',
		'ewol/event/Input.h',
		'ewol/event/Entry.h'
		])
	
	# object:
	my_module.add_src_file([
		'ewol/object/Manager.cpp',
		'ewol/object/Object.cpp',
		'ewol/object/Worker.cpp'
		])
	my_module.add_header_file([
		'ewol/object/Worker.h',
		'ewol/object/Manager.h',
		'ewol/object/Object.h'
		])
	
	# resources:
	my_module.add_src_file([
		'ewol/resource/Colored3DObject.cpp',
		'ewol/resource/ColorFile.cpp',
		'ewol/resource/ConfigFile.cpp',
		'ewol/resource/FontFreeType.cpp',
		'ewol/resource/Image.cpp',
		'ewol/resource/ImageDF.cpp',
		'ewol/resource/Texture.cpp',
		'ewol/resource/TexturedFont.cpp',
		'ewol/resource/DistanceFieldFont.cpp'
		])
	my_module.add_header_file([
		'ewol/resource/FontFreeType.h',
		'ewol/resource/TexturedFont.h',
		'ewol/resource/ColorFile.h',
		'ewol/resource/font/FontBase.h',
		'ewol/resource/font/Kerning.h',
		'ewol/resource/font/GlyphProperty.h',
		'ewol/resource/DistanceFieldFont.h',
		'ewol/resource/ImageDF.h',
		'ewol/resource/Colored3DObject.h',
		'ewol/resource/ConfigFile.h',
		'ewol/resource/Texture.h',
		'ewol/resource/Image.h'
		])
	
	# widget:
	my_module.add_src_file([
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
		'ewol/widget/meta/SpinBase.cpp',
		'ewol/widget/PopUp.cpp',
		'ewol/widget/ProgressBar.cpp',
		'ewol/widget/Scroll.cpp',
		'ewol/widget/Select.cpp',
		'ewol/widget/Sizer.cpp',
		'ewol/widget/Slider.cpp',
		'ewol/widget/Spacer.cpp',
		'ewol/widget/Widget.cpp',
		'ewol/widget/WidgetScrolled.cpp',
		'ewol/widget/Windows.cpp',
		'ewol/widget/WSlider.cpp',
		'ewol/widget/Spin.cpp',
		])
	my_module.add_header_file([
		'ewol/widget/Menu.h',
		'ewol/widget/Slider.h',
		'ewol/widget/WidgetScrolled.h',
		'ewol/widget/ListFileSystem.h',
		'ewol/widget/Panned.h',
		'ewol/widget/WSlider.h',
		'ewol/widget/Container2.h',
		'ewol/widget/Windows.h',
		'ewol/widget/CheckBox.h',
		'ewol/widget/Container.h',
		'ewol/widget/PopUp.h',
		'ewol/widget/Label.h',
		'ewol/widget/Composer.h',
		'ewol/widget/Sizer.h',
		'ewol/widget/Scroll.h',
		'ewol/widget/ContainerN.h',
		'ewol/widget/Spacer.h',
		'ewol/widget/Button.h',
		'ewol/widget/Manager.h',
		'ewol/widget/Entry.h',
		'ewol/widget/ContextMenu.h',
		'ewol/widget/Gird.h',
		'ewol/widget/ProgressBar.h',
		'ewol/widget/ColorBar.h',
		'ewol/widget/ButtonColor.h',
		'ewol/widget/Layer.h',
		'ewol/widget/Joystick.h',
		'ewol/widget/Widget.h',
		'ewol/widget/meta/StdPopUp.h',
		'ewol/widget/meta/SpinBase.h',
		'ewol/widget/meta/ParameterList.h',
		'ewol/widget/meta/ColorChooser.h',
		'ewol/widget/meta/Parameter.h',
		'ewol/widget/meta/FileChooser.h',
		'ewol/widget/Image.h',
		'ewol/widget/List.h',
		'ewol/widget/Select.h',
		'ewol/widget/Spin.h'
		])
	
	# tools:
	my_module.add_src_file([
		'ewol/tools/message.cpp'
		])
	my_module.add_header_file([
		'ewol/tools/message.h'
		])
	
	my_module.copy_path('data/theme/shape/square/*','theme/shape/square')
	my_module.copy_path('data/theme/shape/round/*','theme/shape/round')
	my_module.copy_path('data/theme/color/black/*','theme/color/black')
	my_module.copy_path('data/theme/color/white/*','theme/color/white')
	my_module.copy_path('data/textured.*','')
	my_module.copy_path('data/texturedNoMaterial.*','')
	my_module.copy_path('data/text.*','')
	my_module.copy_path('data/simple3D.*','')
	my_module.copy_path('data/color.*','')
	my_module.copy_path('data/color3.*','')
	my_module.copy_path('data/textured3D2.*','')
	my_module.copy_path('data/textured3D.*','')
	my_module.copy_path('data/texturedDF.*','')
	my_module.copy_path('data/fontDistanceField/*','fontDistanceField')
	my_module.copy_path('data/translate/*','translate/ewol')
	
	# name of the dependency
	my_module.add_depend([
	    'elog',
	    'etk',
	    'esignal',
	    'eproperty',
	    'ememory',
	    'gale',
	    'freetype',
	    'exml',
	    'ejson',
	    'egami',
	    'edtaa3'])
	
	my_module.add_path(tools.get_current_path(__file__))
	
	my_module.add_flag('c++', [
		"-DEWOL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	
	return my_module


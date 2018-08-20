#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "ewol is a widget management library"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	# add extra compilation flags:
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
	    'ewol/ewol.cpp',
	    'ewol/debug.cpp',
	    'ewol/Padding.cpp',
	    'ewol/DrawProperty.cpp',
	    'ewol/gravity.cpp'
	    ])
	my_module.add_header_file([
	    'ewol/debug.hpp', # TODO : Remove this ...
	    'ewol/ewol.hpp',
	    'ewol/Padding.hpp',
	    'ewol/DrawProperty.hpp',
	    'ewol/gravity.hpp'
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
	    'ewol/compositing/Text.hpp',
	    'ewol/compositing/Drawing.hpp',
	    'ewol/compositing/Sprite.hpp',
	    'ewol/compositing/Area.hpp',
	    'ewol/compositing/Shaper.hpp',
	    'ewol/compositing/TextDF.hpp',
	    'ewol/compositing/TextBase.hpp',
	    'ewol/compositing/Compositing.hpp',
	    'ewol/compositing/Image.hpp'
	    ])
	
	# context:
	my_module.add_src_file([
	    'ewol/context/ConfigFont.cpp',
	    'ewol/context/Context.cpp',
	    'ewol/context/Application.cpp',
	    'ewol/context/InputManager.cpp'
	    ])
	my_module.add_header_file([
	    'ewol/context/ConfigFont.hpp',
	    'ewol/context/Context.hpp',
	    'ewol/context/Application.hpp',
	    'ewol/context/InputManager.hpp'
	    ])
	
	# event properties:
	my_module.add_src_file([
	    'ewol/event/Entry.cpp',
	    'ewol/event/Time.cpp',
	    'ewol/event/Input.cpp'
	    ])
	my_module.add_header_file([
	    'ewol/event/Time.hpp',
	    'ewol/event/Input.hpp',
	    'ewol/event/Entry.hpp'
	    ])
	
	# object:
	my_module.add_src_file([
	    'ewol/object/Manager.cpp',
	    'ewol/object/Object.cpp',
	    'ewol/object/Worker.cpp'
	    ])
	my_module.add_header_file([
	    'ewol/object/Worker.hpp',
	    'ewol/object/Manager.hpp',
	    'ewol/object/Object.hpp'
	    ])
	
	# resources:
	my_module.add_src_file([
	    'ewol/resource/Colored3DObject.cpp',
	    'ewol/resource/ColorFile.cpp',
	    'ewol/resource/ConfigFile.cpp',
	    'ewol/resource/FontFreeType.cpp',
	    'ewol/resource/TextureFile.cpp',
	    'ewol/resource/ImageDF.cpp',
	    'ewol/resource/Texture.cpp',
	    'ewol/resource/TexturedFont.cpp',
	    'ewol/resource/DistanceFieldFont.cpp'
	    ])
	my_module.add_header_file([
	    'ewol/resource/FontFreeType.hpp',
	    'ewol/resource/TexturedFont.hpp',
	    'ewol/resource/ColorFile.hpp',
	    'ewol/resource/font/FontBase.hpp',
	    'ewol/resource/font/Kerning.hpp',
	    'ewol/resource/font/GlyphProperty.hpp',
	    'ewol/resource/DistanceFieldFont.hpp',
	    'ewol/resource/ImageDF.hpp',
	    'ewol/resource/Colored3DObject.hpp',
	    'ewol/resource/ConfigFile.hpp',
	    'ewol/resource/Texture.hpp',
	    'ewol/resource/TextureFile.hpp'
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
	    'ewol/widget/TreeView.cpp',
	    ])
	my_module.add_header_file([
	    'ewol/widget/Menu.hpp',
	    'ewol/widget/Slider.hpp',
	    'ewol/widget/WidgetScrolled.hpp',
	    'ewol/widget/ListFileSystem.hpp',
	    'ewol/widget/Panned.hpp',
	    'ewol/widget/WSlider.hpp',
	    'ewol/widget/Container2.hpp',
	    'ewol/widget/Windows.hpp',
	    'ewol/widget/CheckBox.hpp',
	    'ewol/widget/Container.hpp',
	    'ewol/widget/PopUp.hpp',
	    'ewol/widget/Label.hpp',
	    'ewol/widget/Composer.hpp',
	    'ewol/widget/Sizer.hpp',
	    'ewol/widget/Scroll.hpp',
	    'ewol/widget/ContainerN.hpp',
	    'ewol/widget/Spacer.hpp',
	    'ewol/widget/Button.hpp',
	    'ewol/widget/Manager.hpp',
	    'ewol/widget/Entry.hpp',
	    'ewol/widget/ContextMenu.hpp',
	    'ewol/widget/Gird.hpp',
	    'ewol/widget/ProgressBar.hpp',
	    'ewol/widget/ColorBar.hpp',
	    'ewol/widget/ButtonColor.hpp',
	    'ewol/widget/Layer.hpp',
	    'ewol/widget/Joystick.hpp',
	    'ewol/widget/Widget.hpp',
	    'ewol/widget/meta/StdPopUp.hpp',
	    'ewol/widget/meta/SpinBase.hpp',
	    'ewol/widget/meta/ParameterList.hpp',
	    'ewol/widget/meta/ColorChooser.hpp',
	    'ewol/widget/meta/Parameter.hpp',
	    'ewol/widget/meta/FileChooser.hpp',
	    'ewol/widget/Image.hpp',
	    'ewol/widget/List.hpp',
	    'ewol/widget/Select.hpp',
	    'ewol/widget/Spin.hpp',
	    'ewol/widget/TreeView.hpp',
	    ])
	
	# tools:
	my_module.add_src_file([
	    'ewol/tools/message.cpp'
	    ])
	my_module.add_header_file([
	    'ewol/tools/message.hpp'
	    ])
	
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
	    'edtaa3',
	    'etranslate',
	    'ewol-data',
	    'fluorine',
	    ])
	
	my_module.add_path(".")
	
	my_module.add_flag('c++', [
	    "-DEWOL_VERSION=\"\\\"" + tools.version_to_string(my_module.get_pkg("VERSION")) + "\\\"\""
	    ])
	
	return True


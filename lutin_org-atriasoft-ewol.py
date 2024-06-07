#!/usr/bin/python
import realog.debug as debug
import lutin.tools as tools
import realog.debug as debug
import lutin.image as image
import os
import lutin.multiprocess as lutinMultiprocess


def get_type():
	return "LIBRARY_DYNAMIC"

def get_desc():
	return "Ewol Tool Kit"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "org"

def get_compagny_name():
	return "atria-soft"

#def get_maintainer():
#	return "authors.txt"

#def get_version():
#	return "version.txt"

def configure(target, my_module):

	my_module.add_src_file([
	    'src/module-info.java',
	    'src/org/atriasoft/ewol/DrawProperty.java',
	    'src/org/atriasoft/ewol/internal/LoadPackageStream.java',
	    'src/org/atriasoft/ewol/internal/LOGGER.java',
	    'src/org/atriasoft/ewol/context/EwolContext.java',
	    'src/org/atriasoft/ewol/context/EwolApplication.java',
	    'src/org/atriasoft/ewol/context/InputManager.java',
	    'src/org/atriasoft/ewol/Padding.java',
	    'src/org/atriasoft/ewol/annotation/EwolDescription.java',
	    'src/org/atriasoft/ewol/annotation/EwolAnnotation.java',
	    'src/org/atriasoft/ewol/annotation/EwolSignal.java',
	    'src/org/atriasoft/ewol/resource/RefactorColored3DObject.java',
	    'src/org/atriasoft/ewol/resource/font/FontMode.java',
	    'src/org/atriasoft/ewol/resource/font/GlyphProperty.java',
	    'src/org/atriasoft/ewol/resource/ResourceTexturedFont.java',
	    'src/org/atriasoft/ewol/resource/ResourceFontSvg.java',
	    'src/org/atriasoft/ewol/resource/ResourceColorFile.java',
	    'src/org/atriasoft/ewol/resource/ResourceConfigFile.java',
	    'src/org/atriasoft/ewol/event/EntrySystem.java',
	    'src/org/atriasoft/ewol/event/EventInput.java',
	    'src/org/atriasoft/ewol/event/InputSystem.java',
	    'src/org/atriasoft/ewol/event/EventShortCut.java',
	    'src/org/atriasoft/ewol/event/EventEntry.java',
	    'src/org/atriasoft/ewol/event/EventTime.java',
	    'src/org/atriasoft/ewol/widget/Windows.java',
	    'src/org/atriasoft/ewol/widget/Entry.java',
	    'src/org/atriasoft/ewol/widget/ImageDisplay.java',
	    'src/org/atriasoft/ewol/widget/WidgetManager.java',
	    'src/org/atriasoft/ewol/widget/Label.java',
	    'src/org/atriasoft/ewol/widget/ProgressBar.java',
	    'src/org/atriasoft/ewol/widget/Widget.java',
	    'src/org/atriasoft/ewol/widget/Container.java',
	    'src/org/atriasoft/ewol/widget/Spacer.java',
	    'src/org/atriasoft/ewol/widget/Sizer.java',
	    'src/org/atriasoft/ewol/widget/Button.java',
	    'src/org/atriasoft/ewol/widget/ContainerN.java',
	    'src/org/atriasoft/ewol/Gravity.java',
	    'src/org/atriasoft/ewol/tools/Message.java',
	    'src/org/atriasoft/ewol/Ewol.java',
	    'src/org/atriasoft/ewol/object/Worker.java',
	    'src/org/atriasoft/ewol/object/EwolObject.java',
	    'src/org/atriasoft/ewol/object/ObjectManager.java',
	    'src/org/atriasoft/ewol/compositing/CompositingGraphicContext.java',
	    'src/org/atriasoft/ewol/compositing/AlignMode.java',
	    'src/org/atriasoft/ewol/compositing/CompositingText.java',
	    'src/org/atriasoft/ewol/compositing/CompositingDrawing.java',
	    'src/org/atriasoft/ewol/compositing/Compositing.java',
	    'src/org/atriasoft/ewol/compositing/CompositingImage.java',
	    'src/org/atriasoft/ewol/compositing/tools/TextDecoration.java',
	    'src/org/atriasoft/ewol/compositing/TextBase.java',
	    'src/org/atriasoft/ewol/compositing/GuiShapeMode.java',
	    'src/org/atriasoft/ewol/compositing/GuiShape.java',
	    'src/org/atriasoft/etranslate/ETranslate.java',
	    ])
	my_module.add_path('src/', type='java')
	
	my_module.add_depend([
	    'org-atriasoft-gale',
	    'org-atriasoft-iogami',
	    'org-atriasoft-esvg',
	    'org-atriasoft-ejson',
	    'org-atriasoft-exml',
	    'org-atriasoft-esignal',
	    'org-atriasoft-loader3d',
	    ])
	
	#my_module.add_path([
	#    'lib/spotbugs-annotations-4.2.2.jar'
	#    ],
	#    type='java',
	#    export=True
	#);
	my_module.add_flag('java', "RELEASE_15_PREVIEW");
	
	return True


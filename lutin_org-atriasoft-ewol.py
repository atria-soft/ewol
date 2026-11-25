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
	    'src/org/atriasoft/etranslate/ETranslate.java',
	    'src/org/atriasoft/ewol/compositing/AlignMode.java',
	    'src/org/atriasoft/ewol/compositing/Compositing.java',
	    'src/org/atriasoft/ewol/compositing/CompositingDraw.java',
	    'src/org/atriasoft/ewol/compositing/CompositingDrawInterface.java',
	    'src/org/atriasoft/ewol/compositing/CompositingDrawing.java',
	    'src/org/atriasoft/ewol/compositing/CompositingGC.java',
	    'src/org/atriasoft/ewol/compositing/CompositingImage.java',
	    'src/org/atriasoft/ewol/compositing/CompositingSVG.java',
	    'src/org/atriasoft/ewol/compositing/CompositingText.java',
	    'src/org/atriasoft/ewol/compositing/GuiShapeMode.java',
	    'src/org/atriasoft/ewol/compositing/ShapeBox.java',
	    'src/org/atriasoft/ewol/compositing/TextBase.java',
	    'src/org/atriasoft/ewol/compositing/tools/TextDecoration.java',
	    'src/org/atriasoft/ewol/context/EwolApplication.java',
	    'src/org/atriasoft/ewol/context/EwolContext.java',
	    'src/org/atriasoft/ewol/context/InputManager.java',
	    'src/org/atriasoft/ewol/DrawProperty.java',
	    'src/org/atriasoft/ewol/event/EntrySystem.java',
	    'src/org/atriasoft/ewol/event/EventEntry.java',
	    'src/org/atriasoft/ewol/event/EventInput.java',
	    'src/org/atriasoft/ewol/event/EventShortCut.java',
	    'src/org/atriasoft/ewol/event/EventTime.java',
	    'src/org/atriasoft/ewol/event/InputSystem.java',
	    'src/org/atriasoft/ewol/Ewol.java',
	    'src/org/atriasoft/ewol/Gravity.java',
	    'src/org/atriasoft/ewol/GravityDepth.java',
	    'src/org/atriasoft/ewol/GravityHorizontal.java',
	    'src/org/atriasoft/ewol/GravityVertical.java',
	    'src/org/atriasoft/ewol/HighSpeedMode.java',
	    'src/org/atriasoft/ewol/internal/EwolJacksonModule.java',
	    'src/org/atriasoft/ewol/internal/JsonHelper.java',
	    'src/org/atriasoft/ewol/internal/LoadPackageStream.java',
	    'src/org/atriasoft/ewol/internal/WidgetDeserializer.java',
	    'src/org/atriasoft/ewol/object/EwolObject.java',
	    'src/org/atriasoft/ewol/object/ObjectManager.java',
	    'src/org/atriasoft/ewol/object/Worker.java',
	    'src/org/atriasoft/ewol/Padding.java',
	    'src/org/atriasoft/ewol/resource/font/FontMode.java',
	    'src/org/atriasoft/ewol/resource/font/GlyphProperty.java',
	    'src/org/atriasoft/ewol/resource/RefactorColored3DObject.java',
	    'src/org/atriasoft/ewol/resource/ResourceColorFile.java',
	    'src/org/atriasoft/ewol/resource/ResourceConfigFile.java',
	    'src/org/atriasoft/ewol/resource/ResourceFontSvg.java',
	    'src/org/atriasoft/ewol/resource/ResourceTexturedFont.java',
	    'src/org/atriasoft/ewol/tools/Message.java',
	    'src/org/atriasoft/ewol/widget/Box.java',
	    'src/org/atriasoft/ewol/widget/Button.java',
	    'src/org/atriasoft/ewol/widget/CheckBox.java',
	    'src/org/atriasoft/ewol/widget/ColorGradient.java',
	    'src/org/atriasoft/ewol/widget/ColorPicker.java',
	    'src/org/atriasoft/ewol/widget/Composer.java',
	    'src/org/atriasoft/ewol/widget/Container.java',
	    'src/org/atriasoft/ewol/widget/ContainerN.java',
	    'src/org/atriasoft/ewol/widget/ContainerToggle.java',
	    'src/org/atriasoft/ewol/widget/debug/WidgetInspector.java',
	    'src/org/atriasoft/ewol/widget/Dialog.java',
	    'src/org/atriasoft/ewol/widget/Entry.java',
	    'src/org/atriasoft/ewol/widget/FieldSet.java',
	    'src/org/atriasoft/ewol/widget/Gird.java',
	    'src/org/atriasoft/ewol/widget/Icon.java',
	    'src/org/atriasoft/ewol/widget/ImageDisplay.java',
	    'src/org/atriasoft/ewol/widget/Label.java',
	    'src/org/atriasoft/ewol/widget/LabelOnSVG.java',
	    'src/org/atriasoft/ewol/widget/Layer.java',
	    'src/org/atriasoft/ewol/widget/ListFileSystem.java',
	    'src/org/atriasoft/ewol/widget/menu/MenuBar.java',
	    'src/org/atriasoft/ewol/widget/menu/MenuItem.java',
	    'src/org/atriasoft/ewol/widget/menu/MenuPopup.java',
	    'src/org/atriasoft/ewol/widget/menu/MenuSeparator.java',
	    'src/org/atriasoft/ewol/widget/menu/ShortcutBadge.java',
	    'src/org/atriasoft/ewol/widget/menu/ShortcutFormatter.java',
	    'src/org/atriasoft/ewol/widget/meta/ColorPickerPopup.java',
	    'src/org/atriasoft/ewol/widget/meta/FileChooser.java',
	    'src/org/atriasoft/ewol/widget/meta/NumberInputBase.java',
	    'src/org/atriasoft/ewol/widget/meta/Popover.java',
	    'src/org/atriasoft/ewol/widget/meta/PopoverManager.java',
	    'src/org/atriasoft/ewol/widget/meta/PopoverPosition.java',
	    'src/org/atriasoft/ewol/widget/meta/PopoverTrigger.java',
	    'src/org/atriasoft/ewol/widget/meta/SelectPopup.java',
	    'src/org/atriasoft/ewol/widget/model/ListRole.java',
	    'src/org/atriasoft/ewol/widget/model/SpinPosition.java',
	    'src/org/atriasoft/ewol/widget/notification/DefaultNotificationManager.java',
	    'src/org/atriasoft/ewol/widget/notification/NotificationManager.java',
	    'src/org/atriasoft/ewol/widget/notification/Toast.java',
	    'src/org/atriasoft/ewol/widget/notification/ToastConfig.java',
	    'src/org/atriasoft/ewol/widget/notification/ToastType.java',
	    'src/org/atriasoft/ewol/widget/NumberInput.java',
	    'src/org/atriasoft/ewol/widget/PopUp.java',
	    'src/org/atriasoft/ewol/widget/ProgressBar.java',
	    'src/org/atriasoft/ewol/widget/ScrollView.java',
	    'src/org/atriasoft/ewol/widget/Select.java',
	    'src/org/atriasoft/ewol/widget/Sizer.java',
	    'src/org/atriasoft/ewol/widget/Slider.java',
	    'src/org/atriasoft/ewol/widget/Spacer.java',
	    'src/org/atriasoft/ewol/widget/SplitPane.java',
	    'src/org/atriasoft/ewol/widget/Tick.java',
	    'src/org/atriasoft/ewol/widget/TreeCellRenderer.java',
	    'src/org/atriasoft/ewol/widget/TreeColumn.java',
	    'src/org/atriasoft/ewol/widget/TreeColumnAction.java',
	    'src/org/atriasoft/ewol/widget/TreeFileSystem.java',
	    'src/org/atriasoft/ewol/widget/TreeNode.java',
	    'src/org/atriasoft/ewol/widget/TreeView.java',
	    'src/org/atriasoft/ewol/widget/Widget.java',
	    'src/org/atriasoft/ewol/widget/WidgetList.java',
	    'src/org/atriasoft/ewol/widget/WidgetManager.java',
	    'src/org/atriasoft/ewol/widget/WidgetScrolled.java',
	    'src/org/atriasoft/ewol/widget/Windows.java',
	    ])
	my_module.add_path('src/', type='java')
	
	my_module.add_depend([
	    'org-atriasoft-gale',
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


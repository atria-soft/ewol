
=== Objectif ===
:** What is a Widget
:** Simply create a complex Gui

=== What is a Widget ===

A widget is a simple entity of a graphical Object. It herited of every [class[ewol::Object]] class with many graphical interface to draw a complex gui.

We can consider some widget:
:** Windows: Main gui interface to display the unique "windows".
:** Container Widget: Widget that manage some subWidget (generic)
:** Simple widget: all widget that display somthing.
:** Meta widget: Widget composed with some wodget.

=== Simple load & configure of a widget: ===

We have 4 way to create a widget:
==== call create and configure ====

First create the widget:

[code style=c++]
	ewol::widget::ButtonShared tmpWidget = ewol::widget::Button::create();
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
[/code]

Set some parameters:

[code style=c++]
	tmpWidget->propertyName.set("my name");
	tmpWidget->propertyExpand.set(bvec2(true,false));
	tmpWidget->propertyFill.set(bvec2(true,true));
[/code]

[note]
	This is the faster way to configure your gui. and the check are done when you compile your code.
[/note]

==== Call create and direct configuration ====

We can configure the wiget before the init() is called.
[code style=c++]
	ewol::widget::ButtonShared tmpWidget = ewol::widget::Button::create(
	    "name", std::string("my name"),
	    "expand", bvec2(true,false),
	    "fill", bvec2(true,true));
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
[/code]

[note]
	The configuration is done and check at the runtime ==> you need to test it to be sure your configuration work.
[/note]


==== Call generic factory system (compositing) ====

[code style=c++]
	#include <ewol::widget::Composer.h>
	ewol::widget::WidgetShared tmpWidget = ewol::widget::composerGenerateString("<button name='my name' expand='true,false' fill='true,true'/>");
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
[/code]

With this way you just like string ...

==== Load from a xml file (compositing) ====

File gui.xml
[code]
	<button name="my name"
	        expand="true,false"
	        fill="true,true"/>
[/code]

[code style=c++]
	#include <ewol::widget::Composer.h>
	ewol::widget::WidgetShared tmpWidget = ewol::widget::composerGenerateFile("DATA:gui.xml");
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
[/code]

This last way is just to simplify gui generation


=== Now we will create a simple hierarchic gui ===

The objective is to create a text label area with 2 button centered.


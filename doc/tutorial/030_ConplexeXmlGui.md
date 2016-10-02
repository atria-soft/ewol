EWOL: Comple XML GUI                                {#ewol_tutorial_complex_xml_gui}
====================

@tableofcontents

Objectifs:                                {#ewol_tutorial_complex_xml_gui_objectif}
==========

  - What is a Widget
  - Simply create a complex Gui

What is a Widget                                {#ewol_tutorial_complex_xml_gui_what}
================

A widget is a simple entity of a graphical Object. It herited of every ewol::Object class with many graphical interface to draw a complex gui.

We can consider some widget:
  - ewol::Windows: Main gui interface to display the unique "windows".
  - Container Widget: Widget that manage some subWidget (generic)
  - Simple widget: all widget that display somthing.
  - Meta widget: Widget composed with some wodget.

Simple load & configure of a widget:                                {#ewol_tutorial_complex_xml_gui_simple}
====================================

We have 4 way to create a widget:

call create and configure
-------------------------

First create the widget:

```{.cpp}
	ewol::widget::ButtonShared tmpWidget = ewol::widget::Button::create();
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
```

Set some parameters:

```{.cpp}
	tmpWidget->propertyName.set("my name");
	tmpWidget->propertyExpand.set(bvec2(true,false));
	tmpWidget->propertyFill.set(bvec2(true,true));
```

**Note:**

```
	This is the faster way to configure your gui. and the check are done when you compile your code.
```

Call create and direct configuration
------------------------------------

We can configure the wiget before the init() is called.
```{.cpp}
	ewol::widget::ButtonShared tmpWidget = ewol::widget::Button::create(
	    "name", std::string("my name"),
	    "expand", bvec2(true,false),
	    "fill", bvec2(true,true));
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
```

**Note:**

```
	The configuration is done and check at the runtime ==> you need to test it to be sure your configuration work.
```


Call generic factory system (compositing)
-----------------------------------------

```{.cpp}
	#include <ewol::widget::Composer.hpp>
	ewol::widget::WidgetShared tmpWidget = ewol::widget::composerGenerateString("<button name='my name' expand='true,false' fill='true,true'/>");
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
```

With this way you just like string ...

Load from a xml file (compositing)
----------------------------------

File ```gui.xml```

```{.xml}
	<button name="my name"
	        expand="true,false"
	        fill="true,true"/>
```

```{.cpp}
	#include <ewol::widget::Composer.hpp>
	ewol::widget::WidgetShared tmpWidget = ewol::widget::composerGenerateFile("DATA:gui.xml");
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("The widget can not be created");
	}
```

This last way is just to simplify gui generation


Now we will create a simple hierarchic gui                                {#ewol_tutorial_complex_xml_gui_simple}
==========================================

The objective is to create a text label area with 2 button centered.

**TODO ...**

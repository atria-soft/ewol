EWOL: Add Widget in XML access                                {#ewol_tutorial_add_widget_custum_in_xml}
==============================

@tableofcontents

Objectifs:                                                    {#ewol_tutorial_add_widget_custum_in_xml_objectif}
==========

  - Create a C++ widget
  - use it in a XML


Declare object in XML                                {#ewol_tutorial_add_widget_custum_in_xml_declare}
=====================

Object can be declared in some XML, (like gui decription), then we need to declare the Object in the system recognition.

Declare Object
--------------

In your application "void onCreate(ewol::Context& _context) override" add the function:

```{.cpp}
	YourWidgetClass::createManagerWidget(_context.getWidgetManager());
```

The simple question is: I does not define this function, where it is done ?

The createManagerWidget is instancuate when you use the macro:

```{.cpp}
	DECLARE_WIDGET_FACTORY(YourWidgetClass, "YourWidgetClass");
```

it create 2 function: "create(...)" and "createManagerWidget()"

Declare on XML and configuration                                {#ewol_tutorial_add_widget_custum_in_xml_dec2}
================================

in the xml instance simply request it like:

```{.xml}
	<YourWidgetClass name="jkjkj">
		...
	</YourWidgetClass>
```

The xml attribute are automaticaly parsed to configure properties of you object (this is the reason of naming it).

Special case SubParsing XML element                                {#ewol_tutorial_add_widget_custum_in_xml_parse}
===================================

If you want to parse sub-node of the xml just override the function member:

```{.cpp}
	bool loadXML(const exml::Element& _node) override;
```

Many example are availlable in container widget.

Simple example:

```{.cpp}
	if (_node.exist() == false) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// parse all the elements:
	for (const auto it : _node.nodes) {
		exml::Element pNode = it.toElement();
		if (pNode.exist() == false) {
			// trash here all that is not element
			continue;
		}
		// Get the sub-node name:
		std::string widgetName = pNode.getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			APPL_ERROR("[" << getId() << "] (l "<<pNode->getPos()<<") Unknown basic node='" << widgetName << "' not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		...
	}
	return true;
```




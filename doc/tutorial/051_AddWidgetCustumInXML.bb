
== Declare object in XML ==

Object can be declared in some XML, (like gui decription), then we need to declare the Object in the system recognition.

=== Declare Object ===

In your application "void onCreate(ewol::Context& _context) override" add the function:

[code style=c++]
	YourWidgetClass::createManagerWidget(_context.getWidgetManager());
[/code]

The simple question is: I does not define this function, where it is done ?

The createManagerWidget is instancuate when you use the macro:

[code style=c++]
	DECLARE_WIDGET_FACTORY(YourWidgetClass, "YourWidgetClass");
[/code]

it create 2 function: "create(...)" and "createManagerWidget()"

=== Declare on XML and configuration ===

in the xml instance simply request it like:

[code style=xml]
	<YourWidgetClass name="jkjkj">
		...
	</YourWidgetClass>
[/code]

The xml attribute are automaticaly parsed to configure properties of you object (this is the reason of naming it).

=== Special case SubParsing XML element ===

If you want to parse sub-node of the xml just override the function member:

[code style=c++]
	bool loadXML(const std::shared_ptr<const exml::Element>& _node) override;
[/code]

Many example are availlable in container widget.

Simple example:

[code style=c++]
	if (_node == nullptr) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// parse all the elements:
	for (size_t iii=0; iii < _node->size(); iii++) {
		std::shared_ptr<const exml::Element> pNode = _node->getElement(iii);
		if (pNode == nullptr) {
			// trash here all that is not element
			continue;
		}
		// Get the sub-node name:
		std::string widgetName = pNode->getValue();
		if (getWidgetManager().exist(widgetName) == false) {
			APPL_ERROR("[" << getId() << "] (l "<<pNode->getPos()<<") Unknown basic node='" << widgetName << "' not in : [" << getWidgetManager().list() << "]" );
			continue;
		}
		...
	}
	return true;
[/code]




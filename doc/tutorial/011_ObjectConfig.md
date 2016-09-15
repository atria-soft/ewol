
Objectifs:
==========

:** Understand base of [lib[eproperty]] configuration parameter
:** Create an configurable object

Configuration using
===================

All [class[ewol::Object]] have a configuration of parameters (the object name is a parameter), Then we need to set get and use xml to update parameters.

Set a Parameter/Property
------------------------

[note]
	Parameter is managed by the [lib[eproperty|e-property library]]
[/note]

With a string configuration
***************************

[code style=c++]
	if (tmpObject->parameterSet("name", "new name of object") == false) {
		APPL_ERROR("Can not set object parameter");
	}
[/code]

whith the object name
*********************

[code style=c++]
	if (parameterSetOnWidgetNamed("objectName", "value", "16.2") == false) {
		APPL_ERROR("Can not set object parameter");
	}
[/code]

Get Parameter
-------------

[code style=c++]
	std::string val = tmpObject->parameterGet("name");
	APPL_INFO("Get Object property : name='" << val << "'");
[/code]

Implement configuration
=======================

[code style=c++]
#include <ewol/object/Object.h>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			eproperty::Value<bool> propertyValue; //!< Property interface (all time start with "property")
		protected:
			//! @brief Constructor
			MyObj(void) :
			  propertyValue(this, "value",
			                      false,
			                      "Value of the parameter (descrition string)",
			                      &appl::MyObj::onChangeParameterValue) {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
				// do some stuff with the init value
			}
		public:
			//! @brief Destructor
			virtual ~MyObj(void) { }
			DECLARE_FACTORY(MyObj);
		public:
			void onChangeParameterValue() {
				APPL_DEBUG("The internal value has change, new value is : '" << *m_value << "'");
			}
	}
}
[/code]

In the contructor we need to add:
[code style=c++]
m_value(*this, "value", false, "Value of the parameter (descrition string)")
[/code]
:** [b]'this':[/b] Pointer the main class to call it chen value change.
:** [b]"value":[/b] Is the name of the parameter.
:** [b]false:[/b] The default value.
:** [b]"....."[/b] Description of the parameter (optionnal).
:** [b]&appl::MyObj::onChangeParameterValue[/b] The callback when the value change (optionnal).


The last point is that the *m_value is an inline fuction then it take no more CPU cycle to access the value than normal variable.

Some other parameter are availlable :
:** eproperty::Value<T> Basic parameter.
:** eproperty::Range<T> For numeric parameter that range value are check befor setting new value.
:** eproperty::List<T> For List of parameter values.

For more information see [lib[eproperty]]


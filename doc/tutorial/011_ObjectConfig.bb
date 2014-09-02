=?= Tutorial 3: Object Config =?=
__________________________________________________
[left][tutorial[010_ObjectModel | Previous: Object model]][/left] [right][tutorial[012_ObjectMessage | Next: Object signals]][/right]

=== Objectif ===
:** Understand ewol::Object configuration parameter
:** Create an configurable object

== Configuration using ==

All [class[ewol::Object]] have a configuration of parameters (the object name is a parameter), Then we need to set get and use xml to update parameters.

=== Set a Parameter ===

[note]
	Using getter and setter is really better, and faster.
[/note]

==== With a string configuration ====

[code style=c++]
	if (tmpObject->parameterSet("name", "new name of object") == false) {
		APPL_ERROR("Can not set object parameter");
	}
[/code]

==== whith the object name ====

[code style=c++]
	if (parameterSetOnWidgetNamed("objectName", "value", "16.2") == false) {
		APPL_ERROR("Can not set object parameter");
	}
[/code]

=== Get Parameter ===

[code style=c++]
	std::string val = tmpObject->parameterGet("name");
	APPL_INFO("Get Object property : name='" << val << "'");
[/code]

== Implement configuration ==

[code style=c++]
#include <ewol/object/Object.h>
namespace appl {
	class MyObj : public ewol::Object {
		protected:
			//! @brief Constructor
			MyObj(void) :
			  m_value(*this, "value", false, "Value of the parameter (descrition string)") {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
			}
		public:
			//! @brief Destructor
			virtual ~MyObj(void) { }
			DECLARE_FACTORY(MyObj);
		private:
			ewol::object::Param<bool> m_value; //!< Internal Object value
		public:
			//! @brief Setter
			void setValue(bool _val) {
				m_value.set(_val);
			}
			//! @brief Getter
			bool getValue() const {
				return m_value.get();
			}
		public: // herited function:
			void onParameterChangeValue(const ewol::object::ParameterRef& _paramPointer) {
				if (_paramPointer == m_value) {
					APPL_DEBUG("The internal value has change, new value is : '" << m_value.get() << "'");
				}
			}
	}
}
[/code]

In the contructor we need to add:
[code style=c++]
m_value(*this, "value", false, "Value of the parameter (descrition string)")
[/code]
:** [b]'*this':[/b] Reference the main class to call it chen value change.
:** [b]"value":[/b] Is the name of the parameter.
:** [b]false:[/b] The default value.
:** [b]"....."[/b] Description of the parameter (optionnal).


The function [b]onParameterChangeValue[/b] is called only the parameter change (no historic has been registered)

The last point is that the m_value.get() is an inline fuction then it take no more CPU cycle to access the value than normal variable.

Some other parameter are availlable :
:** ewol::object::Param<T> Basic parameter.
:** ewol::object::ParamRange<T> For numeric parameter that range value are check befor setting new value.
:** ewol::object::ParamList<T> For List of parameter values.



EWOL: Object config                                {#ewol_tutorial_object_config}
===================

@tableofcontents

Objectifs:                                {#ewol_tutorial_object_config_objectives}
==========

  - Understand base of [e-property](http://atria-soft.github.io/eproperty) configuration parameter
  - Create an configurable object

Configuration using                                {#ewol_tutorial_object_configusing}
===================

All ewol::Object have a configuration of parameters (the object name is a parameter),
then we need to set get and use xml to update parameters.

Set a Parameter/Property                                {#ewol_tutorial_object_config_prop}
------------------------

**Note:**

```
	Parameter is managed by the [e-property](http://atria-soft.github.io/eproperty)
```

With a string configuration
***************************

```{.cpp}
	if (tmpObject->properties.set("name", "new name of object") == false) {
		APPL_ERROR("Can not set object parameter");
	}
```

whith the object name
*********************

```{.cpp}
	if (ewol::propertySetOnObjectNamed("objectName", "value", "16.2") == false) {
		APPL_ERROR("Can not set object parameter");
	}
```

Get Parameter                                {#ewol_tutorial_object_config_param}
-------------

```{.cpp}
	etk::String val = tmpObject->properties.get("name");
	APPL_INFO("Get Object property: name='" << val << "'");
```

Implement configuration                                {#ewol_tutorial_object_config_impl}
=======================

```{.cpp}
#include <ewol/object/Object.hpp>
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
```

In the contructor we need to add:
```{.cpp}
propertyValue(this,
              "value",
              false,
              "Value of the parameter (descrition string)",
              &appl::MyObj::onChangeParameterValue)
```
  - **'this':** Pointer the main class to call it chen value change.
  - **"value":** Is the name of the parameter.
  - **false:** The default value.
  - **"....."** Description of the parameter (optionnal).
  - **&appl::MyObj::onChangeParameterValue** The callback when the value change (optionnal).


The last point is that the ```*propertyValue``` same as ```propertyValue.get()``` are inline fuction then it take no more CPU cycle to access the value than normal variable.

Some other parameter are availlable :
  - eproperty::Value<T> Basic parameter.
  - eproperty::Range<T> For numeric parameter that range value are check befor setting new value.
  - eproperty::List<T> For List of parameter values.

For more information see [e-property](http://atria-soft.github.io/eproperty)


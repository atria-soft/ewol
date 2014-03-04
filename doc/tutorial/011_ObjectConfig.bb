=?= Tutorial 3: Object Config =?=
__________________________________________________
[left][tutorial[010_ObjectModel | Previous: Object model]][/left] [right][tutorial[012_ObjectMessage | Next: Object message]][/right]

=== Objectif ===
:** Understand ewol::Object configuration parameter
:** Create an configurable object

== Use of Configuration ==

Configuration are parameters to set some property on an object,
this is a really generic use in many system,
this is the reason why we implement one.


This is a generic interface to set and get parameter,
when you did not really know it,
but it generate many convertion to string search and many other things...

If you know the object just interact with it with his own accessors (faster ond conpilation check).


=== Set config ===

When you have the pointer on the object:
[code style=c++]
	if (tmpObject->setConfig("name", "new name of object") == false) {
		APPL_ERROR("Can not set object property");
	}
[/code]

A second methode is to request the name with his direct config name:
[code style=c++]
	// to be sure that the name exist:
	if (tmpObject->setConfig(ewol::Object::configName, "new name of object") == false) {
		APPL_ERROR("Can not set object property");
	}
[/code]
The only aventage here is to have an automatic update on the name of the parameter.

It is possible to configure an object whitout knowing his name:
[code style=c++]
	// in an ewol::Object only ...
	if (setConfigNamed("object name", "ewol::Object::configName, "new name of object") == false) {
		APPL_ERROR("Can not set object property");
	}
[/code]


=== Get config ===

Direct get the configuration:
[code style=c++]
	std::string val = tmpObject->getConfig("name");
	APPL_INFO("Get Object property : name='" << val << "'");
[/code]

Get with his direct definition name:
[code style=c++]
	std::string val = tmpObject->getConfig(ewol::Object::configName);
	APPL_INFO("Get Object property : " << ewol::Object::configName << "'" << val << "'");
[/code]


== Implement configuration ==

=== Declare config ===

In the header file:
[code style=c++]
#include <ewol/object/Object.h>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			// Config list of properties
			static const char* const configValue;
		public:
			//! @brief Constructor
			MyObj(void);
			//! @brief Destructor
			virtual ~MyObj(void);
		private:
			bool m_value; //!< Internal Object value
		public:
			//! @brief Setter
			void setValue(bool _val) {
				m_value = _val;
			}
			//! @brief Getter
			bool getValue(void) const {
				return m_value;
			}
		public: // herited function:
			bool onSetConfig(const ewol::object::Config& _conf);
	}
}
[/code]

[note]
By convention declare config started with "configXXXXXX"
[/note]

In the source file:
[code style=c++]
// Declare the configuration Name:
const char* const appl::MyObj::configValue = "value";

appl::MyObj::MyObj(void) {
	// declare the configuration on this object:
	registerConfig(configValue, "bool", NULL, "object configuration description");
	// Note : This API is not compleately define ...
}
appl::MyObj::~MyObj(void) {
	// nothing to do ...
}
[/code]
Now an extern Object can register configure these parameter, otherwise, they will be rejected!!!


=== Get and Set config ===

You can see in these implementation that we not compare the string but just the pointer.
The ewol::Object convert the string in the correct pointer to be faster in many case.

==== Set configuration ====

[code style=c++]
bool appl::MyObj::onSetConfig(const ewol::object::Config& _conf) {
	APPL_VERBOSE("[" << getId() << "] {" << getObjectType() << "} set config : " << _conf);
	if (_conf.getConfig() == configValue) {
		setValue(std::stob(_conf.getData()));
		return true;
	}
	return false;
}
[/code]

==== Get configuration ====

[code style=c++]
bool appl::MyObj::onGetConfig(const char* _config, std::string& _result) const {
	if (_config == configValue) {
		_result = std::to_string(getValue());
		return true;
	}
	return false;
}
[/code]

== Conclusion ==

Now you can choice the methode you want in your application to implement your basic configuration feature.


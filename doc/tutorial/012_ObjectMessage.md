EWOL: Object message                                {#ewol_tutorial_object_message}
====================

@tableofcontents

Objectifs:                                {#ewol_tutorial_object_message_objectifs}
==========

  - Understand base of [e-signal](http://atria-soft.github.io/esignal) Messaging system
  - Create extern message and receive Object message

Message system                                {#ewol_tutorial_object_message_system}
==============

esignal base his signal on landa functions

It permit to an object to generate some **'signals'**

All signal are synchronous *(asynchronous is not implemented yet)*


Receive signals from other object                                {#ewol_tutorial_object_message_from_object}
=================================

Register on the 'up' and 'value' signal of a button:

Button header :
```{.cpp}
	...
	public:
		esignal::Signal<> signalDown;
		esignal::Signal<> signalUp;
		...
		esignal::Signal<bool> signalValue;
	...
```

simple signal connection:
-------------------------

```{.cpp}
#include <ewol/object/Object.hpp>
#include <ewol/widget/Button.hpp>
namespace appl {
	class MyObj : public ewol::Object {
		private:
			ewol::widget::ButtonShared m_button;
			esignal::Connection m_connect;
		protected:
			//! @brief Constructor
			MyObj() {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
				m_button = ewol::widget::Button::create();
				if (m_button == nullptr) {
					APPL_ERROR("Can not create button...");
					return;
				}
				m_button->propertyToggle.set(true);
				// We connect signals here: (permanent connection)
				m_button->signalUp.connect(sharedFromThis(), &appl::MyObj::onCallbackUp);
				m_button->signalValue.connect(sharedFromThis(), &appl::MyObj::onCallbackValue);
			}
		public:
			//! @brief Destructor
			virtual ~MyObj() { }
			DECLARE_FACTORY(MyObj);
		private:
			void onCallbackUp() {
				APPL_INFO("button pressed: UP");
			}
			void onCallbackDown() {
				APPL_INFO("button pressed: DOWN");
			}
			void onCallbackValue(const bool& _value) {
				APPL_INFO("button value: " << _value);
				if (_value == true) {
					// We connect signals here: (removable connection)
					m_connect = m_button->signalDown.connect(this, &appl::MyObj::onCallbackDown);
				} else {
					// we disconnect the previous connection
					m_connect.disconnect();
				}
			}
	}
}
```

**Note:**

```
	The connection with SharedPtr are static. they keep in internal a WeakPtr to remove connection if the object is removed.
```

**Note:**

```
	The connection that return a esignal::Connection are volatil, if you don't keep the connection handle, the connection is automaticly removed.
```

Declare Signal:                                {#ewol_tutorial_object_message_declare}
===============

source
-------

```{.cpp}
#include <ewol/object/Object.hpp>
#include <ewol/widget/Button.hpp>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			ewol::object::Signal<> signalEmpty;
			ewol::object::Signal<bool> signalBoolean;
			ewol::object::Signal<etk::String> signalString;
		protected:
			//! @brief Constructor
			MyObj() :
			  signalEmpty(this, "empty"),
			  signalBoolean(this, "boolean"),
			  signalString(this, "string") {
				// nothing to do..
			}
		public:
			//! @brief Destructor
			virtual ~MyObj() { }
			DECLARE_FACTORY(MyObj);
		private:
			void process() {
				signalEmpty.emit();
				signalBoolean.emit(false);
				signalString.emit("plop... plop");
			}
	}
}
```

Conclusion:                                {#ewol_tutorial_object_message_conclusion}
===========

You will now able to reise signals between objects...

For more information see [e-signal](http://atria-soft.github.io/esignal)





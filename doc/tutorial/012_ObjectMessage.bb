=?= Tutorial 4: Object Signals =?=
__________________________________________________
[left][tutorial[011_ObjectConfig | Previous: Object config]][/left] [right][tutorial[020_FileAccess | Next: File Access]][/right]

=== Objectif ===
:** Understand ewol::Object Messaging system
:** Create extern message and receive Object message

== Message system ==

Message system is based on generic std::funtion and std::bind methode:

It permit to an object to generate some [b]'signals'[/b].

All signal are synchronous


== Receive signals from other object ==

[todo]
Link with the signal name
[/todo]

Register on the 'up' and 'value' signal of a button:

Button header :
[code style=c++]
	...
	public:
		ewol::object::Signal<void> signalDown;
		ewol::object::Signal<void> signalUp;
		...
		ewol::object::Signal<bool> signalValue;
	...
[/code]

=== simple signal connection: ===

[code style=c++]
#include <ewol/object/Object.h>
#include <ewol/widget/Button.h>
namespace appl {
	class MyObj : public ewol::Object {
		private:
			std::shared_ptr<ewol::widget::Button> m_button;
		protected:
			//! @brief Constructor
			MyObj(void) {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
				m_button = ewol::widget::Button::Create();
				if (m_button == nullptr) {
					APPL_ERROR("Can not create button...");
					return;
				}
				// We connect signals here :
				m_button->signalUp.bind(shared_from_this(), &appl::MyObj::onCallbackUp);
				m_button->signalValue.bind(shared_from_this(), &appl::MyObj::onCallbackValue);
			}
		public:
			//! @brief Destructor
			virtual ~MyObj(void) { }
			DECLARE_FACTORY(MyObj);
		private:
			void onCallbackUp() {
				APPL_INFO("button pressed: UP);
			}
			void onCallbackValue(const bool& _value) {
				APPL_INFO("button value: " << _value);
			}
	}
}
[/code]


=== Advenced signal connection: ===

Here we will see how to connect an advance function on a signal

[code style=c++]
#include <ewol/object/Object.h>
#include <ewol/widget/Button.h>
namespace appl {
	class MyObj : public ewol::Object {
		private:
			std::shared_ptr<ewol::widget::Button> m_button;
		protected:
			//! @brief Constructor
			MyObj(void) {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
				m_button = ewol::widget::Button::Create();
				if (m_button == nullptr) {
					APPL_ERROR("Can not create button...");
					return;
				}
				// We connect signals here :
				m_button->signalUp.register(shared_from_this(), std::bind(&appl::MyObj::onCallbackUp, this, std::string("tmpVariableToSend")));
				m_button->signalValue.register(shared_from_this(), std::bind(&appl::MyObj::onCallbackValue, this));
			}
		public:
			//! @brief Destructor
			virtual ~MyObj(void) { }
			DECLARE_FACTORY(MyObj);
		private:
			void onCallbackUp(const std::string& _value) {
				APPL_INFO("button pressed: UP inputMessage: " << _value);
			}
			void onCallbackValue() {
				APPL_INFO("button value: " << _value);
			}
	}
}
[/code]

=== Connect to a signal with a named widget ===

TODO: documentation :
:** subBind(_type, _name, _event, _obj, _func)
:** globalBind(_type, _name, _event, _obj, _func)
:** externSubBind(_object, _type, _name, _event, _obj, _func)


== Declare Signal ==

=== source ===

[code style=c++]
#include <ewol/object/Object.h>
#include <ewol/widget/Button.h>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			ewol::object::Signal<void> signalEmpty;
			ewol::object::Signal<bool> signalBoolean;
			ewol::object::Signal<std::string> signalString;
		protected:
			//! @brief Constructor
			MyObj(void) :
			  signalEmpty(*this, "empty"),
			  signalBoolean(*this, "boolean"),
			  signalString(*this, "string") {
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
			void process() {
				signalEmpty.emit();
				signalBoolean.emit(false);
				signalString.emit("plop... plop");
			}
	}
}
[/code]

== Conclusion ==

You will now able to reise signals between objects...







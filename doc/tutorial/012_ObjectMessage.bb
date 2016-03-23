
=== Objectif ===
:** Understand base of [lib[esignal]] Messaging system
:** Create extern message and receive Object message

== Message system ==

esignal base his signal on landa functions

It permit to an object to generate some [b]'signals'[/b].

All signal are synchronous [i](asynchronous is not implemented yet)[/i]


== Receive signals from other object ==

Register on the 'up' and 'value' signal of a button:

Button header :
[code style=c++]
	...
	public:
		esignal::ISignal<> signalDown;
		esignal::ISignal<> signalUp;
		...
		esignal::ISignal<bool> signalValue;
	...
[/code]

=== simple signal connection: ===

[code style=c++]
#include <ewol/object/Object.h>
#include <ewol/widget/Button.h>
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
				m_button = ewol::widget::Button::Create();
				if (m_button == nullptr) {
					APPL_ERROR("Can not create button...");
					return;
				}
				m_button.propertyToggle.set(true);
				// We connect signals here: (permanent connection)
				m_button->signalUp.connect(shared_from_this(), &appl::MyObj::onCallbackUp);
				m_button->signalValue.connect(shared_from_this(), &appl::MyObj::onCallbackValue);
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
[/code]

[note]
	The connection with SharedPtr are static. they keep in internal a WeakPtr to remove connection if the object is removed.
[/note]

[note]
	The connection that return a [class[esignal::Connection]] are volatil, if you don't keep the connection handle, the connection is automaticly removed.
[/note]

== Declare Signal ==

=== source ===

[code style=c++]
#include <ewol/object/Object.h>
#include <ewol/widget/Button.h>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			ewol::object::Signal<> signalEmpty;
			ewol::object::Signal<bool> signalBoolean;
			ewol::object::Signal<std::string> signalString;
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
[/code]

== Conclusion ==

You will now able to reise signals between objects...

For more information see [lib[esignal]]





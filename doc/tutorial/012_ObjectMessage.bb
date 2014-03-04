=?= Tutorial 4: Object Message =?=
__________________________________________________
[left][tutorial[011_ObjectConfig | Previous: Object config]][/left] [right][tutorial[020_FileAccess | Next: File Access]][/right]

=== Objectif ===
:** Understand ewol::Object Messaging system
:** Create extern message and receive Object message

== Message system ==

The message system is a simple message sending between Object.
This is composed with a pointer (const char* const) that represent the mesage ID (pointer, then unique)
and a value (std::string)

The message are broadcast or multicast on object watcher.

== Receive Message from other object ==

=== Register on message ===

We will se an example on the widget : [class[ewol::widget::Button]]

By default the messageID is the event generated, But to overwrite this message Id just create a new one:
[code style=c++]
// on the global on the file or in private class member:
static const char* const g_backMessage = "local-event-button-pressed";
static const char* const g_backMessageValue = "local-event-button-value";
static const char* const g_backMessageDataOverWritte = "local-event-button-data";
[/code]


Register with his name:
[code style=c++]
registerOnEvent(this, "pressed", g_backMessage);
[/code]

Register with his direct name:
[code style=c++]
registerOnEvent(this, ewol::widget::Button::eventValue, g_backMessageValue);
[/code]

It is possible to overwrote the data send by the Object :
[code style=c++]
registerOnEvent(this, ewol::widget::Button::eventPressed, g_backMessageDataOverWritte, "Data we want to receive");
[/code]


=== Receive message ===

To receive message from other widget, just implement this function:

[code style=c++]

void appl::ObjectName::onReceiveMessage(const ewol::object::Message& _msg) {
	APPL_INFO("Receive Event : " << _msg);
	if (_msg.getMessage() == g_backMessage) {
		// process here
		return;
	}
	if (_msg.getMessage() == g_backMessageValue) {
		APPL_INFO("message value: '" << _msg.getData() << "'");
		return;
	}
	if (_msg.getMessage() == g_backMessageDataOverWritte) {
		APPL_INFO("Overwrite message data: '" << _msg.getData() << "'");
		return;
	}
}
[/code]


== Declare Extern Message ==

=== Declare Message ===

In the header file:
[code style=c++]
#include <ewol/object/Object.h>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			// Event list of properties
			static const char* const eventValue;
		public:
			//! @brief Constructor
			MyObj(void);
			//! @brief Destructor
			virtual ~MyObj(void);
	}
}
[/code]

[note]
By convention declare events started with "eventXXXXXX"
[/note]

In the source file:
[code style=c++]
// Declare the configuration Name:
const char* const appl::MyObj::eventValue = "value";

appl::MyObj::MyObj(void) {
	// declare Event generated on this object:
	addEventId(eventValue);
}
appl::MyObj::~MyObj(void) {
	// nothing to do ...
}
[/code]
Now an extern Object can register event on this object, otherwise, they will be rejected!!!


=== Generate Message ===

Now we have register object message, We need to have generated it, This is really simple :

[code style=c++]
	// with no data:
	generateEventId(eventValue);
	// With a custom data:
	generateEventId(eventValue, "My sring custom data ...");
[/code]


== Conclusion ==

You will now able to generate event between objects...







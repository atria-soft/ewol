
=== Objectif ===
:** Understand ewol basic [class[ewol::Object]]
:** Use [class[ewol::Object]] correctly

== Basis of the ewol::Object ==

An object in Ewol is a simple class: [class[ewol::Object]] This object is the basis of all element in the ewol system.
This is designed to manage many common things:

:** Unique ID
:** Name
:** Parameters
:** Signal generation
:** Xml configuration
:** Removing

[note]
Please do not compare with the gObject basic class...
[/note]


== Create an Object: ==

Creating an object is really simple:

[code style=c++]
	ewol::widget::ButtonShared tmpButon = ewol::widget::Button::create();
	APPL_INFO("We just create a button widget with unique ID=" << tmpButon->getId() << " name='" << tmpButon->propertyName.get() << "'");
[/code]

Note that all object created are [class[ememory::SharedPtr]] base for the current version on [class[std::shared_ptr]].
We wrapped it because the current inplementation of [class[std::shared_ptr]] is not thread safe, and we want use a thread-safe vertion of it.

[note]
	The widget is not stored in a ememory::SharedPtr<ewol::widget::Button> but in a ewol::widget::ButtonShared to simplify the using of the pointer.
	You have also: ememory::WeakPtr<ewol::widget::Button> = ewol::widget::ButtonWeak
[/note]

Set the name of the object:

[code style=c++]
	tmpButon->propertyName.set("my widget name");
	APPL_INFO("We just create an Object with ID=" << tmpButon->getId() << " name='" << tmpButon->propertyName.get() << "'");
[/code]


== Remove an Object: ==

Simply use the function:
[code style=c++]
	tmpButon->destroy();
[/code]

This function request his parrent to remove the [class[ememory::SharedPtr]] it keep on it.
And when all std::shared_ptr is removed the object will be really removed.

At his point we can think an object is allive all the time someone keep a reference on it, then when you are not a parrent of the object, do not keep a [class[ememory::SharedPtr]] but a [class[ememory::WeakPtr]].

[note]
If some Object is not removed when you close the application, the system inform you with displaying all object alive.
[/note]


== Retrieve an Object: ==

In Ewol this is possible to get a object with his name.

=== Find a global Object (ouside an Object) ===

[code style=c++]
	#include <ewol/context/Context.h>
	
	ewol::ObjectShared tmpObject = ewol::getContext().getEObjectManager().getObjectNamed("obj Name");
	if (tmpObject == nullptr) {
		APPL_ERROR("The Object does not exist");
	}
[/code]

=== Find a global Object (inside an Object) ===

[code style=c++]
	ewol::ObjectShared tmpObject = getObjectNamed("obj Name");
	if (tmpObject == nullptr) {
		APPL_ERROR("The Object does not exist");
	}
[/code]

=== Find a sub-object ===

[code style=c++]
	ewol::ObjectShared tmpObject = getSubObjectNamed("obj Name");
	if (tmpObject == NULL) {
		APPL_ERROR("The Object does not exist");
	}
[/code]

=== retriving your object type ===

It could be really interesting to retrive your own instance:

[code style=c++]
	ewol::ObjectShared tmpObject ...;
	
	appl::MyOwnObjectShared myObject = std::dynamic_pointer_cast<appl::MyOwnObject>(tmpObject);
[/code]


=?= Tutorial 2: Object Model =?=
__________________________________________________
[left][tutorial[001_HelloWord | Previous: Hello Word]][/left] [right][tutorial[011_ObjectConfig | Next: Object config]][/right]

=== Objectif ===
:** Understand ewol basic [class[ewol::Object]]
:** Use [class[ewol::Object]] correctly

== Basis of the ewol::Object ==

An object in Ewol is a simple class : [class[ewol::Object]] This object is the basis of all element in the ewol system.
This is designed to manage many common things:

:** Unique ID
:** Name
:** Parameters
:** Signal generation
:** Xml configuration
:** Removing
:** Perodic calling

[note]
Please do not compare with the gObject basic class...
[/note]


== Create an Object: ==

Creating an object is really simple:

[code style=c++]
	std::shared_ptr<ewol::Button> tmpButon = ewol::Button::create();
	APPL_INFO("We just create a button widget with unique ID=" << tmpButon->getId() << " name='" << tmpButon->getName() << "'");
[/code]

Note that all object created are std::shared_ptr.


Set the name of the object:

[code style=c++]
	tmpButon->setName("my widget name");
	APPL_INFO("We just create an Object with ID=" << tmpButon->getId() << " name='" << tmpButon->getName() << "'");
[/code]


== Remove an Object: ==

Simply use the function:
[code style=c++]
	tmpButon->destroy();
[/code]

This function request his parrent to remove the std::shared_ptr it keep on it.
And when all std::shared_ptr is removed the object will be really removed.

At his point we can think an object is allive all the time someone keep a reference on it, then when you are not a parrent of the object, do not keep a std::shared_ptr but a std::weak_ptr.

[note]
If some Object is not removed when you close the application, the system inform you with displaying all object already alive.
[/note]


== Retrieve an Object: ==

In Ewol this is possible to get a object with his name.

=== Find a global Object (ouside an Object) ===

[code style=c++]
	#include <ewol/context/Context.h>
	
	std::shared_ptr<ewol::Object> tmpObject = ewol::getContext().getEObjectManager().getObjectNamed("obj Name");
	if (tmpObject == NULL) {
		APPL_ERROR("The Object does not exist");
	}
[/code]

=== Find a global Object (inside an Object) ===

[code style=c++]
	std::shared_ptr<ewol::Object> tmpObject = getObjectNamed("obj Name");
	if (tmpObject == NULL) {
		APPL_ERROR("The Object does not exist");
	}
[/code]

=== Find a sub-object ===

[code style=c++]
	std::shared_ptr<ewol::Object> tmpObject = getSubObjectNamed("obj Name");
	if (tmpObject == NULL) {
		APPL_ERROR("The Object does not exist");
	}
[/code]

=== retriving your object type ===

It could be really interesting to retrive your own instance:

[code style=c++]
	std::shared_ptr<ewol::Object> tmpObject ...;
	
	std::shared_ptr<appl::MyOwnObject> myObject = std::dynamic_pointer_cast<appl::MyOwnObject>(tmpObject);
[/code]

== conclusion ==

TODO ...



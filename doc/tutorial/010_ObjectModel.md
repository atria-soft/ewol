EWOL: Object model                                {#ewol_tutorial_object_model}
==================

@tableofcontents

Objectifs:                                {#ewol_tutorial_object_model_objectives}
==========

  - Understand ewol basic ewol::Object
  - Use ewol::Object correctly

Basis of the ewol::Object                                {#ewol_tutorial_object_model_object}
=========================

An object in Ewol is a simple class: ewol::Object This object is the basis of all element in the ewol system.
This is designed to manage many common things:

  - Unique ID
  - Name
  - Parameters
  - Signal generation
  - Xml configuration
  - Removing

**Note:**

```
Please do not compare with the gObject basic class...
```


Create an Object:                                {#ewol_tutorial_object_model_create}
=================

Creating an object is really simple:

```{.cpp}
	ewol::widget::ButtonShared tmpButon = ewol::widget::Button::create();
	APPL_INFO("We just create a button widget with unique ID=" << tmpButon->getId() << " name='" << tmpButon->propertyName.get() << "'");
```

Note that all object created are ememory::SharedPtr base for the current version on std::shared_ptr.
We wrapped it because the current inplementation of std::shared_ptr is not thread safe, and we want use a thread-safe version of it.

**Note:**

```
	The widget is not stored in a ememory::SharedPtr<ewol::widget::Button> but in a ewol::widget::ButtonShared to simplify the using of the pointer.
	You have also: ememory::WeakPtr<ewol::widget::Button> = ewol::widget::ButtonWeak
```

Set the name of the object:

```{.cpp}
	tmpButon->propertyName.set("my widget name");
	APPL_INFO("We just create an Object with ID=" << tmpButon->getId() << " name='" << tmpButon->propertyName.get() << "'");
```


Remove an Object:                                {#ewol_tutorial_object_model_remove}
=================

Simply use the function:
```{.cpp}
	tmpButon->destroy();
```

This function request his parrent to remove the ememory::SharedPtr it keep on it.
And when all ememory::SharedPtr is removed the object will be really removed.

At his point we can think an object is alive all the time someone keep a reference on it,
then when you are not a parrent of the object, do not keep a ememory::SharedPtr but a ememory::WeakPtr.

**Note:**

```
If some Object is not removed when you close the application, the system inform you with displaying all object alive.
```


Retrieve an Object:                                {#ewol_tutorial_object_model_find}
===================

In Ewol this is possible to get a object with his name.

Find a global Object (ouside an Object)
---------------------------------------

```{.cpp}
	#include <ewol/context/Context.hpp>
	
	ewol::ObjectShared tmpObject = ewol::getContext().getEObjectManager().getObjectNamed("obj Name");
	if (tmpObject == nullptr) {
		APPL_ERROR("The Object does not exist");
	}
```

Find a global Object (inside an Object)
---------------------------------------

```{.cpp}
	ewol::ObjectShared tmpObject = getObjectNamed("obj Name");
	if (tmpObject == nullptr) {
		APPL_ERROR("The Object does not exist");
	}
```

Find a sub-object
-----------------

```{.cpp}
	ewol::ObjectShared tmpObject = getSubObjectNamed("obj Name");
	if (tmpObject == NULL) {
		APPL_ERROR("The Object does not exist");
	}
```

retriving your object type
--------------------------

It could be really interesting to retrive your own instance:

```{.cpp}
	ewol::ObjectShared tmpObject ...;
	
	appl::MyOwnObjectShared myObject = std::dynamic_pointer_cast<appl::MyOwnObject>(tmpObject);
```


EWOL: Hello world                                {#ewol_tutorial_hello_world}
=================

@tableofcontents

Objectif:                                        {#ewol_tutorial_hello_world_objectives}
=========
  - Understand basis of ewol
  - Create a simple windows with a label "Hello Word"

debug tools:                                     {#ewol_tutorial_hello_world_debug}
============

I will use for all test a basic template [elog](http://atria-soft.github.io/elog) for debug logger that redirect logs in Android and IOs

File ```appl/debug.hpp```:

@include HelloWord/appl/debug.hpp

File ```appl/debug.cpp```:

@include HelloWord/appl/debug.cpp


Application Sources:                             {#ewol_tutorial_hello_world_sources}
====================

Application Main:                                {#ewol_tutorial_hello_world_sources_main}
-----------------

A generic Ewol application is manage by creating an ewol::context::Application that is the basis of your application.

Due to the fact the ewol library is a multi-platform framework (base on [GALE](http://atria-soft.github.io/gale)), you will have many contraint like:
  - One application at the same time (note an exception for android wallpaper)
  - One Windows displayable at the time (main point of view of apple developpers)
  - Not a big CPU ...

Then we will create the application:

First things: Some includes:

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_include

Declare the application:

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_application


The input ewol::Context is the main system context (for ewol).

**Note:**

```
It is important to know that the system can create your application multiple times, the basic example of this is the Wallpaper on Android.

What is done:
  - When we select the wallpaper it create a new application (to show an example)
  - When applying your choice, it create the real one an remove the previous one.
```

In all program we need to have a main()

To be portable on Android, the "main" in the java might call your main through the Android wrapper.

To simplify compabilities between platform it is recommanded to not add other things in the application main:

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_main


Some configuration are needed                             {#ewol_tutorial_hello_world_sources_config}
-----------------------------

In your application you can use many configuration, it is really better to set all your configuration dynamic.
With this basic condition will simplify the interface of the library if you would have many different application
(never forget the compilator garbage collector is really very efficient).

All of this will be done one time: 
Then we will do it in:

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_create

### Parse arguments: ####

All the argument is store in the ewol main application context: just get it...

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_parse_arguments

### Set basic windosw size (for desktop): ####

On descktop you can specify a start windows size:

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_set_windows_size


### Select fonts: ####

This can be a problem when you design an application for some other operating system (OS),
They do not have the same default fonts, than you can embended some of them or try to use the system fonts.

We select an order to search the font names and the system basic size.

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_set_font_property


Main Windows:                             {#ewol_tutorial_hello_world_sources_windows}
-------------

Create the main Windows:

For this point we will create a class that herited form the basic ewol::widget::Windows class:

@include HelloWord/appl/Windows.hpp

The C macro "DECLARE_FACTORY" create a simple factory function "create" that return the ewol::Object well create.

For some internal reason, we create the object and we call the "init" function after creating the object. When well done we return the shared object created.

See @ref ewol_tutorial_object_model to understand why this structure is so complex.

@include HelloWord/appl/Windows.cpp

The init function is virtual and you must call your parent object (or at least the ewol::Object::init)

@snippet HelloWord/appl/Windows.cpp ewol_sample_HW_windows_init



The title is associated on the current windows then it is a simple property of ewol::widget::Windows.

We can change with calling the "setDirectCheck" function instead of "set" function when you are in the constructor (the callback can be unstable when we construct the object)

@snippet HelloWord/appl/Windows.cpp ewol_sample_HW_windows_title


The object ewol::widget::Windows is a simple container. 
But the reference between Object is ememory::SharedPtr, and this is not accessible in the constructor.
This is the reason we use init function.

After we simple create a ewol::widget::Label in the main windows init.
We set label and basic properties:

@snippet HelloWord/appl/Windows.cpp ewol_sample_HW_windows_label

When we call the function ```ewol::Windows::setSubWidget```, it use the SharedFromThis() function that create an exception if we are in constructor (when setting the sub-widget parrent)


We can see in this example that the label have some other property like the font color.

The label can have decorated text based on the html generic writing but it is composed with really simple set of balise.
I will take a really long time to create a real html parser.

The availlable property is:
  - ```<br/>``` : New line
  - ```<font color="#FF0000\"> ... </font>``` :  change the font color.
  - ```<center> ... </center>``` : center the text.
  - ```<left> ... </left>``` : Set the text on the left.
  - ```<right> ... </right>``` : Set the text on the right.
  - ```<justify> ... </justify>``` : Set the text mode in justify.

**Note:**

```
The xml parser is a little strict on the case and end node (!! </br> !!),
but it support to:
  - Not have a main node.
  - replace '"' with ''' to simplify xml writing in C code.
```


Configure Ewol to have display the windows                             {#ewol_tutorial_hello_world_sources_configure_ewol}
------------------------------------------

At this point we have created the basic windows. 
But the system does not know it. 
Then we create windows and set it in the main context main ```appl::MainApplication::onCreate```:

@snippet HelloWord/appl/Main.cpp ewol_sample_HW_main_set_windows

Here we call the create function that is created by the DECLARE_FACTORY macro


**Note:**

```
You can use many windows and select the one you want to display, but I do not think it is the best design.
```

Build declaration:                             {#ewol_tutorial_hello_world_build}
==================

Ewol commonly use the [lutin](http://HeeroYui.github.io/lutin) build system.

Then we need to add a "lutin_YourApplicationName.py", then for this example: ```lutin_ewol-sample-HelloWord.py```

@include lutin_ewol-sample-HelloWord.py

Show [lutin](http://HeeroYui.github.io/lutin/lutin_module.html) doc for more information...

Build your application                         {#ewol_tutorial_hello_world_buildappl}
======================

Go to your workspace folder and launch:

```{.sh}
	lutin -C -mdebug ewol-sample-HelloWord
	# or
	lutin -C -mdebug ewol-sample-HelloWord?build
```

You can now execute your application:

```{.sh}
	lutin -C -mdebug ewol-sample-HelloWord?run
```

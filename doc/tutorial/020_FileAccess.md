EWOL: File access                                {#ewol_tutorial_file_access}
=================

@tableofcontents

Objectifs:                                {#ewol_tutorial_file_access_objectifs}
==========

  - Understand why we wrap interface on file system
  - read and write files


Limitation                                {#ewol_tutorial_file_access_limit}
==========

Application generation is really simple, but package management can create some problems...

For example :
  - Android does not permit access on the file system, but we need data that is named assets, these data in contained in a zip file.
  - Linux set his own application data in a special path : /usr/shared/applName/
  - MacOs create a bundle (*.app) that is a folder with all application data.
  - ...

For all these reasons we need to wrap standard application interface. (you can acces directly but it could be tricky and depend on the target)

Generic Properties                                {#ewol_tutorial_file_access_property}
==================

By default we dertermine some basics for files.

Then we need to determine file in the tree with:
  - **"DATA:///XXX"** Application internal data
    * Linux: /usr/share/applName/
    * Android: /xxx/yyy/applName.apk/asssets/
    * IOs: applName.app/share
    * MacOs: applName.app/Resources
  - **"USERDATA:XXX"** User save data (like game save)
    * Linux: ~/.local/share/applName/
    * Android: /xxx/yyy/data/applName/
  - **"HOME:XXX"** User home folder
    * Linux: ~/
    * Android: /sdcard/
    * IOs: --- => no Home
    * MacOs: ~/
  - **"/XXX"** Direct acces on a file in the fileSystem
  - ...

When you will call a file, you need to just call it with the starting name.

For example if I want to access at an application data I will call the file : "DATA:///myImage.png"

Integrate a file in a package                                {#ewol_tutorial_file_access_package}
=============================

In your ```lutin_xxx.py``` file add:

```{.py}
	# to copy a single file:
	myModule.copy_file("relative/path/file.svg","destination/folder/fileNewName.svg")
	# to copy an entire patern path
	myModule.copy_path("relative/path/start*.png","destination/folder/")
```

And now you can acces on these file with : "DATA:///destination/folder/fileNewName.svg"


Read a file                                {#ewol_tutorial_file_access_read}
===========


```{.cpp}
	#include <etk/os/FSNode.hpp>
	
	...
	
	etk::FSNode file("DATA:///destination/folder/file.svg");
	if (file.exist() == false) {
		APPL_ERROR("Can not read the file (Does not exist)");
		return;
	}
	APPL_INFO("open :" << file << " with size=" << file.fileSize());
	if (file.fileOpenRead() == false) {
		APPL_ERROR("Can not open in read mode the file: " << file);
		return;
	}
	// get a char
	APPL_INFO("read in: " << file << " the first char='" << file.fileGet() << "'");
	// Get a line
	etk::String output;
	file.fileGets(output);
	APPL_INFO("and the end of the line ='" << output << "'");
	// close the file (note : if you did not do it, it will be close automaticly with an error)
	file.fileClose();
```

Write a file                                {#ewol_tutorial_file_access_write}
============

```{.cpp}
	#include <etk/os/FSNode.hpp>
	
	...
	
	etk::FSNode file("USERDATA:exmple.txt");
	APPL_INFO("open :" << file);
	if (file.fileOpenWrite() == false) {
		APPL_ERROR("Can not open in write mode the file: " << file);
		return;
	}
	// put a char
	file.filePut('A');
	// write a line
	file.filePuts(" other string to put in the file ... \n");
	// close the file (note : if you did not do it, it will be close automaticly with an error)
	file.fileClose();
```

'Theme' management:                                {#ewol_tutorial_file_access_theme}
===================

The theme management is a subset of a file selected by a main key.
For example the basic theme of an API can be manage with only 2 commands (set the theme, and request upate of GUI)

At the start of the program, you might specify the default path theme:

```{.cpp}
	etk::theme::setNameDefault("GUI_COLOR", "theme/black");
	etk::theme::setNameDefault("GUI_SHAPE", "theme/rounded");
```

And when you want to change the theme, just call:

```{.cpp}
	// change the theme :
	etk::theme::setName("GUI_COLOR", "theme/white");
	// force reload of all the resources :
	ewol::getContext().getResourcesManager().reLoadResources();
	ewol::getContext().forceRedrawAll();
```

**Note:**

```
	This is not done automaticly, because reloading the resources can have a real cost of time.
```

You can acces on your theme with accessing the filename: "THEME_GUI_COLOR:///your/sub/path/file.xx"

An important think is that the theme file is searching in many path in the order:
  - USERDATA:GUI_COLOR:your/sub/path/file.xx
  - DATA:GUI_COLOR:your/sub/path/file.xx
  - USERDATA:GUI_COLOR(default):your/sub/path/file.xx
  - DATA:GUI_COLOR(default):your/sub/path/file.xx


Like this a user can overload the application theme...





=== Objectif ===
:** Understand why we wrap interface on file system

== Limitation ==

Application generation is really simple, but package management can create some problems...

For example :
:** Android does not permit access on the file system, but we need data that is named assets, thes data in contained in a zip file.
:** Linux set his own application data in a special path : /usr/shared/applName/
:** MacOs create a bundle (*.app) that is a folder with all application data.
:** ...

For all tese reason we need to wrap standard application interface. (you can acces directly but it could be tricky and depend on the target)

== Generic Properties ==

By default we dertermine some basics for files.

Then we need to determine file in the tree with :
:** "DATA:XXX" Application internal data
:** "USERDATA:XXX" User save data (like game save)
:** "HOME:XXX" User home folder
:** "/XXX" Direct acces on a file in the fileSystem
:** ...

When you will call a file, you need to just call it with the starting name.

For example if I want to acces at an application data I will call the file : "DATA:myImage.png"

== Integrate a file in a package ==

In your lutin_xxx.py file add:
[code style=python]
	# to copy a single file:
	myModule.copy_file("relative/path/file.svg","destination/folder/file.svg")
	# to copy an entire patern path
	myModule.copy_folder("relative/path/start*.png","destination/folder/")
[/code]

And now you can acces on these file with : "DATA:destination/folder/file.svg"


== Read a file ==

[code style=c++]
	#include <etk/os/FSNode.h>
	
	...
	
	etk::FSNode file("DATA:destination/folder/file.svg");
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
	std::string output;
	file.fileGets(output);
	APPL_INFO("and the end of the line ='" << output << "'");
	// close the file (note : if you did not do it, it will be close automaticly with an error)
	file.fileClose();
[/code]

== Write a file ==

[code style=c++]
	#include <etk/os/FSNode.h>
	
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
[/code]

== 'Theme' management ==

The theme management is a subset a file selected by a main key.
For example The basic theme of an API can be manage with only 2 commands (set the theme, and request upate of GUI)

At the start of the program, you might specify the default path theme:

[code style=c++]
	etk::theme::setNameDefault("GUI_COLOR", "theme/black");
	etk::theme::setNameDefault("GUI_SHAPE", "theme/rounded");
[/code]

And when you want to change the theme, just call:

[code style=c++]
	// change the theme :
	etk::theme::setName("GUI_COLOR", "theme/white");
	// force reload of all the resources :
	ewol::getContext().getResourcesManager().reLoadResources();
	ewol::getContext().forceRedrawAll();
[/code]

[note]
This is not done automaticly, because reloading the resources can have a real cost of time.
[/note]

You can acces on your theme with accessing the filename: "THEME:GUI_COLOR:your/sub/path/file.xx"

An important think is that the theme file is searching in many path in the order:
:** USERDATA:GUI_COLOR:your/sub/path/file.xx
:** DATA:GUI_COLOR:your/sub/path/file.xx
:** USERDATA:GUI_COLOR(default):your/sub/path/file.xx
:** DATA:GUI_COLOR(default):your/sub/path/file.xx


Like this a user can overload the application theme...




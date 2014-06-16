=?= Tutorial 1: Hello Word =?=
__________________________________________________
[left][tutorial[000_Build | Previous: Download & Build]][/left] [right][tutorial[010_ObjectModel | Next: Object model]][/right]

=== Objectif ===
:** Understand basis of ewol
:** Create a simple windows with a label "Hello Word"

=== Application Sources: ===

==== Application Main: ====

A generic Ewol application is manage by creating an [class[ewol::context::Application]] that is the basis of your application.

Due to the fact the ewol librairy is a multi-platform framework, then you need to think all you will do with only one 
application and only one windows displayable at the same time.

Then we will create the application:

[code style=c++]
	class MainApplication : public ewol::context::Application {
		public:
			bool init(ewol::Context& _context, size_t _initId) {
				APPL_INFO("==> Init APPL (START)");
				// nothing to do ...
				APPL_INFO("==> Init APPL (END)");
				return true;
			}
			void unInit(ewol::Context& _context) {
				APPL_INFO("==> Un-Init APPL (START)");
				// nothing to do ...
				APPL_INFO("==> Un-Init APPL (END)");
			}
	};
[/code]

The input [class[ewol::Context]] is the main system context.

[note]
It is important to know that the system can call your application in parallele, the basic exemple of this is the Wallpaper on Android.

What is done:
** When selected, it create an intance and when it is apply Android create a new instance and remove the previous one...
[/note]

In all program we need to have a main()

To be portable on Android, that have a java main the User might the Wrapper call the generic main() (please do not add other things in this main).

[code style=c++]
	int main(int argc, const char *argv[]) {
		// only one things to do : 
		return ewol::run(new MainApplication(), _argc, _argv);
	}
[/code]


==== Some configuration are needed ====

In your application you can use many configuration,
none of them are set in static for compilation and interface reason,
then you will to set it on dynamic.


Select fonts:

This can be a problem when you design an application for some other operating system (OS),
They do not have the same default font.

And we select an order to search the font names and the system basic size.
[code style=c++]
	// Use External font depending on the system (for specific application, it is better to provide fonts)
	_context.getFontDefault().setUseExternal(true);
	// Select font in order you want : if Ewol find FreeSerif, it selected it ...
	_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
[/code]


==== Main Windows: ====

Create the main Windows:

For this point we will create a class that herited form the basic windows class:

[b]Windows.h[/b]
[code style=c++]
	#ifndef __APPL_WINDOWS_H__
	#define __APPL_WINDOWS_H__
	
	#include <ewol/widget/Windows.h>
	
	namespace appl {
		class Windows : public ewol::widget::Windows {
			public:
				Windows(void);
				virtual ~Windows(void) {};
		};
	};
	#endif
[/code]

[b]Windows.cpp[/b]
[code style=c++]
	#include <ewol/ewol.h>
	#include <appl/debug.h>
	#include <appl/Windows.h>
	#include <ewol/widget/Label.h>
	
	#undef __class__
	#define __class__ "Windows"
	
	appl::Windows::Windows(void) {
		setTitle("example 001_HelloWord");
		ewol::object::Shared<ewol::widget::Label> tmpWidget = new ewol::widget::Label();
		if (NULL == tmpWidget) {
			APPL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->setLabel("Hello <font color='blue'>Word</font>");
			tmpWidget->setExpand(bvec2(true,true));
			setSubWidget(tmpWidget);
		}
	}
[/code]

The fist basic property to set is the Title:
[code style=c++]
	setTitle("example 001_HelloWord");
[/code]

After we simple create a [class[widget::Label]] in the main windows constructor.
And we set the widget property (label).
[code style=c++]
	ewol::object::Shared<ewol::widget::Label> tmpWidget = ewol::object::makeShared(new ewol::widget::Label());
	tmpWidget->setLabel("Hello <font color=\"blue\">Word</font>");
	tmpWidget->setExpand(bvec2(true,true));
[/code]
We can se in this example that the label have some other property like the font color.

The label can have decorated text based on the html generic writing but it is composed with really simple set of balise.
I will take a really long time to create a real html parser, the the availlable property is:
:** [b]<br/>[/b] : New line
:** [b]<font color="#FF0000\"> ... </font>[/b] :  change the font color.
:** [b]<center> ... </center>[/b] : center the text.
:** [b]<left> ... </left>[/b] : Set the text on the left.
:** [b]<right> ... </right>[/b] : Set the text on the right.
:** [b]<justify> ... </justify>[/b] : Set the text mode in justify.

[note]
The xml parser is a little strict on the case and end node (!! </br> !!),
but it support to:
:** Not have a main node.
:** replace '"' with ''' to simplify xml writing in C code.
[/note]

The last step is to add the widget on the windows :
[code style=c++]
	setSubWidget(tmpWidget);
[/code]


==== Configure Ewol to have display the windows ====

At this point we have created the basic windows.
But the system does not know it.
Then we create windows and set it in the main contect main (in the MainApplication::init()):
[code style=c++]
	ewol::object::Shared<ewol::Windows> basicWindows = ewol::object::makeShared(new appl::Windows());
	// create the specific windows
	_context.setWindows(basicWindows);
[/code]

Then the init fuction is :
[code style=c++]
bool MainApplication::init(ewol::Context& _context, size_t _initId) {
	APPL_INFO("==> Init APPL (START)");
	// select internal data for font ...
	_context.getFontDefault().setUseExternal(true);
	_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
	
	ewol::Windows* basicWindows = new appl::Windows();
	// create the specific windows
	_context.setWindows(basicWindows);
	APPL_INFO("==> Init APPL (END)");
	return true;
}
[/code]

To un-init the application, the context call a generic function [b]MainApplication::unInit[/b].
In this function we just need to remove the windows and un-init all needed by the system.
[code style=c++]
void MainApplication::unInit(ewol::Context& _context) {
	APPL_INFO("==> Un-Init APPL (START)");
	// Windows is auto-removed just before
	APPL_INFO("==> Un-Init APPL (END)");
}
[/code]


[note]
You can use many windows and select the one you want to display, but I do not think it is the best design.
[/note]

=== Build declaration: ===

ewol commonly use the [b]lutin.py[/b] build system.

Then we need to add a "lutin_YourApplicationName.py", then for this example: [b]lutin_001_HelloWord.py[/b]


[code style=python]
#!/usr/bin/python
import lutinModule as module
import lutinTools as tools

# optionnal : Describe in the "lutin.py --help"
def get_desc():
	return "Tutorial 001 : Hello Word"

# Module creation instance (not optionnal)
def create(target):
	# module name is '001_HelloWord' and type binary.
	myModule = module.Module(__file__, '001_HelloWord', 'BINARY')
	# add the file to compile:
	myModule.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/Windows.cpp',
		])
	# add Library dependency name
	myModule.add_module_depend(['ewol'])
	# add application C flags
	myModule.compile_flags_CC([
		"-DPROJECT_NAME=\"\\\""+myModule.name+"\\\"\""])
	# Add current include Path
	myModule.add_path(tools.get_current_path(__file__))
	return the created module
	return myModule
[/code]

show lutin doc for more information...

[note]
I do not explain again the lutin file, for next tutorial, show example sources ...
[/note]

=== Build your application ===

go to your workspace folder and launch
[code style=shell]
	./ewol/build/lutin.py -C -mdebug -p 001_HelloWord
[/code]

Your program example will build correctly...

Launch it :
[code style=shell]
	./out/Linux/debug/staging/gcc/001_HelloWord/usr/bin/001_HelloWord -l6
[/code]

The [b]-l6[/b] is used to specify the Log level of the application display (this log is synchronous)

The output compile in a separate folder depending on the compilation tool (gcc or clang) 

It create a complete final tree in the ./out/Linux/debug/staging/gcc/001_HelloWord/ folder

The final folder contain the package generated

tree of the output
:** out
::** MacOs
::** Android
::** Windows
::** ...
::** Linux
:::** release
:::** debug
::::** build
:::::** clang
:::::** gcc
::::::** ewol
::::::** exml
::::::** ejson
::::::** 001_HelloWord
::::::** ...
::::** staging
:::::** clang
:::::** gcc
::::::** 001_HelloWord
:::::::** usr
::::::::** bin
::::::::** share
::::** final
:::::** 001_HelloWord.deb


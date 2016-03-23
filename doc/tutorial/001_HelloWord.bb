
=== Objectif ===
:** Understand basis of ewol
:** Create a simple windows with a label "Hello Word"

=== Application Sources: ===

==== Application Main: ====

A generic Ewol application is manage by creating an [class[ewol::context::Application]] that is the basis of your application.

Due to the fact the ewol library is a multi-platform framework (base on [lib[gale|Gale]]), you will have many contraint like:
:** One application at the same time (note an exception for android wallpaper)
:** One Windows displayable at the time (main point of view of apple developpers)
:** Not a big CPU ...

Then we will create the application:

[code style=c++]
namespace appl {
	class MainApplication : public ewol::context::Application {
		public:
			void onCreate(ewol::Context& _context) override {
				APPL_INFO("==> CREATE ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> CREATE ... " PROJECT_NAME " (END)");
			}
			void onStart(ewol::Context& _context) override {
				APPL_INFO("==> START ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> START ... " PROJECT_NAME " (END)");
			}
			void onResume(ewol::Context& _context) override {
				APPL_INFO("==> RESUME ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> RESUME ... " PROJECT_NAME " (END)");
			}
			void onPause(ewol::Context& _context) override {
				APPL_INFO("==> PAUSE ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> PAUSE ... " PROJECT_NAME " (END)");
			}
			void onStop(ewol::Context& _context) override {
				APPL_INFO("==> STOP ... " PROJECT_NAME " (START)");
				// nothing to do ...
				APPL_INFO("==> STOP ... " PROJECT_NAME " (END)");
			}
			void onDestroy(ewol::Context& _context) override {
				APPL_INFO("==> DESTROY ... " PROJECT_NAME " (START)");
				// nothing to do ...
				APPL_INFO("==> DESTROY ... " PROJECT_NAME " (END)");
			}
	};
};
[/code]

The input [class[ewol::Context]] is the main system context.

[note]
It is important to know that the system can create your application multiple times, the basic exemple of this is the Wallpaper on Android.

What is done:
** When we select the wallpaper it create a new application (to show an example)
** When applying your choice, it create the real one an remove the previous one.
[/note]

In all program we need to have a main()

To be portable on Android, the "main" in the java might call your main through the Android wrapper.

To simplify compabilities between platform it is recommanded to not add other things in the application main:

[code style=c++]
	int main(int argc, const char *argv[]) {
		// only one things to do : 
		return ewol::run(new appl::MainApplication(), _argc, _argv);
	}
[/code]


==== Some configuration are needed ====

In your application you can use many configuration, it is really better to set all your configuration dynamic.
With this basic condiction will simplify the interface of the library if you would have many different application
(never forger the compilator garbage collector is really very efficient).



[b]Select fonts:[/b]

This can be a problem when you design an application for some other operating system (OS),
They do not have the same default font.

We select an order to search the font names and the system basic size.
[code style=c++]
	// Use External font depending on the system (for specific application, it is better to provide fonts)
	_context.getFontDefault().setUseExternal(true);
	// Select font in order you want : if Ewol find FreeSerif, it selected it ...
	_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
[/code]


==== Main Windows: ====

Create the main Windows:

For this point we will create a class that herited form the basic [class[ewol::widget::Windows]] class:

[b]Windows.h[/b]
[code style=c++]
	#pragma once
	#include <ewol/widget/Windows.h>
	namespace appl {
		class Windows;
		using WindowsShared = ememory::SharedPtr<appl::Windows>;
		using WindowsWeak = ememory::WeakPtr<appl::Windows>;
		class Windows : public ewol::widget::Windows {
			protected:
				Windows(void);
				init();
			public:
				DECLARE_FACTORY(Windows);
				virtual ~Windows(void) {};
		};
	};
[/code]

The C macro "DECLARE_FACTORY" create a simple factory function "create" that return the [class[ewol::Object]] well create.

For some internal reason, we create the object and we call the "init" function after creating the object. When well done we return the shared object created.

See [tutorial[010_ObjectModel | Next: Object model]] to understand why this structure is so complex.

[b]Windows.cpp[/b]
[code style=c++]
	#include <ewol/ewol.h>
	#include <appl/debug.h>
	#include <appl/Windows.h>
	#include <ewol/widget/Label.h>
	
	#undef __class__
	#define __class__ "Windows"
	
	appl::Windows::Windows() {
		addObjectType("appl::Windows");
		propertyTitle.setDirectCheck(std::string("sample ") + PROJECT_NAME);
	}
	void appl::Windows::init() {
		ewol::widget::Windows::init();
		ewol::widget::LabelShared tmpWidget = ewol::widget::Label::create();
		if (tmpWidget == nullptr) {
			APPL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->propertyValue.set("Hello <font color='blue'>World</font>");
			tmpWidget->propertyExpand.set(bvec2(true,true));
			setSubWidget(tmpWidget);
		}
	}
[/code]

The init function is virtual and you must call your parent object (or at least the [class[ewol::Object]] init)
[code style=c++]
	ewol::widget::Windows::init();
[/code]

The title is assiciated on the current windows then it is a simple [class[ewol::widget::Windows]] property.
Please use the "setDirectCheck" fucntion instead of "set" function when you are in the constructor (the callback can be unstable when we construct the object)
[code style=c++]
	propertyTitle.setDirectCheck(std::string("sample ") + PROJECT_NAME);
[/code]

The object [class[ewol::widget::Windows]] is a simple container. But the reference between Object is shared_ptr, and this is not accessible in the constructor. This is the reason we use init function.

After we simple create a [class[widget::Label]] in the main windows init.
We set label and basic properties:
[code style=c++]
	std::shared_ptr<ewol::widget::Label> tmpWidget = ewol::widget::Label::create();
	tmpWidget->propertyValue.set("Hello <font color='blue'>World</font>");
	tmpWidget->propertyExpand.set(bvec2(true,true));
[/code]
We can see in this example that the label have some other property like the font color.


The label can have decorated text based on the html generic writing but it is composed with really simple set of balise.
I will take a really long time to create a real html parser.

The availlable property is:
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

The last step is to add the widget on the windows:
[code style=c++]
	setSubWidget(tmpWidget);
[/code]
When we call this function, it use the shared_from_this() function that create an exception if we are in constructor (when setting the wub-widget parrent)


==== Configure Ewol to have display the windows ====

At this point we have created the basic windows.
But the system does not know it.
Then we create windows and set it in the main context main (in the appl::MainApplication::init()):
[code style=c++]
	ewol::WindowsShared basicWindows = appl::Windows::create());
	// create the specific windows
	_context.setWindows(basicWindows);
[/code]
Here we call the create function that is created by the DECLARE_FACTORY macro


Then the init fuction is:
[code style=c++]
bool MainApplication::init(ewol::Context& _context, size_t _initId) {
	APPL_INFO("==> Init APPL (START)");
	// select internal data for font ...
	_context.getFontDefault().setUseExternal(true);
	_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
	
	ewol::WindowsShared basicWindows = appl::Windows::create();
	// create the specific windows
	_context.setWindows(basicWindows);
	APPL_INFO("==> Init APPL (END)");
	return true;
}
[/code]

[note]
You can use many windows and select the one you want to display, but I do not think it is the best design.
[/note]

=== Build declaration: ===

ewol commonly use the [b]lutin[/b] build system.

Then we need to add a "lutin_YourApplicationName.py", then for this example: [b]lutin_ewol-sample-HelloWord.py[/b]


[code style=python]
#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools

def get_type():
	return "BINARY"

def get_sub_type():
	return "SAMPLE"

def get_desc():
	return "Tutorial 001 : Hello Word"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'appl/Main.cpp',
		'appl/debug.cpp',
		'appl/Windows.cpp',
		])
	my_module.add_module_depend(['ewol'])
	my_module.compile_flags('c++', [
		"-DPROJECT_NAME=\"\\\""+my_module.name+"\\\"\"",
		"-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	my_module.add_path(tools.get_current_path(__file__))
	return my_module
[/code]

show lutin doc for more information...

[note]
I do not explain again the lutin file, for next tutorial, show example sources ...
[/note]

=== Build your application ===

Go to your workspace folder and launch
[code style=shell]
	lutin -C -mdebug ewol-sample-HelloWord
	# or
	lutin -C -mdebug ewol-sample-HelloWord?build
[/code]

Your program example will build correctly...

Launch it :
[code style=shell]
	lutin -C -mdebug ewol-sample-HelloWord?run
[/code]

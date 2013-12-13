=?= Tutorial 1: Hello Word =?=
__________________________________________________
[left][tutorial[000_Build | Previous: Download & Build]][/left] [right][tutorial[002_HelloWord | Next: Hello Word]][/right]

=== Objectif ===
:** Understand basis of ewol
:** Create a simple windows with a label "Hello Word"

=== Application Sources: ===

==== Main Windows: ====

==== Application "main()": ====

=== Build declaration: ===


=== Build your application ===



In all the application we need to have a main, for some reason this main is stored by the application and only call the EWOL main:

[code style=c++]
	int main(int argc, const char *argv[]) {
		// only one things to do : 
		return ewol::run(argc, argv);
	}
[/code]

Then the first question, is where is the input of the application:

[code style=c++]
	// application start:
	bool APP_Init(ewol::Context& _context) {
		return true;
	}
	// application stop:
	void APP_UnInit(ewol::Context& _context) {
	}
[/code]

The input [class[ewol::eContext]] is the main application context.
All the application globals have a reference in this element.
It is important to note that the system can call you some time in parallele, the basic exemple of this is the Wallpaper on Android.
When selected, it create an intance and when it is apply Android create a new instance and remove the previous one...



Now we will create some generic property:

In first: set the font availlagle on the global font property (system font).
This can be a problem when you designe an application for some other operating system (OS), They do not have the same default font.
and we select an order to search the font names and the system basic size.
[code style=c++]
	_context.getFontDefault().setUseExternal(true);
	_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
[/code]


In second: we will create a windows.

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
			public:
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
		ewol::widget::Label* tmpWidget = new ewol::widget::Label();
		if (NULL == tmpWidget) {
			APPL_ERROR("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->setLabel("Hello <font color=\"blue\">Word</font>");
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
	ewol::widget::Label* tmpWidget = new ewol::widget::Label();
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

[note] The xml parser is a little strict on the case and end node, but it support to not have a main node.[/note]


The last step is to add the widget on the windows :
[code style=c++]
	setSubWidget(tmpWidget);
[/code]


At this point we have created the basic windows.
But the system does not know it.
Then we create windows and set it in the main contect main (in the APPL_init()):
[code style=c++]
	ewol::Windows* basicWindows = new appl::Windows();
	// create the specific windows
	_context.setWindows(basicWindows);
[/code]


Then the init fuction is :
[code style=c++]
bool APP_Init(ewol::Context& _context) {
	// select internal data for font ...
	_context.getFontDefault().setUseExternal(true);
	_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
	
	ewol::Windows* basicWindows = new appl::Windows();
	// create the specific windows
	_context.setWindows(basicWindows);
	return true;
}
[/code]

To un-init the application, the context call a generic function [b]APP_UnInit[/b].
In this function we just need to remove the windows and un-init all needed by the system.
[code style=c++]
void APP_UnInit(ewol::Context& _context) {
	// The main windows will be auto-remove after this call if it is not done...
}
[/code]

=?=Ewol extract and build examples=?=
__________________________________________________
[left][doc[001_bases | Previous: Doc]][/left] [right][tutorial[001_HelloWord | Next: Hello-Word]][/right]

All developpement software will start by getting the dependency and the sources.

=== Linux dependency packages ===
[code style=shell]
	sudo apt-get install g++ libgl1-mesa-dev zlib1g-dev libasound2-dev
	# if you want to compile with clang :
	sudo apt-get install clang
	# For andoid compilation (jdk 7 does not work...)
	sudo apt-get install javacc openjdk-6-jdk
	# if you want to compile for windows :
	sudo apt-get install mingw32
	# on 64 bits processor for compatibility
	sudo apt-get install ia32-libs
[/code]


=== Download instructions ===

Download the software :
[code style=shell]
	# create a working directory path
	mkdir your_workspace_path
	cd your_workspace_path
	# clone ewol and all sub-library
	git clone git://github.com/HeeroYui/ewol.git
	cd ewol
	git submodule init
	git submodule update
	cd ..
	# clone the exemple repository
	git clone git://github.com/HeeroYui/example.git
[/code]

[note]
The full build tool documentation is availlable here : [[http://heeroyui.github.io/lutin/ | lutin]]
[/note]

=== Common build instructions ===

Compile software in debug for the curent platform :
[code style=shell]
	./ewol/build/lutin.py -mdebug
[/code]

You can specify the platform with:
[code style=shell]
	./ewol/build/lutin.py -mdebug -tAndroid
[/code]

It coud be usefull to disable the package generation in local debug :
[code style=shell]
	./ewol/build/lutin.py -mdebug -p
[/code]

Build with clang instead of gcc:
[code style=shell]
	./ewol/build/lutin.py -C -cclang
[/code]

Display the build in color :
[code style=shell]
	./ewol/build/lutin.py -C -mdebug -p
[/code]

Build in realease and install the program named 'edn'
[code style=shell]
	./ewol/build/lutin.py -C edn-install
[/code]

To run an application you will find it directly on the out 'staging' tree :
[code style=shell]
	./out/Linux/debug/staging/clang/edn/usr/bin/edn
[/code]

== Simple explanation : ==

The workspace is a simple folder that contain all the modules ans sub module.
It will create a tree like this :

:** workspace
::** ewol
:::** external [i](ewol external dependency sub-lib)[/i]
::::** agg
::::** bullet
::::** date
::::** egami
::::** ege
::::** ejson
::::** etk
::::** exml
::::** freetype
::::** glew
::::** lua
::::** ogg
::::** png
::::** portaudio
::::** z
:::** souces
::::** ewol
::** example
::** youApplication_1
::** youApplication_2
::** youOtherLib_1
::** youOtherLib_2
::** out


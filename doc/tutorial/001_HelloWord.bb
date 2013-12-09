== [center]Tutorial 1: Hello Word[/center] ==
__________________________________________________




=== Instructions ===
download the software :
[code style=shell]
	git clone git://github.com/HeeroYui/ewol.git
	cd ewol
	git submodule init
	git submodule update
[/code]


Compile software and install :
[code style=shell]
	For this you might use a project that create binaries or something else ...
	Show http://github.com/HeeroYui/edn project
[/code]


=== Dependency packages ===
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
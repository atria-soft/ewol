=?=Ewol extract and build examples=?=
__________________________________________________
[left][doc[001_bases | Previous: Doc]][/left] [right][tutorial[001_HelloWord | Next: Hello-Word]][/right]

All developpement software will start by getting the dependency and the sources.

=== Linux dependency packages ===

==== Ubuntu or Debian ====
[code style=shell]
	sudo apt-get install g++ libgl1-mesa-dev zlib1g-dev libasound2-dev
	# Compile with Clang:
	sudo apt-get install clang
	# For andoid compilation (jdk 7 does not work...):
	sudo apt-get install javacc openjdk-6-jdk
	# Cross compile for windows:
	sudo add-apt-repository ppa:ubuntu-toolchain-r/test
	sudo apt-get update
	sudo apt-get install gcc-mingw-w64
	# On 64 bits processor for compatibility:
	sudo apt-get install ia32-libs
	sudo apt-get install g++-multilib libc6-dev-i386
[/code]

==== Arch-linux ====
[code style=shell]
	# Cross compile for windows:
	pacman -S mingw-w64-gcc
	
	# Cross compile for Android:
	in /etc/pacman.conf file uncomment:
		[multilib]
		Include = /etc/pacman.d/mirrorlist
	# update the system:
	Pacman -Syu
	# install lib C:
	pacman -S lib32-glibc lib32-zlib lib32-gcc-libs
	# install open-jdk 7.0
	pacman -S jdk7-openjdk
	# connect adb: (and you can do a "android/sdk/platform-tools/adb shell" to enable computer key on device)
	pacman -S android-udev
[/code]

=== Download instructions ===

==== download Build system: ====

[code style=shell]
	sudo pip install lutin
	sudo pip install pillow
[/code]

==== need google repo: ====

see: http://source.android.com/source/downloading.html#installing-repo

[code style=shell]
	mkdir ~/.bin
	PATH=~/.bin:$PATH
	curl https://storage.googleapis.com/git-repo-downloads/repo > ~/.bin/repo
	chmod a+x ~/.bin/repo
[/code]
==== download the software: ====

[code style=shell]
	mkdir WORKING_DIRECTORY
	cd WORKING_DIRECTORY
	repo init -u git://github.com/atria-soft/manifest.git
	repo sync -j8
[/code]

==== Compile software and test: ====

[code style=shell]
	lutin ewol-*
[/code]

[note]
The full build tool documentation is availlable here : [[http://heeroyui.github.io/lutin/ | lutin]]
[/note]

=== Common build instructions ===

Compile software in debug for the curent platform :
[code style=shell]
	lutin -mdebug
[/code]

You can specify the platform with:
[code style=shell]
	lutin -tAndroid -mdebug
[/code]

It coud be usefull to disable the package generation in local debug:
[code style=shell]
	lutin -mdebug -p
[/code]

Build with clang instead of gcc:
[code style=shell]
	lutin -cclang
[/code]

Display the build in color :
[code style=shell]
	lutin -C -mdebug -p
[/code]

Build in realease and install the program named 'ewol-sample-HelloWord'. Note the install will install it in user mode in the ~/.local/application/ in a stand-alone mode
[code style=shell]
	lutin -C ewol-sample-HelloWord?install
	#or
	lutin -C ewol-sample-HelloWord@install
[/code]

To run an application you will find it directly on the out 'staging' tree or execute the command:
[code style=shell]
	lutin -C ewol-sample-HelloWord@run
	#or (with better log level
	lutin -C ewol-sample-HelloWord@run:--elog-level=5
	# or specify the lib
	lutin -C ewol-sample-HelloWord@run:--elog-lib=etk:6
[/code]

== Simple explanation : ==

The workspace is a simple folder that contain all the modules ans sub module availlable for build.
It will create a tree like this :

:** workspace
::** application
:::** Application clone application area.
::** framework
:::** atria-soft
::::** Graphic interface
:::** generic-library
::::** common untuch library (just wrap in lutin mode)
:::** HeeroYui
::::** unstable stuff
:::** musicdsp
::::** Common library for audio interfacing
:::** tools

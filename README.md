Ewol
====

`Ewol` (Edn Widget OpenGl Layer) is a FREE software.

Release (master)
----------------

[![Build Status](https://travis-ci.org/atria-soft/ewol.svg?branch=master)](https://travis-ci.org/atria-soft/ewol)
[![Coverage Status](http://atria-soft.com/ci/coverage/atria-soft/ewol.svg?branch=master)](http://atria-soft.com/ci/atria-soft/ewol)
[![Test Status](http://atria-soft.com/ci/test/atria-soft/ewol.svg?branch=master)](http://atria-soft.com/ci/atria-soft/ewol)
[![Warning Status](http://atria-soft.com/ci/warning/atria-soft/ewol.svg?branch=master)](http://atria-soft.com/ci/atria-soft/ewol)

Developement (dev)
------------------

[![Build Status](https://travis-ci.org/atria-soft/ewol.svg?branch=dev)](https://travis-ci.org/atria-soft/ewol)
[![Coverage Status](http://atria-soft.com/ci/coverage/atria-soft/ewol.svg?branch=dev)](http://atria-soft.com/ci/atria-soft/ewol)
[![Test Status](http://atria-soft.com/ci/test/atria-soft/ewol.svg?branch=dev)](http://atria-soft.com/ci/atria-soft/ewol)
[![Warning Status](http://atria-soft.com/ci/warning/atria-soft/ewol.svg?branch=dev)](http://atria-soft.com/ci/atria-soft/ewol)

Instructions
============

need google repo:
-----------------

see: http://source.android.com/source/downloading.html#installing-repo

	mkdir ~/.bin
	PATH=~/.bin:$PATH
	curl https://storage.googleapis.com/git-repo-downloads/repo > ~/.bin/repo
	chmod a+x ~/.bin/repo


download the software:
----------------------

	mkdir WORKING_DIRECTORY
	cd WORKING_DIRECTORY
	sudo pip install lutin
	sudo pip install pillow
	repo init -u git://github.com/HeeroYui/manifest.git
	repo sync -j8


Compile software and install:
-----------------------------

	For this you might use a project that create binaries or something else ...
	Show http://github.com/HeeroYui/edn project

Dependency packages
===================

Ubuntu Or Debian:

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

Arch-linux:

	# Cross compile for windows:
	pacman -S mingw-w64-gcc
	
	# Cross compile for Android:
	in /etc/pacman.conf file uncomment:
		[multilib]
		Include = /etc/pacman.d/mirrorlist
	# update the system:
	Pacman -Syu
	# install lib C:
	pacman -S lib32-glibc

License (APACHE v2.0)
=====================

Copyright ewol Edouard DUPIN

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


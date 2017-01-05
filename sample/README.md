Ewol Samples
============

Ewol sample are a FREE software for learn use of ewol.

Instructions
============

download the software :

	git clone git://github.com/HeeroYui/ewol.git
	cd ewol
	git submodule init
	git submodule update

Compile software and install : (build all binary and libs)

	ewol/build/lutin.py -j4

Dependency packages
===================

	sudo apt-get install g++ libgl1-mesa-dev zlib1g-dev libasound2-dev
	# if you want to compile with clang :
	sudo apt-get install clang
	# For andoid compilation (jdk 7 does not work...)
	sudo apt-get install javacc openjdk-6-jdk
	# if you want to compile for windows :
	sudo add-apt-repository ppa:ubuntu-toolchain-r/test
	sudo apt-get update
	sudo apt-get install gcc-mingw-w64
	# on 64 bits processor for compatibility
	sudo apt-get install ia32-libs
	sudo apt-get install g++-multilib libc6-dev-i386

License (MPL v2.0)
=====================

Copyright ewol Edouard DUPIN

Licensed under the Mozilla Public License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.mozilla.org/MPL/2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


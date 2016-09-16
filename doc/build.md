Build lib & build sample                           {#ewol_build}
========================

@tableofcontents
Linux dependency packages                          {#ewol_build_dependency}
=========================

Ubuntu or Debian                                   {#ewol_build_dependency_debian}
----------------

```{.sh}
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
```

Arch-linux                                         {#ewol_build_dependency_archlinux}
----------
```{.sh}
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
```

Download:                                          {#ewol_build_download}
=========

ewol use some tools to manage source and build it:

need google repo:                                  {#ewol_build_download_repo}
-----------------

see: http://source.android.com/source/downloading.html#installing-repo

On all platform:
```{.sh}
	mkdir ~/.bin
	PATH=~/.bin:$PATH
	curl https://storage.googleapis.com/git-repo-downloads/repo > ~/.bin/repo
	chmod a+x ~/.bin/repo
```

On ubuntu
```{.sh}
	sudo apt-get install repo
```

On archlinux
```{.sh}
	sudo pacman -S repo
```

lutin (build-system):                              {#ewol_build_download_lutin}
---------------------

```{.sh}
	pip install lutin --user
	# optionnal dependency of lutin (manage image changing size for application release)
	pip install pillow --user
```

The full build tool documentation is availlable here : [lutin](http://heeroyui.github.io/lutin/)


dependency:                                        {#ewol_build_download_dependency}
-----------

```{.sh}
	mkdir -p WORKING_DIRECTORY/framework
	cd WORKING_DIRECTORY/framework
	repo init -u git://github.com/atria-soft/manifest.git
	repo sync -j8
	cd ../..
```

sources:                                           {#ewol_build_download_sources}
--------

They are already download in the repo manifest in:

```{.sh}
	cd WORKING_DIRECTORY/framework/atria-soft/ewol
```

Build:                                             {#ewol_build_build}
======

you must stay in zour working directory...
```{.sh}
	cd WORKING_DIRECTORY
```

library:                                           {#ewol_build_build_library}
--------

```{.sh}
	lutin -mdebug ewol
```

Sample:                                            {#ewol_build_build_sample}
-------

```{.sh}
	lutin -mdebug ewol-sample-*
```

Run sample:                                        {#ewol_build_run_sample}
-----------

Basic way

```{.sh}
	lutin -mdebug ewol-sample-*?run
```

With some option: (set global log leval at 2 (print, error, warning), and "appl" library at log level 6

```{.sh}
	lutin -mdebug ewol-sample-*?run:--elog-level=2:--elog-lib=appl:6
```


Build for Android and install:                     {#ewol_build_android}
==============================

Sample:                                            {#ewol_build_android_sample}
-------

```{.sh}
	lutin -tAndroid -mdebug ewol-sample-*
```

Sample:                                            {#ewol_build_android_install_sample}
-------

```{.sh}
	lutin -tAndroid -mdebug ewol-sample-*?install
```

Worktree explanation:                              {#ewol_build_workspace}
=====================

The workspace is a simple folder that contain all the modules ans sub module availlable for build.
It will create a tree like this :

  - **workspace**
    * **application:** set your application here, it is a good position
    * **framework:** framework download by repo
      + **atria-soft:** graphic framework
      + **generic-library:** open sources library that is wrapped on lutin builder
      + **musicdsp:** Common library for audio interfacing
      + **tools:** build tools (now only the IOs flasher)
    * **out:**
      + Android_arm_32
      + Android_arm_64
      + Windows_x86_32
      + Windows_x86_64
      + Linux_x86_32
      + Linux_x86_64
      + MacOs_x86_32
      + MacOs_x86_64
      + IOs_x86_32
      + IOs_x86_64

All the build object are set in the out path, then to restart with a clean build simply remove this folder



EWOL: Bases                                {#ewol_page_bases}
===========

@tableofcontents

Overview:                                  {#ewol_page_bases_overview}
=========

EWOL is an OpenGL library for creating graphical user interfaces.
It works on many UNIX-like platforms, Windows, and OS X and some mobile platforms Android, iOs.
EWOL is released under the MPL v2.0 license, which allows for very flexible licensing of client applications.
EWOL has a C++ architecture that allows for maximum flexibility.

**MPL v2.0 license limitation:** I limit myself to use only Apache2 / MIT / BSD / PNG / MPL licencing to permit to release binary on IOs.
This I a big point, because it need to rewrite many libraries (like SVG...).


The main idea of EWOL is to create a complete abstraction of the platforms.
This generate some restriction that you will see an overwiew in the under section.

User requires:                             {#ewol_page_bases_require}
==============

To use ewol you need to know only C++ language. It could be usefull to know:
  - **Python** for [lutin](http://HeeroYui.github.io/lutin) build tool.
  - **git** and **repo** for all version management.
  - **OpenGL-ES2** if you want to create custum advanced widget.

If you just want to have an interface of the openGl just refer to the [gale library](http://atria-soft.github.io/gale)

Ewol does not manage the Audio but it is full integrated in [audio-river library](http://musicdsp.github.io/audio-river).


Architecture:                              {#ewol_page_bases_architecture}
=============

One of the important point to know in this framework is some of absurd things came from the multiple architecture type.

I will Explain the main points:
  - IOs does **NOT** simply support the shared object sub lib, this force ewol to be MPL v2.0, and depend on some sub-library with small license restriction.
  - Android have a JAVA main, then the application main will not be used with this platform
  - Android event (keyboard, mouse, touch-screen and ...) will arrive in asynchron mode ==> need to be resynchronyse in one thread
  - Only one graphyc framework is availlable on all platform. This is OpenGL (windows phone is not supported (moribund archi))
  - Main interesting point is packaging of the application data:
    * Linux store it in /usr/share/applName/*
    * MacOs store it in applName.app/subFolder/*
    * Android store it in the .pkg that is a renamed .zip that the name is dynamic
    * Windows (TODO : Never done a pakage)
      This will generate a complex result of data access...
  - Sub lib Change on all the platform, then I will use the idea of Apple, that incluse in a package all needed libs.
    This could be a problem for small platform, but this framwork need to have a OpenGL-ES2 instance then the memory problem,
    is not really a problem.


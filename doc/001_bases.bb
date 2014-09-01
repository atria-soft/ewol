=?=EWOL: Bases =?=
__________________________________________________
[right][tutorial[000_Build | Next: Tutorals]][/right]

=== Overview:===
EWOL is an OpenGL library for creating graphical user interfaces.
It works on many UNIX-like platforms, Windows, and OS X and some mobile platforms Android, iOs(soon).
EWOL is released under the APACHE-2 license, which allows for very flexible licensing of client applications.
EWOL has a C++ architecture that allows for maximum flexibility.

The main idea of EWOL is to create a complete abstraction of the platforms.
This generate some restriction that you will see an overwiew in the under section.

===User requires:===
To use ewol you need to know only C++ language. It could be usefull to know:
:** [b]Python[/b] for all build tool.
:** [b]git[/b] for all version management
:** [b]OpenGL-ES2[/b] if you want to create custum advenced widget.

=== Architecture:===
One of the important point to know in this framwork is some of absurd things came from the multiple architecture type.

I will Explain the main points:
:** IOs does [b]NOT[/b] simply support the shared object sub lib, this force ewol to be APACHE-2, and depend on some sub-library with small license restriction.
:** Android have a JAVA main, then the application main will not be used with this platform
:** Android event (keyboard, mouse, touch-screen and ...) will arrive in asynchron mode ==> need to be resynchronyse in one thread
:** Only one graphyc framework is availlable on all platform. This is OpenGL
:** Main interesting point is packaging of the application data:
::** Linux store it in /usr/share/applName/*
::** MacOs store it in applName.app/subFolder/*
::** Android store it in the .pkg that is a renamed .zip that the name is dynamic
::** Windows (TODO : Never done a pakage)
:: This will generate a complex result of data access...
:** Sub lib Change on all the platform, the I will use the idea of Apple, that incluse in a package all needed libs. This could be a problem for small platform, but this framwork need to have a OpenGL-ES2 instance then the memory problem, is not really a problem.


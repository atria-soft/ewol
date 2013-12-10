== [center]Ewol library[/center] ==
__________________________________________________

===What is EWOL, and how can I use it?===
EWOL, or Edn Widget OpenGl Layer, is a multi-platform library for creating graphical user interfaces in OpenGL. Offering a complete set of widgets.

===Where can I use it?===
Everywhere! EWOL is cross-platform devolopped to support bases OS:
: ** Linux (X11) (mouse)
: ** Windows (mouse)
: ** MacOs (mouse)
: ** Android (mouse + touch)
: ** IOs (in-progress)

===What languages are supported?===
EWOL is written in C++ and is not (for now) supported for other languages.

===Are there any licensing restrictions?===
EWOL is [b]FREE software[/b] and [i]all sub-library are FREE and staticly linkable !!![/i]

That allow you to use it for every program you want, including those developing proprietary software, without any license fees or royalties.

[note]The static support is important for some platform like IOs, and this limit the external library use at some license like :
:** BSD*
:** MIT
:** APPACHE
:** PNG
:** ZLIB
This exclude the classical extern library with licence:
:** L-GPL
:** GPL
[/note]

==== License (DSB) ====
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

:** Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
:** Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
:** The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


==== Sub library: ====
===== License: =====
:** [b][lib[etk | e-tk]][/b] : BSD 3 clauses
::** [b][lib[linearmath | Linear-Math]][/b] : z-lib (subset of bullet lib)
::** [b][lib[earchive | e-Archive]][/b] : BSD 3 clauses
:::** [b][lib[z | Z lib]][/b] : z-lib
:** [b][lib[egami | e-gami]][/b] : BSD 3 clauses
::** [b][lib[esvg | e-svg]][/b] : BSD 3 clauses
:::** [b][lib[agg | AGG]][/b] : BSD-like
::** [b][lib[png | libPNG]][/b] : PNG
:** [b][lib[portaudio | portaudio]][/b] : MIT
:** [b][lib[ogg | ogg]][/b] : BSD-like
:** [b][lib[freetype | Free-Type]][/b] : BSD-like
:** [b][lib[ejson | e-json]][/b] : BSD 3 clauses
:** [b][lib[exml | e-xml]][/b] : BSD 3 clauses


==== Description : ====
===== Internal: =====
:** [b][lib[etk | e-tk]][/b] : Generic toolkit to acces on file, standardize acces on string (for Android and MacOs) ...
:** [b][lib[earchive | e-Archive]][/b] : Generic access to a zip file (used to access on file on Android)
:** [b][lib[eggami | e-gami]][/b] : generic image accessor for PNG, svg and bmp image (might add some other type ...)
:** [b][lib[esvg | e-svg]][/b] : Generic SVG image parser and displayer
:** [b][lib[ejson | e-json]][/b] : JSON file access (read and write)
:** [b][lib[exml | e-xml]][/b] : XML file access (read and write)
:** [b][lib[ege | e-ge]][/b] : (library in BSDv3) Ewol Game engine is a wrapper on the the bullet physical engine and ewol renderer engin. this is in developpement for now (the simple objective is to produce game to make profitable all my work)

===== External: =====
:** [b][lib[linearmath | Linear-Math]][/b] : bullet mathamatical sub lib (for using same vec3).
:** [b][lib[z | Z lib]][/b] : Clkassical zlib lib.
:** [b][lib[agg | AGG]][/b] : A c++ drawing lib.
:** [b][lib[png | libPNG]][/b] : the classical png display lib.
:** [b][lib[portaudio | portaudio]][/b] : The classical audio wrapper lib.
:** [b][lib[ogg | ogg]][/b] : The classical Ogg coder reader lib.
:** [b][lib[freetype | Free-Type]][/b] : The classicle true-type reader lib.
:** [b][lib[bullet | bullet]][/b] : the classical bullet library physical engine. (dependence by ege)

===== Program Using EWOL =====
:** [b][lib[edn | edn]][/b] : (Application in GPLv3) Edn is the main application using this lib and designed for (in the first time). This is a "Code editor".

== Main documentation: ==

[doc[001_bases | Global Documantation]]

[tutorial[000_Build | Tutorials]]

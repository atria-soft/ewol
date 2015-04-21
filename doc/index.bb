== [center]Ewol library[/center] ==
__________________________________________________

===What is EWOL, and how can I use it?===
EWOL, or Edn Widget OpenGl Layer, is a multi-platform library for creating graphical user interfaces in OpenGL. Offering a complete set of widgets.

===Where can I use it?===
Everywhere! EWOL is cross-platform devolopped to support bases OS:
: ** Linux (X11) (mouse)
: ** Windows (mouse) (not compile anymore ==> TODO)
: ** MacOs (mouse)
: ** Android (mouse + touch)
: ** IOs (touch)

===What languages are supported?===
EWOL is written in C++ and is not (for now) supported for other languages.

===Are there any licensing restrictions?===
EWOL is [b]FREE software[/b] and [i]all sub-library are FREE and staticly linkable !!![/i]

That allow you to use it for every program you want, including those developing proprietary software, without any license fees or royalties.

[note]The static support is important for some platform like IOs, and this limit the external library use at some license like :
:** BSD*
:** MIT
:** APPACHE-2
:** PNG
:** ZLIB
This exclude the classical extern library with licence:
:** L-GPL
:** GPL
[/note]

==== License (APACHE 2) ====
Copyright ewol Edouard DUPIN

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	[[http://www.apache.org/licenses/LICENSE-2.0]]

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


==== Sub library: ====
===== License: =====
:** [b][lib[etk | e-tk]][/b] : APACHE-2
::** [b][lib[linearmath | Linear-Math]][/b] : z-lib (subset of bullet lib)
::** [b][lib[earchive | e-Archive]][/b] : APACHE-2
:::** [b][lib[z | Z lib]][/b] : z-lib
:** [b][lib[egami | e-gami]][/b] : APACHE-2
::** [b][lib[esvg | e-svg]][/b] : APACHE-2
:::** [b][lib[agg | AGG]][/b] : BSD-like
::** [b][lib[png | libPNG]][/b] : PNG
:** [b][lib[portaudio | portaudio]][/b] : MIT
:** [b][lib[ogg | ogg]][/b] : BSD-like
:** [b][lib[freetype | Free-Type]][/b] : BSD-like
:** [b][lib[ejson | e-json]][/b] : APACHE-2
:** [b][lib[exml | e-xml]][/b] : APACHE-2


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
:** [b][[http://play.google.com/store/apps/details?id=com.edouarddupin.worddown | worddown]][/b] : (Proprietary) Worddown is a simple word game.

== Main documentation: ==

[doc[001_bases | Global Documantation]]

[tutorial[000_Build | Tutorials]]

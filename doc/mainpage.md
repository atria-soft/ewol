Ewol library                                {#mainpage}
============

@tableofcontents

What is EWOL?                               {#ewol_mainpage_what}
=============

EWOL, or Edn Widget OpenGl Layer, is a multi-platform library for creating graphical user interfaces in OpenGL. Offering a complete set of widgets.

Where can I use it?                         {#ewol_mainpage_where}
===================

Everywhere! EWOL is cross-platform devolopped to support bases OS:
- Linux (X11) (mouse)
- Windows (mouse) (build on linux...)
- MacOs (mouse)
- Android (mouse + touch)
- IOs (touch)

What languages are supported?                   {#ewol_mainpage_language}
=============================

EWOL is written in C++ and is not (for now) supported for other languages.


Are there any licensing restrictions?           {#ewol_mainpage_restriction}
=====================================

EWOL is **FREE software** and _all sub-library are FREE and staticly linkable !!!_

That allow you to use it for every program you want, including those developing proprietary software, without any license fees or royalties.

The static support is important for some platform like IOs, and this limit the external library use at some license like:
  - BSD*
  - MIT
  - APPACHE-2
  - PNG
  - ZLIB

This exclude the classical extern library with licence:
  - L-GPL
  - GPL

License (MPL v2.0)                            {#ewol_mainpage_license}
==================

Copyright ewol Edouard DUPIN

Licensed under the Mozilla Public License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

<https://www.mozilla.org/MPL/2.0>

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Sub library:                                  {#ewol_mainpage_sub}
============

License:

  - [**etk**](http://atria-soft.github.io/etk) : MPL v2.0
  - [**elog**](http://atria-soft.github.io/elog) : MPL v2.0
    + **linearmath** : z-lib (subset of bullet lib)
    + [**earchive**](http://atria-soft.github.io/earchive) : MPL v2.0
      * **zlib** : z-lib
  - [**egami**](http://atria-soft.github.io/egami) : MPL v2.0
    + [**esvg**](http://atria-soft.github.io/esvg) : MPL v2.0
    + **libpng** : PNG
  - **libogg** : BSD-like
  - **libfreetype** : BSD-like
  - [**e-json**](http://atria-soft.github.io/ejson) : MPL v2.0
  - [**e-xml**](http://atria-soft.github.io/exml) : MPL v2.0
  - [**audio**](http://musicdsp.github.io/audio) : MPL v2.0
  - [**audio-orchestra**](http://musicdsp.github.io/audio-orchestra) : MIT
  - [**audio-drain**](http://musicdsp.github.io/audio-drain) : MPL v2.0
  - [**audio-river**](http://musicdsp.github.io/audio-river) : MPL v2.0
  - [**audio-ess**](http://musicdsp.github.io/audio-ess) : MPL v2.0
  - [**ege**](http://atria-soft.github.io/ege) : MPL v2.0
  - [**dollar**](http://atria-soft.github.io/dollar) : MPL v2.0
...

Description:                                  {#ewol_mainpage_desc}
------------

Internal:

  - [**elog**](http://atria-soft.github.io/elog) : Generic Log interface (for Android and MacOs) ...
  - [**etk**](http://atria-soft.github.io/etk) : Generic toolkit to acces on file, standardize acces on string (for Android and MacOs) ...
  - [**earchive**](http://atria-soft.github.io/earchive) : Generic access to a zip file (used to access on file on Android)
  - [**egami**](http://atria-soft.github.io/egami) : generic image accessor for PNG, svg and bmp image (might add some other type ...)
  - [**e-svg**](http://atria-soft.github.io/esvg) : Generic SVG image parser and displayer
  - [**e-json**](http://atria-soft.github.io/ejson) : JSON file access (read and write)
  - [**e-xml**](http://atria-soft.github.io/exml) : XML file access (read and write)
  - [**audio**](http://musicdsp.github.io/audio) : Basic audio format
  - [**audio-orchestra**](http://musicdsp.github.io/audio-orchestra) : Low level wrapper of audio interface (Linux/Windows/MacOs/IOs/Android)
  - [**audio-drain**](http://musicdsp.github.io/audio-drain) : single stream audio processing
  - [**audio-river**](http://musicdsp.github.io/audio-river) : High level audio abstraction
  - [**audio-ess**](http://musicdsp.github.io/audio-ess) : basic sound set manager
  - [**ege**](http://musicdsp.github.io/ege) : Ewol Game engine is a wrapper on the the bullet physical engine and ewol renderer engin. This is in developpement for now (the simple objective is to produce game to make profitable all my work)
  - [**zeus**](http://atria-soft.github.io/zeus) : RPC over websocket
  - [**dollar**](http://atria-soft.github.io/dollar) : $1 $n $p and $p+

External:                                     {#ewol_mainpage_ext}
---------

  - **linearmath** : bullet mathamatical sub lib (for using same vec3).
  - **Z lib** : Clkassical zlib lib.
  - **libPNG** : the classical png display lib.
  - **ogg** : The classical Ogg coder reader lib.
  - **freetype** : The classicle true-type reader lib.
  - **bulletlib** : the classical bullet library physical engine. (dependence by ege)
  - **openSSL** : generic SSL library. (dependence by zeus)

Program Using EWOL                            {#ewol_mainpage_using}
------------------

  - [**edn**](http://github.com/HeeroYui/edn): (Application in GPLv3) Edn is the main application using this lib and designed for (in the first time). This is a "Code editor".
  - [**worddown**](http://play.google.com/store/apps/details?id=com.edouarddupin.worddown): (Proprietary) Worddown is a simple word game.


Main documentation:                            {#ewol_mainpage_sub_doc}
===================

  - @ref ewol_build
  - @ref ewol_page_bases
  - @ref ewol_tutorials
  - @ref ewol_coding_style


FAQ                                {#page_FAQ}
===

What is ewol licence
--------------------

This is really simple : MPL v2.0 :

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




Why we use "DECLARE_FACTORY" Macro?
-----------------------------------

For some reason!!! But everything might be clear:
- In ewol we masively use ememory::SharedPtr.
- The main class : ewol::Object herited from ememory::EnableSharedFromThis<ewol::Object> to permit to access at his own ememory::SharedPtr.
- Acces At his own ememory::SharedPtr is not allowed in the class contructor/destructor.
- Many time for meta-widget we need to propagate our ememory::SharedPtr in child through the ememory::WeakPtr.

Then for all these reasons, I have create a simple MACRO that create a static template funtion that create the object and just after 
creation call the init(...) function to permit to create a complex widget or others with some writing convinience.




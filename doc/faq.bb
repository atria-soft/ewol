=?= FAQ =?=

== What is ewol licence ==

This is really simple : APACHE-2 :

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




== Why we use "DECLARE_FACTORY" Macro ? ==

For some reason!!! But everything might be clear:
:** In ewol we masively use std::shared_ptr<xxx> (I have create my own but it is not "standard" (I like when we use genecic system)).
:** The main class : [class[ewol::Object]] herited from [i]std::enable_shared_from_this<Object>[/i] to permit to access at his own [i]std::shared_ptr[/i].
:** Acces At his own [i]std::shared_ptr[/i] is not allowed in the class contructor/destructor.
:** Many time for meta-widget we need to propagate our [i]std::shared_ptr[/i] in child.

Then for all these reasons, I have create a simple MACRO that create a static template funtion that create the object and just after 
creation call the init(...) function to permit to create a complex widget or others with some writing convinience.







== Why we use "DECLARE_FACTORY" Macro ? ==

For some reason!!! But everything might be clear:
:** In ewol we masively use std::shared_ptr<xxx> (I have create my own but it is not "standard" (I like when we use genecic system)).
:** The main class : [class[ewol::Object]] herited from [i]std::enable_shared_from_this<Object>[/i] to permit to access at his own [i]std::shared_ptr[/i].
:** Acces At his own [i]std::shared_ptr[/i] is not allowed in the class contructor/destructor.
:** Many time for meta-widget we need to propagate our [i]std::shared_ptr[/i] in child.

Then for all these reasons, I have create a simple MACRO that create a static template funtion that create the object and just after 
creation call the init(...) function to permit to create a complex widget or others with some writing convinience.




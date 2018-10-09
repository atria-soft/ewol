EWOL: Resources management                                {#ewol_tutorial_resources}
==========================

@tableofcontents

Objectifs                                {#ewol_tutorial_resources_objectif}
=========

  - Understand what is a resource
  - Use resources

What is a resource:                                {#ewol_tutorial_resources_what}
===================

A resource is an unique element that can be used by many element like:
  - An image (special resolution)
  - A configuration file
  - An application manager (special case)
  - A sound file
  - ...

A resource have an other objective, form some platform, the graphic interface can be stopped, then we need to reload texture in the graphic inteface...
Then the texture is an other graphic interface.

Get a resource:                                {#ewol_tutorial_resources_get}
===============

For this example we will load a configuration file:
```{.cpp}
#include <ewol/object/Object.hpp>
#include <ewol/resource/ConfigFile.hpp>
namespace appl {
	class MyObj : public ewol::Object {
		public:
			eproperty::Value<etk::String> propertyConfig;
		private:
			ememory::SharedPtr<ewol::resource::ConfigFile> m_config;
			int32_t m_configValId;
		protected:
			//! @brief Constructor
			MyObj(void) :
			  propertyConfig(this, "file",
			                       "DATA:///ExapleConfig.json",
			                       "no desc",
			                       &appl::MyObj::onChangePropertyFile),
			  m_configValId(-1) {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
				onChangePropertyFile();
			}
		public:
			//! @brief Destructor
			virtual ~MyObj() { }
			DECLARE_FACTORY(MyObj);
		public:
			void onChangePropertyFile() {
				m_config = ewol::resource::ConfigFile::create(*propertyConfig);
				if (m_config != null) {
					m_configValId = m_config->request("exampleConfigName");
				}
			}
			void process() {
				double value = m_config->getNumber(m_configValId);
				APPL_DEBUG("example value : " << value);
			}
	}
}
```


Create a new resource:                                {#ewol_tutorial_resources_create}
======================

A resource is a generic gale::Resource:
  - DECLARE_RESOURCE_FACTORY(className) To declare a resource with no name (unique for every creation)
  - DECLARE_RESOURCE_NAMED_FACTORY(className) To create a resource that have a specific name. When created, we will find the previous resource with the specify name in the fanctory.
  - DECLARE_RESOURCE_SINGLE_FACTORY(className,uniqueName) This is to have a unique resource for all the application (name is specify in the Macro)

we have now some specific interface to compleate (if needed):

The Resource Level                                {#ewol_tutorial_resources_level}
------------------

The resources can be reloaded, then we need to reaload in the good order (level [0 .. 5])

The resources are loaded fron 0 to 5.

Then for basic resource:

```{.cpp}
#include <ewol/object/Resource.hpp>
namespace appl {
	class MyResource : public gale::Resource {
		protected:
			//! @brief Constructor
			MyResource() :
			  m_configValId(-1) {
				m_resourceLevel = 4;
				addObjectType("appl::MyResource");
			}
			void init(const etk::String& _name) {
				ewol::Resource::init(_name);
			}
		public:
			//! @brief Destructor
			virtual ~MyResource() { }
			DECLARE_RESOURCE_NAMED_FACTORY(MyResource);
	}
}
```

Now we need to implement somme functions:

To send data on the hardware (openGL):
```{.cpp}
	void updateContext();
```

To remove data from the the hardware (openGL):
```{.cpp}
	void removeContext();
```

When loose hardware (juste update internal state the hardware is no more present):
```{.cpp}
	void removeContextToLate();
```

When user request to reload all resources (can be usefull when using file type : THEME_GUI:///xxx)
```{.cpp}
	void reload();
```


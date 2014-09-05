
=== Objectifs ===
:** Understand What is a resource
:** Use resources

=== What is a resource: ===

A resource is an usique element that can be used by manny element like:
:** An image (special resolution)
:** A configuration file
:** An application manager (special case)
:** A sound file
:** ...

A resource have an other objective, form some platform, the graphic interface can be stopped, then we need to reload texture in the graphic inteface...
Then the texture is an other graphic interface.

=== Get a resource: ===

For this example we will load a configuration file:
[code style=c++]
#include <ewol/object/Object.h>
#include <ewol/resource/ConfigFile.h>
namespace appl {
	class MyObj : public ewol::Object {
		private:
			std::shared_ptr<ewol::resource::ConfigFile> m_config;
			int32_t m_configValId;
		protected:
			//! @brief Constructor
			MyObj(void) :
			  m_configValId(-1) {
				// nothing to do..
			}
			void init() {
				ewol::Object::init();
				m_config = ewol::resource::ConfigFile::create("DATA:ExapleConfig.json");
				m_configValId = m_config->request("exampleConfigName");
			}
		public:
			//! @brief Destructor
			virtual ~MyObj(void) { }
			DECLARE_FACTORY(MyObj);
		public:
			void process() {
				double value = m_config->getNumber(m_configValId);
				APPL_DEBUG("example value : " << value);
			}
	}
}
[/code]


=== Create a new resource: ===

A resource is a generic [class[ewol::Resource]] that hrited form a generic [class[ewol::Object]], simply change the FACTORY macro in :
:** DECLARE_RESOURCE_FACTORY(className) To declare a resource with no name (unique for every creation)
:** DECLARE_RESOURCE_NAMED_FACTORY(className) To create a resource that have a specific name. When created, we will find the previous resource with the specify name in the fanctory.
:** DECLARE_RESOURCE_SINGLE_FACTORY(className,uniqueName) This is to have a unique resource for all the application (name is specify in the Macro)

we have now some specific interface to compleate (if needed):

==== The Resource Level ====

The resources can be reloaded, then we need to reaload in the good order (level [0 .. 5])

The resources are loaded fron 0 to 5.

Then for basic resource :

[code style=c++]
#include <ewol/object/Resource.h>
namespace appl {
	class MyResource : public ewol::Resource {
		protected:
			//! @brief Constructor
			MyResource() :
			  m_configValId(-1) {
				m_resourceLevel = 4;
				addObjectType("ewol::MyResource");
			}
			void init(const std::& _name) {
				ewol::Resource::init(_name);
			}
		public:
			//! @brief Destructor
			virtual ~MyResource(void) { }
			DECLARE_RESOURCE_NAMED_FACTORY(MyResource);
	}
}
[/code]

Now we need to implement somme functions:

To send data on the hardware (openGL):
[code style=c++]
	void updateContext();
[/code]

To remove data from the the hardware (openGL):
[code style=c++]
	void removeContext();
[/code]

When loose hardware (juste update internal state):
[code style=c++]
	void removeContextToLate();
[/code]

When user request to reload all resources (can be usefull when using file type : THEME:GUI:xxx)
[code style=c++]
	void reload();
[/code]
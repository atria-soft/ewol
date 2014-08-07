/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <etk/types.h>
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/object/Object.h>

#define MAX_RESOURCE_LEVEL (5)

#define DECLARE_RESOURCE_FACTORY(className) \
	template<typename ... T> static std::shared_ptr<className> create( T&& ... all ) { \
		std::shared_ptr<className> object(new className()); \
		if (object == nullptr) { \
			EWOL_ERROR("Factory resource error"); \
			return nullptr; \
		} \
		object->init(std::forward<T>(all)... ); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		getManager().localAdd(object); \
		return object; \
	}

#define DECLARE_RESOURCE_NAMED_FACTORY(className) \
	template<typename ... T> static std::shared_ptr<className> create(const std::string& _name, T&& ... all ) { \
		std::shared_ptr<className> object; \
		std::shared_ptr<ewol::Resource> object2 = getManager().localKeep(_name); \
		if (object2 != nullptr) { \
			object = std::dynamic_pointer_cast<className>(object2); \
			if (object == nullptr) { \
				EWOL_CRITICAL("Request resource file : '" << _name << "' With the wrong type (dynamic cast error)"); \
				return nullptr; \
			} \
		} \
		if (object != nullptr) { \
			return object; \
		} \
		object = std::shared_ptr<className>(new className()); \
		if (object == nullptr) { \
			EWOL_ERROR("allocation error of a resource : " << _name); \
			return nullptr; \
		} \
		object->init(_name, std::forward<T>(all)... ); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		getManager().localAdd(object); \
		return object; \
	}

#define DECLARE_RESOURCE_SINGLE_FACTORY(className,uniqueName) \
	template<typename ... T> static std::shared_ptr<className> create(T&& ... all ) { \
		std::shared_ptr<className> object; \
		std::shared_ptr<ewol::Resource> object2 = getManager().localKeep(uniqueName); \
		if (object2 != nullptr) { \
			object = std::dynamic_pointer_cast<className>(object2); \
			if (object == nullptr) { \
				EWOL_CRITICAL("Request resource file : '" << uniqueName << "' With the wrong type (dynamic cast error)"); \
				return nullptr; \
			} \
		} \
		if (object != nullptr) { \
			return object; \
		} \
		object = std::shared_ptr<className>(new className()); \
		if (object == nullptr) { \
			EWOL_ERROR("allocation error of a resource : " << uniqueName); \
			return nullptr; \
		} \
		object->init(uniqueName, std::forward<T>(all)... ); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		getManager().localAdd(object); \
		return object; \
	}

namespace ewol {
	namespace resource {
		class Manager;
	};
	// class resources is pure virtual
	class Resource : public ewol::Object {
		protected:
			Resource() :
			  m_resourceLevel(MAX_RESOURCE_LEVEL-1) {
				addObjectType("ewol::Resource");
				setStatusResource(true);
			};
			void init();
			void init(const std::string& _name);
		public:
			
			virtual ~Resource() {
				
			};
		protected:
			uint8_t m_resourceLevel; //!< Level of the resource ==> for updata priority [0..5] 0 must be update first.
		public:
			uint8_t  getResourceLevel() {
				return m_resourceLevel;
			};
			virtual void updateContext();
			virtual void removeContext();
			virtual void removeContextToLate();
			virtual void reload();
			static ewol::resource::Manager& getManager();
	};
};

#include <ewol/resource/Manager.h>

#endif


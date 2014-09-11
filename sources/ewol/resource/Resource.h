/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
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
	/**
	 * @brief A Resource is a generic interface to have an instance that have things that can be used by many people, ad have some hardware dependency.
	 * For example of resources :
	 * :** Shaders: openGL display interface.
	 * :** Texture: openGL imega interface.
	 * :** Font: Single file interface to store many glyphe ==> reduce the number of parallele loaded files.
	 * :** ConfigFile: simple widget configuration files
	 * :** ...
	 */
	class Resource : public ewol::Object {
		protected:
			/**
			 * @brief generic protected contructor (use factory to create this class)
			 */
			Resource() :
			  m_resourceLevel(MAX_RESOURCE_LEVEL-1) {
				addObjectType("ewol::Resource");
				setStatusResource(true);
			};
			/**
			 * @brief Initialisation of the class and previous classes.
			 * @param[in] _name Name of the resource.
			 */
			void init();
			//! @previous
			void init(const std::string& _name);
		public:
			//! geenric destructor
			virtual ~Resource() {
				
			};
		protected:
			uint8_t m_resourceLevel; //!< Level of the resource ==> for update priority [0..5] 0 must be update first.
		public:
			/**
			 * @brief Get the current resource level;
			 * @return value in [0..5]
			 */
			uint8_t getResourceLevel() {
				return m_resourceLevel;
			};
			/**
			 * @brief Call when need to send data on the harware (openGL)
			 * @note This is done asynchronously with the create of the Resource.
			 */
			virtual void updateContext();
			/**
			 * @brief The current OpenGl context is removing ==> remove yout own system data
			 */
			virtual void removeContext();
			/**
			 * @brief The notification of the Context removing is too late, we have no more acces on the OpenGl context (thank you Android).
			 * Juste update your internal state
			 */
			virtual void removeContextToLate();
			/**
			 * @brief User request the reload of all resources (usefull when the file depend on DATA:GUI:xxx ...
			 */
			virtual void reload();
		protected:
			/**
			 * @brief Get the current resource Manager
			 */
			static ewol::resource::Manager& getManager();
	};
};

#include <ewol/resource/Manager.h>

#endif


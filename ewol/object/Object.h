/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_OBJECT_H__
#define __EWOL_OBJECT_H__

#include <etk/types.h>
#include <vector>
#include <exml/exml.h>
#include <mutex>
#include <memory>

#include <ewol/debug.h>
#include <ewol/parameter/Interface.h>
#include <ewol/parameter/Value.h>
#include <ewol/parameter/Range.h>
#include <ewol/parameter/List.h>
#include <ewol/signal/Interface.h>

namespace ewol {
	// some class need to define element befor other ...
	class Object;
	namespace object {
		class Manager;
	};
	class Context;
};

#define DECLARE_FACTORY(className) \
	template<typename ... T> static std::shared_ptr<className> create( T&& ... all ) { \
		std::shared_ptr<className> object(new className()); \
		if (object == nullptr) { \
			EWOL_ERROR("Factory error"); \
			return nullptr; \
		} \
		object->init(std::forward<T>(all)... ); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		return object; \
	}

#define DECLARE_SINGLE_FACTORY(className,uniqueName) \
	template<typename ... T> static std::shared_ptr<className> create( T&& ... all ) { \
		std::shared_ptr<className> object; \
		std::shared_ptr<ewol::Object> object2 = getObjectNamed(uniqueName); \
		if (object2 != nullptr) { \
			object = std::dynamic_pointer_cast<className>(object2); \
			if (object == nullptr) { \
				GALE_CRITICAL("Request object element: '" << uniqueName << "' With the wrong type (dynamic cast error)"); \
				return nullptr; \
			} \
		} \
		if (object != nullptr) { \
			return object; \
		} \
		object = std::shared_ptr<className>(new className()); \
		if (object == nullptr) { \
			EWOL_ERROR("Factory error"); \
			return nullptr; \
		} \
		object->init(uniqueName, std::forward<T>(all)... ); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object is not correctly init : " << #className ); \
		} \
		return object; \
	}

namespace ewol {
	/**
	 * @brief Basic message classes for ewol system
	 * this class mermit at every Object to communicate between them.
	 */
	class Object : public std::enable_shared_from_this<Object>,
	               public ewol::parameter::Interface,
	               public ewol::signal::Interface {
		private:
			static size_t m_valUID; //!< Static used for the unique ID definition
		private:
			bool m_objectHasBeenInit; //!< Know if the init function has bben called
		protected:
			/**
			 * @brief Constructor.
			 */
			Object();
			void init();
			//! @previous
			void init(const std::string& _name);
		public:
			/**
			 * @brief Factory
			 */
			DECLARE_FACTORY(Object);
			/**
			 * @brief Destructor
			 */
			virtual ~Object();
			bool objectHasBeenCorectlyInit() {
				return m_objectHasBeenInit;
			}
		protected:
			std::weak_ptr<Object> m_parent;
			bool m_destroy;
		protected:
			/**
			 * @brief Auto-destroy the object
			 */
			void autoDestroy();
		public:
			virtual void destroy() {
				autoDestroy();
			}
			bool isDestroyed() const {
				return m_destroy;
			}
		public:
			virtual void requestDestroyFromChild(const std::shared_ptr<Object>& _child);
			virtual void setParent(const std::shared_ptr<Object>& _newParent);
			virtual void removeParent();
		private:
			std::vector<const char*> m_listType;
		public:
			/**
			 * @brief get the current Object type of the Object
			 * @return the last type name of the element
			 */
			const char * const getObjectType();
			/**
			 * @brief Get the herarchie of the Object type.
			 * @return descriptive string.
			 */
			std::string getTypeDescription();
			/**
			 * @brief check  if the element herited from a specific type
			 * @param[in] _type Type to check.
			 * @return true if the element is compatible.
			 */
			bool isTypeCompatible(const std::string& _type);
		protected:
			/**
			 * @brief Add a type of the list of Object.
			 * @param[in] _type new type to add.
			 */
			void addObjectType(const char* _type);
		protected:
			bool m_static; //!< set this variable at true if this element must not be auto destroy (exemple : use static object)
		public:
			/**
			 * @brief get the static status of the Object  == > mark at true if the user set the object mark as static allocated element ==> not auto remove element
			 * @return true if it might not be removed  == > usefull for conficuration class
			 */
			bool getStatic(){
				return m_static;
			};
		private:
			int32_t m_uniqueId; //!< Object UniqueID  == > TODO : Check if it use is needed
		public:
			/**
			 * @brief get the UniqueId of the Object
			 * @return the requested ID
			 */
			int32_t getId(){
				return m_uniqueId;
			};
		public:
			// TODO : Rework the position on this function ... This is a convignent function ...
			bool parameterSetOnWidgetNamed(const std::string& _objectName, const std::string& _config, const std::string& _value);
		protected:
			ewol::parameter::Value<std::string> m_name; //!< name of the element ...
		public:
			/**
			 * @brief get the Object name
			 * @return The requested name
			 */
			const std::string& getName() const {
				return m_name.get();
			};
			/**
			 * @brief get the Widget name
			 * @param[in] _name The new name
			 */
			void setName(const std::string& _name) {
				m_name = _name;
			};
		public:
			/**
			 * @brief load properties with an XML node.
			 * @param[in] _node Pointer on the tinyXML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
			/**
			 * @brief store properties in this XML node.
			 * @param[in,out] _node Pointer on the tinyXML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool storeXML(const std::shared_ptr<exml::Element>& _node) const;
		public:
			/**
			 * @breif get the current Object manager.
			 * @return the requested object manager.
			 */
			static ewol::object::Manager& getObjectManager();
			/**
			 * @brief get the curent the system inteface.
			 * @return current reference on the instance.
			 */
			static ewol::Context& getContext();
		private:
			bool m_isResource; //!< enable this when you want to declare this element is auto-remove
		public:
			/**
			 * @brief Declare this element as a resource (or singleton) this mean the element will 
			 * not be auto Remove at the end of the programm. It just notify that it is not removed.
			 * @param[in] _val Value of the type of the element.
			 */
			void setStatusResource(bool _val) {
				m_isResource = _val;
			}
			/**
			 * @brief Get the resource status of the element.
			 * @return the resource status.
			 */
			bool getStatusResource() const {
				return m_isResource;
			}
			/**
			 * @brief Retrive an object with his name (in the global list)
			 * @param[in] _name Name of the object
			 * @return the requested object or nullptr
			 */
			static std::shared_ptr<ewol::Object> getObjectNamed(const std::string& _objectName);
			/**
			 * @brief Retrive an object with his name (in the global list)
			 * @param[in] _name Name of the object
			 * @return the requested object or nullptr
			 */
			virtual std::shared_ptr<ewol::Object> getSubObjectNamed(const std::string& _objectName);
		protected:
			// TODO : Create a template ...
			/**
			 * @brief link on an signal in the subwiget with his name
			 */
			#define subBind(_type, _name, _event, _shared_ptr, _func, ...) do {\
				std::shared_ptr<_type> myObject = std::dynamic_pointer_cast<_type>(getSubObjectNamed(_name)); \
				if (myObject != nullptr) { \
					myObject->_event.bind(_shared_ptr, _func, ##__VA_ARGS__); \
				} else { \
					EWOL_ERROR("object named='" << _name << "' not exit or can not be cast in : " << #_type); \
				} \
			} while (false)
			/*
			template<class TYPE> void subBind(std::shared_ptr<ewol::Object> _obj, void (TYPE::*_func)()) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
				if (obj2 == nullptr) {
					EWOL_ERROR("Can not bind signal ...");
					return;
				}
				m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), std::bind(_func, obj2.get())));
			}
			*/
	};
	bool parameterSetOnObjectNamed(const std::string& _objectName, const std::string& _config, const std::string& _value);
};

/**
 * @brief link on an signal in the global object list with his name
 */
#define globalBind(_type, _name, _event, _obj, _func, ...) do {\
	std::shared_ptr<_type> myObject = std::dynamic_pointer_cast<_type>(ewol::getContext().getEObjectManager().getObjectNamed(_name)); \
	if (myObject != nullptr) { \
		myObject->_event.bind(_obj, _func, ##__VA_ARGS__); \
	} else { \
		EWOL_ERROR("object named='" << _name << "' not exit or can not be cast in : " << #_type); \
	} \
} while (false)

/**
 * @brief link on an signal in the subWidget of an object with his name
 */
#define externSubBind(_object, _type, _name, _event, _obj, _func, ...) do {\
	std::shared_ptr<_type> myObject = std::dynamic_pointer_cast<_type>(_object->getObjectNamed(_name)); \
	if (myObject != nullptr) { \
		myObject->_event.bind(_obj, _func, ##__VA_ARGS__); \
	} else { \
		EWOL_ERROR("object named='" << _name << "' not exit or can not be cast in : " << #_type); \
	} \
} while (false)
//#include <ewol/signal/Signal.h>

#endif




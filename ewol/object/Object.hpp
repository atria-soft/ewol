/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Vector.hpp>
#include <exml/exml.hpp>
#include <ethread/Mutex.hpp>
#include <ememory/memory.hpp>

#include <ewol/debug.hpp>
#include <ememory/memory.hpp>
#include <eproperty/Interface.hpp>
#include <eproperty/Value.hpp>
#include <eproperty/Range.hpp>
#include <eproperty/List.hpp>
#include <esignal/Interface.hpp>


namespace ewol {
	// some class need to define element befor other ...
	class Object;
	namespace object {
		class Manager;
	}
	class Context;
}

template<class TYPE_OBJECT> static void baseInit(const ememory::SharedPtr<TYPE_OBJECT>& _object) {
	// end of recurtion
	return;
}

template<class TYPE_OBJECT, class TYPE_VAL, class ... TYPE> static void baseInit(const ememory::SharedPtr<TYPE_OBJECT>& _object, const etk::String& _name, const TYPE_VAL& _val, TYPE&& ... _all ) {
	eproperty::Property* prop(null);
	eproperty::PropertyType<TYPE_VAL>* propType(null);
	if (_object == null) {
		EWOL_ERROR("EMPTY pointer");
		return;
	}
	prop = _object->properties.getRaw(_name);
	if (prop == null) {
		EWOL_ERROR("property does not exit ... '" << _name << "'");
		goto exit_on_error;
	}
	propType = dynamic_cast<eproperty::PropertyType<TYPE_VAL>*>(prop);
	if (propType == null) {
		EWOL_ERROR("property does not cast in requested type ... '" << _name << "' require type : " << /*typeid(_val).name()*/ "?TODO?" << "' instead of '" << prop->getType() << "'");
		goto exit_on_error;
	}
	propType->setDirectCheck(_val);
exit_on_error:
	baseInit(_object, etk::forward<TYPE>(_all)... );
	return;
}

#define UN_DECLARE_FACTORY(className) \
	template<class ... EWOL_FACTORY_CREATE_TYPE> static ememory::SharedPtr<className> create(const EWOL_FACTORY_CREATE_TYPE& ... _all) = delete;

#define DECLARE_FACTORY(className) \
	template<class ... EWOL_FACTORY_CREATE_TYPE> static ememory::SharedPtr<className> create(const EWOL_FACTORY_CREATE_TYPE& ... _all) { \
		ememory::SharedPtr<className> object(ETK_NEW(className)); \
		if (object == null) { \
			EWOL_ERROR("Factory error"); \
			return null; \
		} \
		baseInit(object, _all... ); \
		object->init(); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		return object; \
	} \
	static ememory::SharedPtr<className> createXml(const exml::Element& _node) { \
		ememory::SharedPtr<className> object(ETK_NEW(className)); \
		if (object == null) { \
			EWOL_ERROR("Factory error"); \
			return null; \
		} \
		object->loadXMLAttributes(_node); \
		object->init(); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		return object; \
	}

#define DECLARE_SINGLE_FACTORY(className, uniqueName) \
	template<class ... EWOL_FACTORY_CREATE_TYPE> static ememory::SharedPtr<className> create(const EWOL_FACTORY_CREATE_TYPE& ... _all) { \
		ememory::SharedPtr<className> object; \
		ememory::SharedPtr<ewol::Object> object2 = getObjectNamed(uniqueName); \
		if (object2 != null) { \
			object = ememory::dynamicPointerCast<className>(object2); \
			if (object == null) { \
				EWOL_CRITICAL("Request object element: '" << uniqueName << "' With the wrong type (dynamic cast error)"); \
				return null; \
			} \
		} \
		if (object != null) { \
			return object; \
		} \
		object = ememory::SharedPtr<className>(ETK_NEW(className)); \
		if (object == null) { \
			EWOL_ERROR("Factory error"); \
			return null; \
		} \
		baseInit(object, "name", etk::String(uniqueName), _all... ); \
		object->init(); \
		if (object->objectHasBeenCorectlyInit() == false) { \
			EWOL_CRITICAL("Object Is not correctly init : " << #className ); \
		} \
		return object; \
	}

namespace ewol {
	using ObjectShared = ememory::SharedPtr<ewol::Object>;
	using ObjectWeak = ememory::WeakPtr<ewol::Object>;
	/**
	 * @brief Basic message classes for ewol system
	 * this class mermit at every Object to communicate between them.
	 */
	class Object : public ememory::EnableSharedFromThis<Object>,
	               public eproperty::Interface,
	               public esignal::Interface {
		public: // Event list
			
		public: // propertie list
			eproperty::Value<etk::String> propertyName; //!< name of the element ...
		private:
			static size_t m_valUID; //!< Static used for the unique ID definition
		private:
			bool m_objectHasBeenInit; //!< Know if the init function has bben called
		public:
			/**
			 * @brief Destructor
			 */
			virtual ~Object();
		protected:
			/**
			 * @brief Constructor.
			 */
			Object();
			virtual void init();
		public:
			/**
			 * @brief Factory
			 */
			DECLARE_FACTORY(Object);
			bool objectHasBeenCorectlyInit();
		protected:
			ewol::ObjectWeak m_parent; //!< Reference on the current parrent.
			bool m_destroy; //!< Flag to know if the object is requesting has destroy.
		protected:
			/**
			 * @brief Auto-destroy the object
			 */
			virtual void autoDestroy();
		public:
			/**
			 * @brief Destroy the current object
			 */
			virtual void destroy();
			/**
			 * @brief Check if the current objetc his destroy (in removing)
			 * @return true The object is removed
			 * @return false The object is not removed
			 */
			bool isDestroyed() const;
		protected:
			/**
			 * @brief Called by a whild that want to remove pointer of itself from the current list of his parrent
			 * @param[in] _child Object of the child that want to remove itself
			 */
			virtual void requestDestroyFromChild(const ewol::ObjectShared& _child);
		public:
			/**
			 * @brief Set the Object has new parrent.
			 * @param[in] _newParent Object that requesting the parenting
			 */
			virtual void setParent(const ewol::ObjectShared& _newParent);
			/**
			 * @brief Remove the current parenting.
			 */
			virtual void removeParent();
		private:
			etk::Vector<const char*> m_listType;
		public:
			/**
			 * @brief get the current Object type of the Object
			 * @return the last type name of the element
			 */
			const char * const getObjectType() const;
			/**
			 * @brief Get the herarchie of the Object type.
			 * @return descriptive string.
			 */
			etk::String getTypeDescription() const;
			/**
			 * @brief check  if the element herited from a specific type
			 * @param[in] _type Type to check.
			 * @return true if the element is compatible.
			 */
			bool isTypeCompatible(const etk::String& _type) const;
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
			bool propertySetOnWidgetNamed(const etk::String& _objectName, const etk::String& _config, const etk::String& _value);
		public:
			/**
			 * @brief load attribute properties with an XML node.
			 * @param[in] _node Reference on the XML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			bool loadXMLAttributes(const exml::Element& _node);
			/**
			 * @brief load properties with an XML node.
			 * @param[in] _node Reference on the XML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool loadXML(const exml::Element& _node);
			/**
			 * @brief store properties in this XML node.
			 * @param[in,out] _node Reference on the XML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool storeXML(exml::Element& _node) const;
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
			 * @return the requested object or null
			 */
			static ewol::ObjectShared getObjectNamed(const etk::String& _objectName);
			/**
			 * @brief Retrive an object with his name (in the global list)
			 * @param[in] _name Name of the object
			 * @return the requested object or null
			 */
			virtual ewol::ObjectShared getSubObjectNamed(const etk::String& _objectName);
		protected:
			// TODO : Create a template ...
			/**
			 * @brief link on an signal in the subwiget with his name
			 */
			#define subBind(_type, _name, _event, _shared_ptr, _func, ...) do {\
				ememory::SharedPtr<_type> myObject = ememory::dynamicPointerCast<_type>(getSubObjectNamed(_name)); \
				if (myObject != null) { \
					myObject->_event.connect(_shared_ptr, _func, ##__VA_ARGS__); \
				} else { \
					EWOL_ERROR("object named='" << _name << "' not exit or can not be cast in : " << #_type); \
				} \
			} while (false)
	};
	bool propertySetOnObjectNamed(const etk::String& _objectName, const etk::String& _config, const etk::String& _value);
};

/**
 * @brief link on an signal in the global object list with his name
 */
#define globalBind(_type, _name, _event, _obj, _func, ...) do {\
	ememory::SharedPtr<_type> myObject = ememory::dynamicPointerCast<_type>(ewol::getContext().getEObjectManager().getObjectNamed(_name)); \
	if (myObject != null) { \
		myObject->_event.connect(_obj, _func, ##__VA_ARGS__); \
	} else { \
		EWOL_ERROR("object named='" << _name << "' not exit or can not be cast in : " << #_type); \
	} \
} while (false)

/**
 * @brief link on an signal in the subWidget of an object with his name
 */
#define externSubBind(_object, _type, _name, _event, _obj, _func, ...) do {\
	ememory::SharedPtr<_type> myObject = ememory::dynamicPointerCast<_type>(_object->getObjectNamed(_name)); \
	if (myObject != null) { \
		myObject->_event.connect(_obj, _func, ##__VA_ARGS__); \
	} else { \
		EWOL_ERROR("object named='" << _name << "' not exit or can not be cast in : " << #_type); \
	} \
} while (false)


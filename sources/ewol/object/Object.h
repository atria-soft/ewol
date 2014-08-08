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
#include <etk/types.h>
#include <vector>
#include <exml/exml.h>
#include <mutex>
#include <memory>

namespace ewol {
	// some class need to define element befor other ...
	class Object;
	namespace object {
		class Manager;
		class MultiCast;
	};
	class Context;
};

#include <ewol/object/Config.h>
#include <ewol/object/ConfigElement.h>
#include <ewol/object/Message.h>

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

namespace ewol {
	namespace object {
		/**
		 * local class for event generation
		 * @not-in-doc
		 */
		class EventExtGen {
			public:
				const char* localEventId; //!< local event Id generation
				std::weak_ptr<ewol::Object> destObject; //!< destination widget that might be call
				const char* destEventId; //!< generated event ID on the distant widget
				std::string overloadData; //!< sometimes the user prefer to receive some specific data on an event (instead of the one sed by the widget)
		};
	}
	/**
	 * @brief Basic message classes for ewol system
	 * this class mermit at every Object to communicate between them.
	 */
	class Object : public std::enable_shared_from_this<Object> {
		private:
			static size_t m_valUID; //!< stic used for the unique ID definition
		public:
			// Config list of properties
			static const char* const configName;
		private:
			bool m_objectHasBeenInit; //!< Know if the init function has bben called
		protected:
			/**
			 * @brief Constructor.
			 */
			Object();
			void init();
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
		protected:
			/**
			 * @brief Auto-destroy the object
			 */
			void autoDestroy();
		public:
			void destroy() {
				autoDestroy();
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
		private:
			std::vector<object::EventExtGen> m_externEvent; //!< Generic list of event generation for output link
			std::vector<const char*> m_availlableEventId; //!< List of all event availlable for this widget
		protected:
			/**
			 * @brief add a specific event Id in the list to prevent wrong link on a Object
			 * @param[in] _generateEventId event Id to add
			 */
			void addEventId(const char * _generateEventId);
			/**
			 * @brief generate event on all registered Object
			 * @param[in] _generateEventId event Id that is curetly generated
			 * @param[in] _data data associated with the event
			 */
			void generateEventId(const char * _generateEventId, const std::string& _data = "");
			/**
			 * @brief generate Multicast event on all Object requested the event
			 * @param[in] _messageId Event Id that is generated
			 * @param[in] _data String that is send at all the destinations
			 */
			void sendMultiCast(const char* const _messageId, const std::string& _data = "");
			/**
			 * @brief Register of the arrival of a Multicast message
			 * @param[in] _messageId Event Id waiting for...
			 */
			void registerMultiCast(const char* const _messageId);
		public:
			/**
			 * @brief Register an Object over an other to get event on the second...
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _eventId Event generate inside the object (note : "*" event register on all event generated )
			 * @param[in] _eventIdgenerated event generated when call the distant Object.onReceiveMessage(...)
			 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
			 */
			void registerOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
			                     const char * _eventId,
			                     const char * _eventIdgenerated = nullptr,
			                     const std::string& _overloadData = "");
			/**
			 * @brief Un-Register an Object over an other.
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _eventId Event generate inside the object (nullptr to remove all event on this object)
			 */
			void unRegisterOnEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
			                       const char * _eventId = nullptr);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @note : Sub classes must call this class
			 */
			virtual void onObjectRemove(const std::shared_ptr<ewol::Object>& _object);
			/**
			 * @brief Receive a message from an other Object with a specific eventId and data
			 * @param[in] _msg Message handle
			 */
			virtual void onReceiveMessage(const ewol::object::Message& _msg) {
				
			};
		private:
			std::vector<ewol::object::ConfigElement> m_listConfig;
		protected:
			/**
			 * @brief the Object add a configuration capabilities
			 * @param[in] _config Configuration name.
			 * @param[in] _type Type of the config.
			 * @param[in] _control control of the current type.
			 * @param[in] _description Descritpion on the current type.
			 * @param[in] _default Default value of this parameter.
			 */
			void registerConfig(const char* _config,
			                    const char* _type = nullptr,
			                    const char* _control = nullptr,
			                    const char* _description = nullptr,
			                    const char* _default = nullptr);
			/**
			 * @brief Configuration requested to the curent Object
			 * @param[in] _conf Configuration handle.
			 * @return true if the parametere has been used
			 */
			virtual bool onSetConfig(const ewol::object::Config& _conf);
			/**
			 * @brief Receive a configuration message from an other element system or from the curent Object
			 * @param[in] _config Configuration name.
			 * @param[out] _result Result of the request.
			 * @return true if the config is set
			 */
			virtual bool onGetConfig(const char* _config, std::string& _result) const ;
		public:
			/** 
			 * @brief get all the configuration list
			 * @return The list of all parameter availlable in the widget
			 */
			virtual const std::vector<ewol::object::ConfigElement>& getConfigList() {
				return m_listConfig;
			};
			/**
			 * @brief Configuration requested to the curent Object (systrem mode)
			 * @param[in] _conf Configuration handle.
			 * @return true if config set correctly...
			 */
			bool setConfig(const ewol::object::Config& _conf) {
				return onSetConfig(_conf);
			};
			bool setConfig(const std::string& _config, const std::string& _value); // need a search ...
			// TODO : Distingish global search and sub search ...
			bool setConfigNamed(const std::string& _objectName, const std::string& _config, const std::string& _value); // need a search ...
			bool setConfigNamed(const std::string& _objectName, const ewol::object::Config& _conf);
			/**
			 * @brief Configuration get from the curent Object (systrem mode)
			 * @param[in] _config Configuration name.
			 * @return the config properties
			 */
			std::string getConfig(const char* _config) const;
			std::string getConfig(const std::string& _config) const; // need search
		protected:
			std::string m_name; //!< name of the element ...
		public:
			/**
			 * @brief get the Object name
			 * @return The requested name
			 */
			const std::string& getName() const {
				return m_name;
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
			virtual bool loadXML(exml::Element* _node);
			/**
			 * @brief store properties in this XML node.
			 * @param[in,out] _node Pointer on the tinyXML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool storeXML(exml::Element* _node) const;
		public:
			/**
			 * @breif get the current Object manager.
			 * @return the requested object manager.
			 */
			ewol::object::Manager& getObjectManager() const;
			/**
			 * @breif get the current Object Message Multicast manager.
			 * @return the requested object manager.
			 */
			ewol::object::MultiCast& getMultiCast() const;
			/**
			 * @brief get the curent the system inteface.
			 * @return current reference on the instance.
			 */
			ewol::Context& getContext() const;
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
			 * @brief Register an Event an named widget. @see registerOnEvent
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _objectName Name of the object.
			 * @param[in] _eventId Event generate inside the object.
			 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
			 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
			 * @note : To used when NOT herited from this object.
			 */
			void registerOnObjectEvent(const std::shared_ptr<ewol::Object>& _destinationObject,
			                           const std::string& _objectName,
			                           const char * _eventId,
			                           const char * _eventIdgenerated = nullptr,
			                           const std::string& _overloadData="");
			/**
			 * @brief Retrive an object with his name (in the global list)
			 * @param[in] _name Name of the object
			 * @return the requested object or nullptr
			 */
			std::shared_ptr<ewol::Object> getObjectNamed(const std::string& _objectName) const;
	};
	
};

#endif




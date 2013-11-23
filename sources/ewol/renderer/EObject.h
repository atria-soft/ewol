/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_E_OBJECT_H__
#define __EWOL_E_OBJECT_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <vector>
#include <exml/exml.h>
#include <vector>
namespace ewol {
	// some class need to define element befor other ...
	class EObject;
	class EObjectManager;
	class EMultiCast;
	class eContext;
};

#include <ewol/renderer/EConfig.h>
#include <ewol/renderer/EMessage.h>

namespace ewol {
	/**
	 * local class for event generation
	 */
	class EventExtGen {
		public:
			const char* localEventId; //!< local event Id generation
			ewol::EObject* destEObject; //!< destination widget that might be call
			const char* destEventId; //!< generated event ID on the distant widget
			std::string overloadData; //!< sometimes the user prefer to receive some specific data on an event (instead of the one sed by the widget)
	};
	/**
	 * @brief Basic message classes for ewol system
	 * this class mermit at every EObject to communicate between them.
	 */
	class EObject {
		private:
			static size_t m_valUID; //!< stic used for the unique ID definition
		public:
			// Config list of properties
			static const char* const configName;
		public:
			/**
			 * @brief Constructor.
			 */
			EObject(void);
			/**
			 * @brief Constructor.
			 * @param[in] _name Name of the EObject.
			 */
			EObject(const std::string& _name);
			/**
			 * @brief Destructor
			 */
			virtual ~EObject(void);
		protected:
			/**
			 * @brief Auto-destroy the object
			 */
			void autoDestroy(void);
		public:
			/**
			 * @brief Asynchronous removing the object
			 */
			void removeObject(void);
		private:
			std::vector<const char*> m_listType;
		public:
			/**
			 * @brief get the current Object type of the EObject
			 * @return the last type name of the element
			 */
			const char * const getObjectType(void);
			/**
			 * @brief Get the herarchie of the EObject type.
			 * @return descriptive string.
			 */
			std::string getTypeDescription(void);
			/**
			 * @brief check  if the element herited from a specific type
			 * @param[in] _type Type to check.
			 * @return true if the element is compatible.
			 */
			bool isTypeCompatible(const std::string& _type);
		protected:
			/**
			 * @brief Add a type of the list of eObject.
			 * @param[in] _type new type to add.
			 */
			void addObjectType(const char* _type);
		protected:
			bool m_static; //!< set this variable at true if this element must not be auto destroy (exemple : use static object)
		public:
			/**
			 * @brief get the static status of the EObject  == > mark at true if the user set the object mark as static allocated element ==> not auto remove element
			 * @return true if it might not be removed  == > usefull for conficuration class
			 */
			bool getStatic(void){
				return m_static;
			};
		private:
			int32_t m_uniqueId; //!< Object UniqueID  == > TODO : Check if it use is needed
		public:
			/**
			 * @brief get the UniqueId of the EObject
			 * @return the requested ID
			 */
			int32_t getId(void){
				return m_uniqueId;
			};
		private:
			std::vector<EventExtGen*> m_externEvent; //!< Generic list of event generation for output link
			std::vector<const char*> m_availlableEventId; //!< List of all event availlable for this widget
		protected:
			/**
			 * @brief add a specific event Id in the list to prevent wrong link on a EObject
			 * @param[in] _generateEventId event Id to add
			 */
			void addEventId(const char * _generateEventId);
			/**
			 * @brief generate event on all registered EObject
			 * @param[in] _generateEventId event Id that is curetly generated
			 * @param[in] _data data associated with the event
			 */
			void generateEventId(const char * _generateEventId, const std::string& _data = "");
			/**
			 * @brief generate Multicast event on all EObject requested the event
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
			 * @brief Register an EObject over an other to get event on the second...
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _eventId Event generate inside the object
			 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
			 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
			 */
			void registerOnEvent(ewol::EObject * _destinationObject,
			                     const char * _eventId,
			                     const char * _eventIdgenerated = NULL,
			                     const std::string& _overloadData = "");
			/**
			 * @brief Un-Register an EObject over an other.
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _eventId Event generate inside the object (NULL to remove all event on this object)
			 */
			void unRegisterOnEvent(ewol::EObject * _destinationObject,
			                       const char * _eventId = NULL);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] _removeObject Pointer on the EObject remeved  == > the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 */
			virtual void onObjectRemove(ewol::EObject * _removeObject);
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] _msg Message handle
			 */
			virtual void onReceiveMessage(const ewol::EMessage& _msg) {
				
			};
		private:
			std::vector<ewol::EConfigElement> m_listConfig;
		protected:
			/**
			 * @brief the EObject add a configuration capabilities
			 * @param[in] _config Configuration name.
			 * @param[in] _type Type of the config.
			 * @param[in] _control control of the current type.
			 * @param[in] _description Descritpion on the current type.
			 * @param[in] _default Default value of this parameter.
			 */
			void registerConfig(const char* _config,
			                    const char* _type = NULL,
			                    const char* _control = NULL,
			                    const char* _description = NULL,
			                    const char* _default = NULL);
			/**
			 * @brief Configuration requested to the curent EObject
			 * @param[in] _conf Configuration handle.
			 * @return true if the parametere has been used
			 */
			virtual bool onSetConfig(const ewol::EConfig& _conf);
			/**
			 * @brief Receive a configuration message from an other element system or from the curent EObject
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
			virtual const std::vector<ewol::EConfigElement>& getConfigList(void) {
				return m_listConfig;
			};
			/**
			 * @brief Configuration requested to the curent EObject (systrem mode)
			 * @param[in] _conf Configuration handle.
			 * @return true if config set correctly...
			 */
			bool setConfig(const ewol::EConfig& _conf) {
				return onSetConfig(_conf);
			};
			bool setConfig(const std::string& _config, const std::string& _value); // need a search ...
			// TODO : Distingish global search and sub search ...
			bool setConfigNamed(const std::string& _objectName, const std::string& _config, const std::string& _value); // need a search ...
			bool setConfigNamed(const std::string& _objectName, const ewol::EConfig& _conf);
			/**
			 * @brief Configuration get from the curent EObject (systrem mode)
			 * @param[in] _config Configuration name.
			 * @return the config properties
			 */
			std::string getConfig(const char* _config) const;
			std::string getConfig(const std::string& _config) const; // need search
		protected:
			std::string m_name; //!< name of the element ...
		public:
			/**
			 * @brief get the eObject name
			 * @return The requested name
			 */
			const std::string& getName(void) const {
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
			 * @breif get the current EObject manager.
			 * @return the requested object manager.
			 */
			ewol::EObjectManager& getEObjectManager(void);
			/**
			 * @breif get the current EObject Message Multicast manager.
			 * @return the requested object manager.
			 */
			ewol::EMultiCast& getMultiCast(void);
			/**
			 * @brief get the curent the system inteface.
			 * @return current reference on the instance.
			 */
			eContext& getContext(void);
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
			bool getStatusResource(void) {
				return m_isResource;
			}
	};
};

#endif




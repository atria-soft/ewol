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
#include <etk/Vector.h>
#include <exml/exml.h>
namespace ewol {
	// some class need to define element befor other ...
	class EObject;
	class EObjectManager;
	class EObjectMessageMultiCast;
	class eSystem;
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
			const char* destEventId; //!< Generated event ID on the distant widget
			etk::UString overloadData; //!< sometimes the user prefer to receive some specific data on an event (instead of the one sed by the widget)
	};
	
	/**
	 * @brief Basic message classes for ewol system
	 * 
	 * this class mermit at every EObject to communicate between them.
	 */
	class EObject {
		public:
			// Config list of properties
			static const char* const configName;
		protected:
			bool m_static; //!< set this variable at true if this element must not be auto destroy (exemple : use static object)
		private:
			int32_t m_uniqueId; //!< Object UniqueID ==> TODO : Check if it use is needed
			etk::Vector<EventExtGen*> m_externEvent; //!< Generic list of event generation for output link
			etk::Vector<const char*> m_availlableEventId; //!< List of all event availlable for this widget
		public:
			/**
			 * @brief Constructor
			 */
			EObject(void);
			
			/**
			 * @brief Destructor
			 */
			virtual ~EObject(void);
			
			/**
			 * @brief Get the static status of the EObject ==> mark at true if the user set the object mark as static allocated element ==> not auto remove element
			 * @return true if it might not be removed ==> usefull for conficuration class
			 */
			bool GetStatic(void){ return m_static; };
			
			/**
			 * @brief Get the UniqueId of the EObject
			 * @return the requested ID
			 */
			int32_t GetId(void){ return m_uniqueId; };
			
			/**
			 * @brief Auto-destroy the object
			 */
			void AutoDestroy(void);
			
			/**
			 * @brief Asynchronous removing the object
			 */
			void RemoveObject(void);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] _objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EObject"; };
		protected:
			/**
			 * @brief Add a specific event Id in the list to prevent wrong link on a EObject
			 * @param[in] _generateEventId event Id to add
			 */
			void AddEventId(const char * _generateEventId);
			
			/**
			 * @brief Generate event on all registered EObject
			 * @param[in] _generateEventId event Id that is curetly generated
			 * @param[in] _data data associated with the event
			 */
			void GenerateEventId(const char * _generateEventId, const etk::UString& _data = "");
			
			/**
			 * @brief Generate Multicast event on all EObject requested the event
			 * @param[in] _messageId Event Id that is generated
			 * @param[in] _data String that is send at all the destinations
			 */
			void SendMultiCast(const char* const _messageId, const etk::UString& _data = "");
			
			/**
			 * @brief Register of the arrival of a Multicast message
			 * @param[in] _messageId Event Id waiting for...
			 */
			void RegisterMultiCast(const char* const _messageId);
		public:
			/**
			 * @brief Register an EObject over an other to get event on the second...
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _eventId Event generate inside the object
			 * @param[in] _eventIdgenerated event generated when call the distant EObject.OnReceiveMessage(...)
			 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
			 */
			void RegisterOnEvent(ewol::EObject * _destinationObject,
			                     const char * _eventId,
			                     const char * _eventIdgenerated = NULL,
			                     const etk::UString& _overloadData="");
			
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] _removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 */
			virtual void OnObjectRemove(ewol::EObject * _removeObject);
			
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] _msg Message handle
			 */
			virtual void OnReceiveMessage(const ewol::EMessage& _msg) { };
		private:
			etk::Vector<ewol::EConfigElement> m_listConfig;
		protected:
			/**
			 * @brief the EObject add a configuration capabilities
			 * @param[in] _config Configuration name.
			 * @param[in] _type Type of the config.
			 * @param[in] _control control of the current type.
			 * @param[in] _description Descritpion on the current type.
			 * @param[in] _default Default value of this parameter.
			 */
			void RegisterConfig(const char* _config, const char* _type=NULL, const char* _control=NULL, const char* _description=NULL, const char* _default=NULL);
			/**
			 * @brief Configuration requested to the curent EObject
			 * @param[in] _conf Configuration handle.
			 * @return true if the parametere has been used
			 */
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			/**
			 * @brief Receive a configuration message from an other element system or from the curent EObject
			 * @param[in] _config Configuration name.
			 * @param[out] _result Result of the request.
			 * @return true if the config is set
			 */
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const ;
		public:
			/** 
			 * @brief Get all the configuration list
			 * @return The list of all parameter availlable in the widget
			 */
			virtual const etk::Vector<ewol::EConfigElement>& GetConfigList(void) { return m_listConfig; };
			/**
			 * @brief Configuration requested to the curent EObject (systrem mode)
			 * @param[in] _conf Configuration handle.
			 * @return true if config set correctly...
			 */
			bool SetConfig(const ewol::EConfig& _conf) { return OnSetConfig(_conf); };
			bool SetConfig(const etk::UString& _config, const etk::UString& _value); // need a search ...
			bool SetConfigNamed(const etk::UString& _name, const etk::UString& _config, const etk::UString& _value); // need a search ...
			bool SetConfigNamed(const etk::UString& _name, const ewol::EConfig& _conf);
			/**
			 * @brief Configuration Get from the curent EObject (systrem mode)
			 * @param[in] _config Configuration name.
			 * @return the config properties
			 */
			etk::UString GetConfig(const char* _config) const;
			etk::UString GetConfig(const etk::UString& _config) const; // need search
		protected:
			etk::UString m_name; //!< name of the element ...
		public:
			/**
			 * @brief Get the eObject name
			 * @return The requested name
			 */
			const etk::UString& GetName(void) const { return m_name; };
			/**
			 * @brief Get the Widget name
			 * @param[in] _name The new name
			 */
			void SetName(const etk::UString& _name) { m_name=_name; };
		public:
			/**
			 * @brief Load properties with an XML node.
			 * @param[in] _node Pointer on the tinyXML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool LoadXML(exml::Element* _node);
			/**
			 * @brief Store properties in this XML node.
			 * @param[in,out] _node Pointer on the tinyXML node.
			 * @return true : All has been done corectly.
			 * @return false : An error occured.
			 */
			virtual bool StoreXML(exml::Element* _node) const;
		public:
			/**
			 * @breif Get the current EObject manager.
			 * @return the requested object manager.
			 */
			ewol::EObjectManager& GetEObjectManager(void);
			/**
			 * @breif Get the current EObject Message Multicast manager.
			 * @return the requested object manager.
			 */
			ewol::EObjectMessageMultiCast& GetEObjectMessageMultiCast(void);
			/**
			 * @brief Get the curent the system inteface.
			 * @return current reference on the instance.
			 */
			eSystem& GetSystem(void);
	};
};

#endif




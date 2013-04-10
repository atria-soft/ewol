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

namespace ewol {
	namespace EObjectMessageMultiCast {
		void Init(void);
		void UnInit(void);
		void AnonymousSend(const char* const messageId, const etk::UString& data);
	};
	
	class EObject;
	/**
	 * local class for event generation
	 */
	class EventExtGen {
		public:
			const char* localEventId; //!< local event Id generation
			ewol::EObject* destEObject; //!< destination widget that might be call
			const char* destEventId; //!< Generated event ID on the distant widget
	};
	
	/**
	 * @brief Basic message classes for ewol system
	 * 
	 * this class mermit at every EObject to communicate between them.
	 */
	class EObject {
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
			 * @brief Get the UniqueId of the EObject
			 * @return the requested ID
			 */
			int32_t GetId(void);
			
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
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EObject"; };
		protected:
			/**
			 * @brief Add a specific event Id in the list to prevent wrong link on a EObject
			 * @param[in] generateEventId event Id to add
			 */
			void AddEventId(const char * generateEventId);
			
			/**
			 * @brief Generate event on all registered EObject
			 * @param[in] generateEventId event Id that is curetly generated
			 * @param[in] data data associated with the event
			 */
			void GenerateEventId(const char * generateEventId, const etk::UString& data = "");
			
			/**
			 * @brief Generate Multicast event on all EObject requested the event
			 * @param[in] messageId Event Id that is generated
			 * @param[in] data String that is send at all the destinations
			 */
			void SendMultiCast(const char* const messageId, const etk::UString& data = "");
			
			/**
			 * @brief Register of the arrival of a Multicast message
			 * @param[in] messageId Event Id waiting for...
			 */
			void RegisterMultiCast(const char* const messageId);
		public:
			/**
			 * @brief Register an EObject over an other to get event on the second...
			 * @param[in] destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] eventId Event generate inside the object
			 * @param[in] eventIdgenerated event generated when call the distant EObject.OnReceiveMessage(...)
			 */
			void RegisterOnEvent(ewol::EObject * destinationObject, const char * eventId, const char * eventIdgenerated = NULL);
			
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data) { };
			
		protected:
			etk::UString m_name; //!< name of the element ...
		public:
			/**
			 * @brief Get the eObject name
			 * @return The requested name
			 */
			const etk::UString& GetName(void) { return m_name; };
			/**
			 * @brief Get the Widget name
			 * @param[in] name The new name
			 */
			void SetName(const etk::UString& name) { m_name=name; };
	};
};

#endif



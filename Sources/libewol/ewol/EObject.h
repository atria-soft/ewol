/**
 *******************************************************************************
 * @file ewol/EObject.h
 * @brief basic ewol object (header)
 * @author Edouard DUPIN
 * @date 24/02/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_E_OBJECT_H__
#define __EWOL_E_OBJECT_H__

#include <etk/Types.h>
#include <etk/UString.h>
#include <etk/VectorType.h>

namespace ewol {
	namespace EObjectMessageMultiCast {
		void Init(  void);
		void UnInit(void);
		void AnonymousSend(const char* const messageId, etk::UString& data);
	};
	
	
	class EObject;
	/**
	 * local class for event generation
	 */
	class EventExtGen {
		public:
			const char*    localEventId;       //!< local event Id generation
			ewol::EObject* destEObject;        //!< destination widget that might be call
			const char*    destEventId;        //!< Generated event ID on the distant widget
			etk::UString   destData;           //!< destination data
	};
	
	/**
	 * @brief Basic message classes for ewol system
	 * 
	 * this class mermit at every EObject to communicate between them.
	 */
	class EObject {
		private:
			int32_t                        m_uniqueId;          //!< Object UniqueID ==> TODO : Check if it use is needed
			etk::VectorType<EventExtGen*>  m_externEvent;       //!< Generic list of event generation for output link
			etk::VectorType<const char*>   m_availlableEventId; //!< List of all event availlable for this widget
			
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
			 * @param ---
			 * @return the requested ID
			 */
			int32_t GetId(void);
			
		protected:
			/**
			 * @brief Add a specific event Id in the list to prevent wrong link on a EObject
			 * @param[in] generateEventId event Id to add
			 * @return ---
			 */
			void AddEventId(const char * generateEventId);
			
			/**
			 * @brief Generate event on all registered EObject
			 * @param[in] generateEventId event Id that is curetly generated
			 * @return ---
			 */
			void GenerateEventId(const char * generateEventId);
			
			/**
			 * @brief Generate Multicast event on all EObject requested the event
			 * @param[in] messageId Event Id that is generated
			 * @param[in] data String that is send at all the destinations
			 * @return ---
			 */
			//void SendMultiCast(const char* const messageId, etk::UString& data);
			void SendMultiCast(const char* const messageId, etk::UString data = "");
			
			/**
			 * @brief Register of the arrival of a Multicast message
			 * @param[in] messageId Event Id waiting for...
			 * @return ---
			 */
			void RegisterMultiCast(const char* const messageId);
		public:
			/**
			 * @brief Register an EObject over an other to get event on the second...
			 * @param[in] destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] eventId Event generate inside the object
			 * @param[in] eventIdgenerated event generated when call the distant EObject.OnReceiveMessage(...)
			 * @param[in] data data associated with the event
			 * @return ---
			 */
			void RegisterOnEvent(ewol::EObject * destinationObject, const char * eventId, const char * eventIdgenerated = NULL, etk::UString data = "");
			
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		
	};
};




#endif



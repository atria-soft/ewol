/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_E_MULTICAST_H__
#define __EWOL_E_MULTICAST_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <etk/Vector.h>
#include <exml/exml.h>

#include <ewol/renderer/EObject.h>

namespace ewol {
	class EMultiCast {
		private:
			class MessageList {
				public:
					MessageList(const char* _message=NULL, ewol::EObject* _object=NULL) : m_message(_message), m_object(_object) { }
					const char* m_message;
					ewol::EObject* m_object;
			};
			etk::Vector<MessageList> m_messageList; //!< List of all message ...
		public:
			EMultiCast();
			~EMultiCast(void);
			void anonymousSend(const char* const _messageId, const etk::UString& _data) { send(NULL, _messageId, _data); };
			void send(ewol::EObject* _object, const char* const _message, const etk::UString& _data);
			void rm(ewol::EObject* _object);
			void add(ewol::EObject* _object, const char* const _message);
	};
};

#endif
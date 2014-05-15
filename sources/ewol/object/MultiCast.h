/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_OBJECT_MULTICAST_H__
#define __EWOL_OBJECT_MULTICAST_H__

#include <etk/types.h>
#include <etk/types.h>
#include <vector>
#include <exml/exml.h>

#include <ewol/object/Object.h>

namespace ewol {
	namespace object {
		class MultiCast {
			private:
				class MessageList {
					public:
						MessageList(const char* _message=NULL, ewol::Object* _object=NULL) :
						  m_message(_message), m_object(_object) {
							
						}
						const char* m_message;
						ewol::Object* m_object;
				};
				std::vector<MessageList> m_messageList; //!< List of all message ...
			public:
				MultiCast();
				~MultiCast();
				void anonymousSend(const char* const _messageId, const std::string& _data) {
					send(NULL, _messageId, _data);
				};
				void send(ewol::Object* _object, const char* const _message, const std::string& _data);
				void rm(ewol::Object* _object);
				void add(ewol::Object* _object, const char* const _message);
		};
	};
};

#endif

/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
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
						MessageList(const char* _message=nullptr, std::shared_ptr<ewol::Object> _object=nullptr) :
						  m_message(_message), m_object(_object) {
							
						}
						const char* m_message;
						std::weak_ptr<ewol::Object> m_object;
				};
				std::vector<MessageList> m_messageList; //!< List of all message ...
			public:
				MultiCast();
				virtual ~MultiCast();
				void clear();
				void anonymousSend(const char* const _messageId, const std::string& _data) {
					send(nullptr, _messageId, _data);
				};
				void send(const std::shared_ptr<ewol::Object>& _object, const char* const _message, const std::string& _data);
				void rm(const std::shared_ptr<ewol::Object>& _object);
				void add(const std::shared_ptr<ewol::Object>& _object, const char* const _message);
		};
	};
};

#endif

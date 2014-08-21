/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_OBJECT_MESSAGE_H__
#define __EWOL_OBJECT_MESSAGE_H__

#include <ewol/debug.h>

namespace ewol {
	namespace object {
		class Message {
			private:
				const char* m_event; //!< Event pointer  == > unique Id define by the system ...
				std::string m_data; //!< compositing additionnal message Value.
			public:
				Message(const char* _message,
				        const std::string& _data) :
				  m_event(_message),
				  m_data(_data) {
					
				};
				void setMessage(const char* _message) {
					m_event = _message;
				};
				inline const char* getMessage() const {
					return m_event;
				};
				void setData(const std::string& _data) {
					m_data = _data;
				};
				inline const std::string& getData() const {
					return m_data;
				};
		};
		std::ostream& operator <<(std::ostream& _os, const ewol::object::Message& _obj);
	};
};


#endif


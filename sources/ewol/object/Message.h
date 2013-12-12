/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_OBJECT_MESSAGE_H__
#define __EWOL_OBJECT_MESSAGE_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol {
	namespace object {
		class Message {
			private:
				ewol::Object* m_callerObject; //!< Caller class.
				const char* m_event; //!< Event pointer  == > unique Id define by the system ...
				std::string m_data; //!< compositing additionnal message Value.
			public:
				Message(ewol::Object* _caller,
				        const char* _message,
				        const std::string& _data) :
				  m_callerObject(_caller),
				  m_event(_message),
				  m_data(_data) {
					
				};
				void setCaller(ewol::Object* _caller) {
					m_callerObject = _caller;
				};
				inline ewol::Object* getCaller(void) const {
					return m_callerObject;
				};
				void setMessage(const char* _message) {
					m_event = _message;
				};
				inline const char* getMessage(void) const {
					return m_event;
				};
				void setData(const std::string& _data) {
					m_data = _data;
				};
				inline const std::string& getData(void) const {
					return m_data;
				};
		};
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::object::Message& _obj);
};


#endif


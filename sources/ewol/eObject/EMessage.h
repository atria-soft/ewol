/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_E_MESSAGE_H__
#define __EWOL_E_MESSAGE_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol {
	class EMessage {
		private:
			ewol::EObject* m_callerObject; //!< Caller class.
			const char* m_event; //!< Event pointer ==> unique Id define by the system ...
			etk::UString m_data; //!< compositing additionnal message Value.
		public:
			EMessage(ewol::EObject* _caller,
			         const char* _message,
			         const etk::UString& _data) :
				m_callerObject(_caller),
				m_event(_message),
				m_data(_data)
			{ };
			void SetCaller(ewol::EObject* _caller) { m_callerObject = _caller; };
			inline ewol::EObject* GetCaller(void) const { return m_callerObject; };
			void SetMessage(const char* _message) { m_event = _message; };
			inline const char* GetMessage(void) const { return m_event; };
			void SetData(const etk::UString& _data) { m_data = _data; };
			inline const etk::UString& GetData(void) const { return m_data; };
	};
	etk::CCout& operator <<(etk::CCout &os, const ewol::EMessage &obj);
};


#endif


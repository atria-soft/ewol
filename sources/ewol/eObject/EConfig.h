/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_E_CONFIG_H__
#define __EWOL_E_CONFIG_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol {
	class EConfig {
		private:
			const char* m_config; //!< config properties.
			etk::UString m_data; //!< compositing additionnal message Value.
		public:
			EConfig(const char* _config,
			        const etk::UString& _data) :
				m_config(_config),
				m_data(_data)
			{ };
			void SetConfig(const char* _config) { m_config = _config; };
			inline const char* GetConfig(void) const { return m_config; };
			void SetData(const etk::UString& _data) { m_data = _data; };
			inline const etk::UString& GetData(void) const { return m_data; };
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EConfig& _obj);
	
	class EConfigElement {
		private:
			const char* m_config; //!< config properties (used for XML properties ==> only : "[0-9a-zA-Z\-]" ==> this is a regExp control.
			const char* m_type; //!< type of the config[integer,float,string,reg-exp,list].
			const char* m_control; //!< control the type of the type set ( integer:[0..256], regExp: "[0-9a-zA-Z]", list:[plop,plop2,plop3] )
			const char* m_description; //!< description to help user to configure it.
			const char* m_default; //!< default value ...
		public:
			// note : no parameter capability is needed to create element in case of vector stoarage.
			EConfigElement(const char* _config=NULL,
			               const char* _type=NULL,
			               const char* _control=NULL,
			               const char* _description=NULL,
			               const char* _default=NULL) :
				m_config(_config),
				m_type(_type),
				m_control(_control),
				m_description(_description),
				m_default(_default)
			{ };
			inline const char* GetConfig(void) const { return m_config; };
			inline const char* GetType(void) const { return m_type; };
			inline const char* GetControl(void) const { return m_control; };
			inline const char* GetDescription(void) const { return m_description; };
			inline const char* GetDefault(void) const { return m_default; };
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EConfigElement& _obj);
};


#endif


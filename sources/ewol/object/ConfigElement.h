/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_OBJECT_CONFIG_ELEMENT_H__
#define __EWOL_OBJECT_CONFIG_ELEMENT_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol {
	namespace object {
		class ConfigElement {
			private:
				const char* m_config; //!< config properties (used for XML properties  == > only : "[0-9a-zA-Z\-]" ==> this is a regExp control.
				const char* m_type; //!< type of the config[integer,float,string,reg-exp,list].
				const char* m_control; //!< control the type of the type set ( integer:[0..256], regExp: "[0-9a-zA-Z]", list:[plop,plop2,plop3] )
				const char* m_description; //!< description to help user to configure it.
				const char* m_default; //!< default value ...
			public:
				// note : no parameter capability is needed to create element in case of vector stoarage.
				ConfigElement(const char* _config = NULL,
				              const char* _type = NULL,
				              const char* _control = NULL,
				              const char* _description = NULL,
				              const char* _default = NULL) :
				  m_config(_config),
				  m_type(_type),
				  m_control(_control),
				  m_description(_description),
				  m_default(_default) {
					
				};
				inline const char* getConfig(void) const {
					return m_config;
				};
				inline const char* getType(void) const {
					return m_type;
				};
				inline const char* getControl(void) const {
					return m_control;
				};
				inline const char* getDescription(void) const {
					return m_description;
				};
				inline const char* getDefault(void) const {
					return m_default;
				};
		};
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::object::ConfigElement& _obj);
};


#endif


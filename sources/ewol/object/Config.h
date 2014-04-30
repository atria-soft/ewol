/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_OBJECT_CONFIG_H__
#define __EWOL_OBJECT_CONFIG_H__

#include <ewol/debug.h>

namespace ewol {
	namespace object {
		class Config {
			private:
				const char* m_config; //!< config properties.
				std::string m_data; //!< compositing additionnal message Value.
			public:
				Config(const char* _config,
				       const std::string& _data) :
				  m_config(_config),
				  m_data(_data) {
					
				};
				void setConfig(const char* _config) {
					m_config = _config;
				};
				inline const char* getConfig(void) const {
					return m_config;
				};
				void setData(const std::string& _data) {
					m_data = _data;
				};
				inline const std::string& getData(void) const {
					return m_data;
				};
		};
	};
	std::ostream& operator <<(std::ostream& _os, const ewol::object::Config& _obj);
};


#endif


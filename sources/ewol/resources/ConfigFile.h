/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __SIMPLE_CONFIG_FILE_H__
#define __SIMPLE_CONFIG_FILE_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/resources/Resource.h>

namespace ewol
{
	class SimpleConfigElement
	{
		public:
			etk::UString m_paramName;
		private:
			etk::UString m_value;
			int32_t      m_valueInt;
			float        m_valuefloat;
		public:
			SimpleConfigElement(const etk::UString& _name) :
				m_paramName(_name),
				m_value(""),
				m_valueInt(0),
				m_valuefloat(0.0) { };
			~SimpleConfigElement(void) { };
			void          Parse(const etk::UString& value);
			int32_t       GetInteger(void) { return m_valueInt; };
			float         GetFloat(void)   { return m_valuefloat; };
			etk::UString& GetString(void)  { return m_value; };
	};
	
	class ConfigFile : public ewol::Resource
	{
		private:
			etk::Vector<ewol::SimpleConfigElement*> m_list;
			etk::UString                            m_errorString;
		protected:
			ConfigFile(const etk::UString& _filename);
			virtual ~ConfigFile(void);
		public:
			const char* GetType(void) { return "ewol::SimpleConfigFile"; };
			void Reload(void);
			
			int32_t Request(const etk::UString& _paramName);
			
			int32_t       GetInteger(int32_t id) { if (id<0) { return 0; } return m_list[id]->GetInteger(); };
			float         GetFloat(int32_t id)   { if (id<0) { return 0; } return m_list[id]->GetFloat();   };
			etk::UString& GetString(int32_t id)  { if (id<0) { return m_errorString; } return m_list[id]->GetString();  };
		public:
			/**
			 * @brief Keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static ewol::ConfigFile* Keep(const etk::UString& _filename);
			/**
			 * @brief Release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void Release(ewol::ConfigFile*& _object);
	};
};

#endif


/**
 *******************************************************************************
 * @file ewol/impleConfigFile.h
 * @brief ewol basic file configuration system (header)
 * @author Edouard DUPIN
 * @date 06/11/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __SIMPLE_CONFIG_FILE_H__
#define __SIMPLE_CONFIG_FILE_H__
	#include <etk/Types.h>
	#include <ewol/Debug.h>
	#include <ewol/Resource.h>
	
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
				SimpleConfigElement(etk::UString name) :
					m_paramName(name),
					m_value(""),
					m_valueInt(0),
					m_valuefloat(0.0) { };
				~SimpleConfigElement(void) { };
				void          Parse(etk::UString value);
				int32_t       GetInteger(void) { return m_valueInt; };
				float         GetFloat(void)   { return m_valuefloat; };
				etk::UString& GetString(void)  { return m_value; };
		};
		
		class SimpleConfigFile : public ewol::Resource
		{
			private:
				etk::Vector<ewol::SimpleConfigElement*> m_list;
			public:
				SimpleConfigFile(etk::UString& filename);
				virtual ~SimpleConfigFile(void);
				const char* GetType(void) { return "ewol::SimpleConfigFile"; };
				void Reload(void);
				
				int32_t Request(etk::UString paramName);
				
				int32_t       GetInteger(int32_t id) { return m_list[id]->GetInteger(); };
				float         GetFloat(int32_t id)   { return m_list[id]->GetFloat();   };
				etk::UString& GetString(int32_t id)  { return m_list[id]->GetString();  };
		};
	};
#endif


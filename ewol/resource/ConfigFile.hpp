/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Map.hpp>
#include <ewol/debug.hpp>
#include <ejson/ejson.hpp>
#include <gale/resource/Resource.hpp>

namespace ewol {
	namespace resource {
		class ConfigFile : public gale::Resource {
			private:
				ejson::Document m_doc;
				etk::Map<etk::String, ejson::Value> m_list;
			protected:
				ConfigFile();
				void init(const etk::String& _filename);
			public:
				virtual ~ConfigFile();
				DECLARE_RESOURCE_NAMED_FACTORY(ConfigFile);
			public:
				void reload();
				
				int32_t request(const etk::String& _paramName);
				
				double getNumber(int32_t _id);
				etk::String getString(int32_t _id);
				bool getBoolean(int32_t _id);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the configuration file.
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static ememory::SharedPtr<ewol::resource::ConfigFile> keep(const etk::String& _filename);
		};
	};
};

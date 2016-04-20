/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <etk/Hash.h>
#include <ewol/debug.h>
#include <ejson/ejson.h>
#include <gale/resource/Resource.h>

namespace ewol {
	namespace resource {
		class ConfigFile : public gale::Resource {
			private:
				ejson::Document m_doc;
				etk::Hash<ejson::Value> m_list;
			protected:
				ConfigFile();
				void init(const std::string& _filename);
			public:
				virtual ~ConfigFile();
				DECLARE_RESOURCE_NAMED_FACTORY(ConfigFile);
			public:
				void reload();
				
				int32_t request(const std::string& _paramName);
				
				double getNumber(int32_t _id);
				const std::string& getString(int32_t _id);
				bool getBoolean(int32_t _id);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the configuration file.
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static std::shared_ptr<ewol::resource::ConfigFile> keep(const std::string& _filename);
		};
	};
};

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
				ejson::Document this.doc;
				etk::Map<String, ejson::Value> this.list;
			protected:
				ConfigFile();
				void init( etk::Uri _filename);
			public:
				 ~ConfigFile();
				DECLARE_RESOURCE_URI_FACTORY(ConfigFile);
			public:
				void reload();
				
				int request( String _paramName);
				
				double getNumber(int _id);
				String getString(int _id);
				boolean getBoolean(int _id);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the configuration file.
				 * @return pointer on the resource or null if an error occured.
				 */
				static ememory::Ptr<ewol::resource::ConfigFile> keep( String _filename);
		};
	};
};

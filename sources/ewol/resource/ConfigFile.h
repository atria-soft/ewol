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
#include <etk/Hash.h>
#include <ewol/debug.h>
#include <ejson/ejson.h>
#include <ewol/resource/Resource.h>

namespace ewol {
	namespace resource {
		class ConfigFile : public ewol::Resource {
			private:
				ejson::Document m_doc;
				etk::Hash<ejson::Value*> m_list;
			protected:
				ConfigFile(const std::string& _filename);
				virtual ~ConfigFile();
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
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ewol::resource::ConfigFile* keep(const std::string& _filename);
				/**
				 * @brief release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void release(ewol::resource::ConfigFile*& _object);
		};
	};
};

#endif


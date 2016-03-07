/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
		 */
		class Composer : public ewol::widget::Container {
			protected:
				/**
				 * @brief Constructor
				 */
				Composer();
			public:
				DECLARE_WIDGET_FACTORY(Composer, "Composer");
				/**
				 * @brief Destructor
				 */
				virtual ~Composer();
				/**
				 * @brief load a composition with a file
				 * @param[in] _fileName Name of the file
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				bool loadFromFile(const std::string& _fileName);
				/**
				 * @brief load a composition with a file
				 * @param[in] _composerXmlString xml to parse directly
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				bool loadFromString(const std::string& _composerXmlString);
		};
		std::shared_ptr<ewol::Widget> composerGenerateString(const std::string& _data = "");
		std::shared_ptr<ewol::Widget> composerGenerateFile(const std::string& _data = "");
	};
};

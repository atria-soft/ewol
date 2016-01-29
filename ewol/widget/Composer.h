/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_WIDGET_COMPOSER_H__
#define __EWOL_WIDGET_COMPOSER_H__

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
			public:
				enum composerMode {
					None,
					String,
					file
				};
			protected:
				/**
				 * @brief Constructor
				 */
				Composer();
				/**
				 * @brief Constructor
				 * @param[in] _mode mode of parsing the string
				 * @param[in] _data file/directString data to generate compositing of the widget..
				 */
				void init(enum ewol::widget::Composer::composerMode _mode = ewol::widget::Composer::None, const std::string& _data = "");
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
		std::shared_ptr<ewol::Widget> composerGenerate(enum ewol::widget::Composer::composerMode _mode = ewol::widget::Composer::None, const std::string& _data = "");
	};
};

#endif
